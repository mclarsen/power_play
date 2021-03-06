#ifndef estimator_h
#define estimator_h

#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

class Estimator
{
protected:    
    double m_percent_change;
    std::vector<double> m_times;
    std::vector<double> m_power;
    bool m_is_verbose;
public:
    // Give me un-normalized times with 
    // times[0] being the fastest and times[n-1] slowest
    // power[0] highest and power[n-1] lowest
    Estimator(const double *times, const double *power, const int array_size, bool verbose)
    {
      assert(times[0] <= times[array_size - 1]);
      assert(power[0] >= power[array_size - 1]);
      assert(array_size > 0);

      m_power.resize(array_size);
      m_times.resize(array_size);

      m_is_verbose = verbose;

      double max_val = -1;
      double min_val = 1e30;
      //
      // normalize the time values
      //
      for(int i = 0; i < array_size; ++i)
      {
        max_val = std::max(max_val, times[i]);
        min_val = std::min(min_val, times[i]);
      }

      for(int i = 0; i < array_size; ++i)
      {
        m_times[i] = (times[i]-min_val) / (max_val - min_val);
        m_power[i] = power[i];
      }
      
      m_percent_change = (max_val - min_val) / max_val;
    }

    double get_max_power() const
    {
      return m_power[0];
    }

    double get_min_power() const
    {
      const int size = static_cast<int>(m_power.size());
      return m_power[size-1];
    }

    double estimate(double power_value, double orig_estimate) const
    {
      int min_index = 0;
      const int size = static_cast<int>(m_power.size());
      //
      // Don't ask for a power value we cant give
      //
      assert(power_value <= get_max_power() && 
             power_value >= get_min_power());

      for(int i = 0; i < size; ++i)
      {
        if(m_power[i] >= power_value) 
        {
          min_index++;
        }
        else break;
      }
      if(min_index == size)
      {
        min_index = size-1;
      }
      int max_index = min_index - 1; 
      //std::cout<<"power_value="<<power_value<<"\n";
      //std::cout<<"min_index="<<min_index<<", max_index="<<max_index<<"\n";
      //std::cout<<"m_power[min_index]="<<m_power[min_index]<<", m_power[max_index]="<<m_power[max_index]<<"\n";
      assert(min_index < size);
      assert(max_index > -1);

      double delta = (power_value - m_power[min_index])/(m_power[max_index] - m_power[min_index]);
      assert(delta >= 0.0 && delta <= 1);
      double normalized_time = m_times[min_index] + delta * (m_times[max_index] - m_times[min_index]);
      double diff = orig_estimate * (1.0 + m_percent_change) - orig_estimate;
      double time = orig_estimate + normalized_time * diff;
      return time;
    }
};  // class estimator 

struct Adjustment
{
  int from;
  int to;
  double amount;
};

struct PowerAllocation
{
  // Pointer to data share between other instances 
  Estimator *m_estimator;
  const std::vector<double> *m_orig_estimates;
  
  std::vector<double> m_power_values;
  std::vector<double> m_times;

  PowerAllocation(const int size, 
                  const double ave_power_per_node, // evenly distributed power given some cap
                  const std::vector<double> *orig_estimates,
                  Estimator *estimator)
    : m_power_values(size, ave_power_per_node),
      m_orig_estimates(orig_estimates),
      m_estimator(estimator),
      m_times(size,0)
  {
    assert(size == m_times.size());
    assert(size == m_orig_estimates->size());
    assert(size > 0);
    assert(m_estimator != NULL);
    // init times given ave power per node 
    for(int i = 0; i < size; ++i)
    {
      m_times[i] = m_estimator->estimate(ave_power_per_node, m_orig_estimates->at(i));
    }

    const int vsize = static_cast<int>(m_times.size());
    std::cout<<"Node | Power  | Time\n";
    std::cout<<"--------------------\n";
    for(int i = 0; i < vsize; ++i)
    {
      if(m_times[i] == get_max_runtime())
      {
        std::cout<<std::setw(2)<<i<<"   | "<<std::setprecision(3)<<std::setw(6)<<m_power_values[i]<<" | "<<std::setprecision(5)<<m_times[i]<<" *\n";
      }
      else
      {
        std::cout<<std::setw(2)<<i<<"   | "<<std::setprecision(3)<<std::setw(6)<<m_power_values[i]<<" | "<<std::setprecision(5)<<m_times[i]<<"\n";
      }
    }
    std::cout<<"Runtime: "<<std::setprecision(5)<<get_max_runtime()<<"\n";
  }
  
  int get_max_index() const 
  {
    const int size = static_cast<int>(m_times.size());
    int max_index = 0; 
    double max_value = m_times[0];
    for(int i = 1; i < size; ++i)
    {
      if(m_times[i] > max_value) 
      {
        max_index = i;
        max_value = m_times[i]; 
      }
    }
    return max_index;
  } // get max

  double get_max_runtime() const 
  {
    const int size = static_cast<int>(m_times.size());
    double max_value = m_times[0];
    for(int i = 1; i < size; ++i)
    {
      if(m_times[i] > max_value) 
      {
        max_value = m_times[i]; 
      }
    }
    return max_value;
  } // get max runtime

