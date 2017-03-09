#ifndef estimator_h
#define estimator_h
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

class Estimator
{
protected:    
    double m_percent_change;
    std::vector<double> m_times;
    std::vector<double> m_power;
public:
    //
    // Give me un normalized times with 
    // times[0] being the fastest and times[n-1] slowest
    // power[0] highest and power[n-1] lowest
    Estimator(const double *times, const double *power, const int array_size)
    {
      assert(times[0] <= times[array_size - 1]);
      assert(power[0] >= power[array_size - 1]);
      assert(array_size > 0);

      m_power.resize(array_size);
      m_times.resize(array_size);

      double max_val= -1;
      double min_val= 1e30;
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

    double estimate(double power_value, double orig_estimate) const
    {
      int min_index = 0;
      const int size = static_cast<int>(m_power.size());
      //
      // Don't ask for a power value we cant give
      //
      assert(power_value <= m_power[0] && power_value >= m_power[size - 1]);

      for(int i = 0; i < size; ++i)
      {
        if(m_power[i] >= power_value) 
        {
          min_index++;
        }
        else break;
      }
      int max_index = min_index - 1; 
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
  std::vector<double> *m_orig_estimates;
  
  std::vector<double> m_power_values;
  std::vector<double> m_times;

  PowerAllocation(const int size, 
                  const double max_power,
                  const std::vector<double> &init_estimates,
                  Estimator *estimator)
    : m_power_values(size, max_power),
      m_times(init_estimates),
      m_estimator(estimator)
  {
    assert(size == m_times.size());
    assert(size > 0);
    assert(m_estimator != NULL);
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
      }
    }
    return max_index;
  } // get max

  void apply_adjustment(const Adjustment &adj)
  {
    double power_from =  m_power_values[adj.from];
  } 

}; //class PowerAllocation

class PowerOptimizer
{
protected:
   

};
#endif