  void apply_adjustment(const Adjustment &adj)
  {
    double power_from     = m_power_values[adj.from];
    double power_to       = m_power_values[adj.to];
    double new_power_to   = power_to + adj.amount;
    double new_power_from = power_from - adj.amount;
    double time_from      = m_orig_estimates->at(adj.from);
    double time_to        = m_orig_estimates->at(adj.to);
    double new_est_from   = m_estimator->estimate(new_power_from, time_from);
    double new_est_to     = m_estimator->estimate(new_power_to, time_to);
    
    // save the values
    m_power_values[adj.from] -= adj.amount;
    m_power_values[adj.to]   +=  adj.amount;
    m_times[adj.from] = new_est_from;
    m_times[adj.to] = new_est_to;
  }

  double check_adjustment(const Adjustment &adj, bool verbose)
  {
    double power_from     = m_power_values[adj.from];
    double power_to       = m_power_values[adj.to];
    double new_power_to   = power_to + adj.amount;
    double new_power_from = power_from - adj.amount;
    double time_from      = m_orig_estimates->at(adj.from);
    double time_to        = m_orig_estimates->at(adj.to);
    double new_est_from   = m_estimator->estimate(new_power_from, time_from);
    double new_est_to     = m_estimator->estimate(new_power_to, time_to);

    if(verbose == true)
    {
      std::cout<<"orig_est m_times new_power new_time\n";
      std::cout<<time_from<<" "<<m_times[adj.from]<<" "<<new_power_from<<" "<<std::setprecision(5)<<new_est_from<<"\n";
      std::cout<<time_to<<" "<<m_times[adj.to]<<" "<<new_power_to<<" "<<std::setprecision(5)<<new_est_to<<"\n";
    }
    
    const int size = static_cast<int>(m_times.size());

    double max_value = m_times[0];

    if(adj.from == 0)
    {
      max_value = new_est_from;
    }
    if(adj.to == 0)
    {
      max_value = new_est_to;
    }

    for(int i = 1; i < size; ++i)
    {
      double val = m_times[i];

      if(adj.from == i)
      {
        val = new_est_from;
      }
      if(adj.to == i)
      {
        val = new_est_to;
      }

      if(val > max_value) 
      {
        max_value = val; 
      }
    }
    return max_value;
  }

  void print()
  {
    const int size = static_cast<int>(m_times.size());
    std::cout<<"Node | Power  | Time\n";
    std::cout<<"--------------------\n";
    for(int i = 0; i < size; ++i)
    {
      if(m_times[i] == get_max_runtime())
      {
        std::cout<<std::setw(2)<<i<<"   | "<<std::setprecision(3)<<std::setw(6)<<m_power_values[i]<<" | "<<std::setprecision(5)<<m_times[i]<<" *\n";
      }
      else
      {
        std::cout<<std::setw(2)<<i<<"   | "<<std::setprecision(3)<<std::setw(6)<<m_power_values[i]<<" | "<<std::setprecision(5)<<m_times[i]<<"\n";
      }
    }
    std::cout<<"Runtime: "<<std::setprecision(5)<<get_max_runtime()<<"\n";
  }
}; //class PowerAllocation

class PowerOptimizer
{
protected:
  Estimator m_estimator; 
  std::vector<double> m_orig_estimates; 
  bool m_is_verbose;
public:
  PowerOptimizer(Estimator &e, 
                 const std::vector<double> &estimates,
                 bool verbose)
    : m_estimator(e),
      m_orig_estimates(estimates),
      m_is_verbose(verbose) 
  {
      
  }

  PowerAllocation optimize(double ave_power_per_node, double power_inc, bool verbose)
  {
    // create the inital power allocation
    PowerAllocation allocation(m_orig_estimates.size(),
                               ave_power_per_node,
                               &m_orig_estimates,
                               &m_estimator);
    bool progress = true;
    const int size = static_cast<int>(m_orig_estimates.size());
    int round  = 0;
    while(progress)
    {
      int bottleneck_idx = allocation.get_max_index();
      double bottleneck_time = allocation.m_times[bottleneck_idx];

      if(allocation.m_power_values[bottleneck_idx] + power_inc > m_estimator.get_max_power())
      {
        if(verbose == true)
        {
          std::cout<<"Breaking because bottleneck is at TDP\n";
        }
        break;
      }
      if(verbose == true)
      {
        std::cout<<"---- Round "<<round<<" ----\n";
      }
        
      Adjustment best_adj;
      double best_adj_time = bottleneck_time; 
      progress = false;
      for(int i = 0; i < size; ++i)
      {
        // Check for conditions that can't be done
        if(i == bottleneck_idx) 
        {
          // we can't give from the bottleneck
          continue;
        }
        if(allocation.m_power_values[i] - power_inc < m_estimator.get_min_power())
        {
          // giving power would result in going below the min power threshold
          continue; 
        }
        
        Adjustment temp;
        temp.to = bottleneck_idx;
        temp.from = i;
        temp.amount = power_inc;
        
        double temp_time = allocation.check_adjustment(temp, verbose);
        //std::cout<<i<<" new time "<<temp_time<<" best "<<best_adj_time<<"\n";
        if(temp_time < best_adj_time)
        {
          best_adj_time = temp_time;  
          best_adj = temp;
          progress = true;
        }
    
      } // for

      if(progress)
      {
        allocation.apply_adjustment(best_adj);
      }
      round++;
    }  // while making progress 
    return allocation;
  }
};
#endif
