#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "estimator.h"

int main(int argc, char** argv)
{
  const char *usage = "\n"
                      "NAME\n"
                      "  power_play - Best power scheduling strategy.\n"
                      "SYNOPSIS\n"
                      "  %s [--help | -h] [-i power_incr] [-p avg_pow_per_node] [-v]\n"
                      "OPTIONS\n"
                      "  --help | -h\n"
                      "     Display this help information, then exit.\n"
                      "  -i incrementor\n"
                      "     Increment of power values.\n"
                      "  -p power\n"
                      "     Power per node.\n"
                      "  -v\n"
                      "     Verbose output.\n"
                      "  -n est_size\n"
                      "     Number of estimates.\n"
                      "  -c config\n"
                      "     Letter of config.\n"
                      "\n";
  if(argc == 1 || argc > 1 && (
             strncmp(argv[1], "--help", strlen("--help")) == 0 ||
             strncmp(argv[1], "-h", strlen("-h")) == 0))
  {
    printf(usage, argv[0]);
    return EXIT_SUCCESS;
  }
  if(argc < 9)
  {
    printf(usage, argv[0]);
    return EXIT_FAILURE;
  }

  int opt;
  double power_inc;
  double ave_power_per_node;
  bool is_verbose = false;
  int est_size;
  char *config = NULL;
  while((opt = getopt(argc, argv, "i:p:vn:c:")) != -1)
  {
    switch(opt)
    {
      case 'i':
        power_inc = atof(optarg);
        break;
      case 'p':
        ave_power_per_node = atof(optarg);
        break;
      case 'v':
        is_verbose = true;
        break;
      case 'n':
        est_size = atoi(optarg);
        break;
      case 'c':
        config = optarg;
        break;
      default:
        std::cerr<<"Error: unknown parameter\n";
        printf(usage, argv[0]);
        return EXIT_FAILURE;
    }
  }

  const static int size = 52;
  double power[size] = {115, 114, 113, 112, 111, 
                        110, 109, 108, 107, 106, 
                        105, 104, 103, 102, 101, 
                        100, 99, 98, 97, 96,
                        95, 94, 93, 92, 91,
                        90, 89, 88, 87, 86, 
                        85, 84, 83, 82, 81,
                        80, 79, 78, 77, 76,
                        75, 74, 73, 72, 71, 
                        70, 69, 68, 67, 66,
                        65, 64};

  double times[size] = {163.21, 163.133, 162.991, 163.239, 163.555,
                        163.328, 163, 162.979, 163.467, 163.007,
                        163.251, 163.172, 163.508, 163.237, 163.148, 
                        163.04, 163.96, 165.358, 165.717, 166.549, 
                        167.118, 167.887, 168.771, 170.231, 171.461,
                        172.481, 173.051, 173.822, 174.663, 176.108,
                        176.753, 178.275, 179.087, 180.204, 181.856,
                        183.431, 184.244, 185.467, 186.802, 188.932,
                        191.233, 192.752, 195.378, 198.025, 201.18,
                        204.024, 208.127, 211.4, 215.313, 219.024,
                        223.137, 227.409};

  if (est_size == 8)
  {
    if(strcmp(config, "A") == 0)
    {
      double ests[est_size] = {1.40017, 0.298057, 1.4095, 0.315991, 1.41141, 0.316148, 1.42095, 0.646492};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "B") == 0)
    {
      double ests[est_size] = {0.0984462, 0.136925, 0.0985987, 0.137073, 0.0985987, 0.137073, 0.098704, 0.137221};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "C") == 0)
    {
      double ests[est_size] = {0.635545, 0.146498, 0.639121, 0.153703, 0.640393, 0.153808, 0.644058, 0.24526};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "D") == 0)
    {
      double ests[est_size] = {0.123995, 0.15892, 0.124162, 0.159088, 0.124162, 0.159088, 0.124329, 0.159255};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else
    {
      std::cerr<<"Error: unknown configuration "<<config<<"\n";
      printf(usage, argv[0]);
      return EXIT_FAILURE;
    }
  }
  else if (est_size == 64)
  {
    if(strcmp(config, "A") == 0)
    {
      double ests[est_size] = {0.0718839, 0.0571076, 0.0571076, 0.0571076, 0.120845, 0.0571076, 0.0571076, 0.0571076, 0.120683, 0.0571076, 0.0571076, 0.0571076, 0.0716591, 0.0571076, 0.0571076, 0.0571076, 0.121076, 0.0571692, 0.0571681, 0.0571076, 0.683754, 0.768937, 0.298057, 0.0571076, 0.684801, 0.778464, 0.315991, 0.0571076, 0.120595, 0.0571076, 0.0571076, 0.0571076, 0.120913, 0.0571706, 0.0571706, 0.0571076, 0.68521, 0.779418, 0.316148, 0.0571076, 0.686265, 0.789161, 0.646493, 0.0571076, 0.120371, 0.0571076, 0.0571076, 0.0571076, 0.0717353, 0.0571076, 0.0571076, 0.0571076, 0.120811, 0.0571692, 0.0571681, 0.0571076, 0.1206, 0.0571706, 0.0571706, 0.0571076, 0.0714823, 0.0571076, 0.0571076, 0.0571076};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "B") == 0)
    {
      double ests[est_size] = {0.0625158, 0.0642943, 0.0671348, 0.0720748, 0.0622835, 0.0639309, 0.0665318, 0.0710001, 0.0623145, 0.0639871, 0.066577, 0.0710224, 0.0625171, 0.0643098, 0.0671373, 0.0720791, 0.0622835, 0.0639309, 0.0665318, 0.0710001, 0.0620673, 0.0635966, 0.0659821, 0.0700228, 0.0620978, 0.0636386, 0.0660241, 0.0700441, 0.0622846, 0.0639328, 0.0665342, 0.0710043, 0.0623145, 0.0639871, 0.0665769, 0.0710224, 0.0620978, 0.0636386, 0.0660241, 0.0700441, 0.0621285, 0.0636808, 0.0660664, 0.0700655, 0.0623157, 0.0639771, 0.0665793, 0.0710264, 0.0625171, 0.0643098, 0.0671373, 0.0720791, 0.0622846, 0.0639328, 0.0665342, 0.0710043, 0.0623157, 0.063977, 0.0665793, 0.0710264, 0.0625183, 0.0642983, 0.0671399, 0.0720837};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "C") == 0)
    {
      double ests[est_size] = {0.0625545, 0.0571076, 0.0571076, 0.0571076, 0.0808647, 0.0571076, 0.0571076, 0.0571076, 0.0807889, 0.0571076, 0.0571076, 0.0571076, 0.0624502, 0.0571076, 0.0571076, 0.0571076, 0.0810187, 0.0571295, 0.0571291, 0.0571076, 0.317146, 0.355104, 0.146498, 0.0571076, 0.317537, 0.358811, 0.153703, 0.0571076, 0.080759, 0.0571076, 0.0571076, 0.0571076, 0.0809423, 0.05713, 0.05713, 0.0571076, 0.317815, 0.359447, 0.153808, 0.0571076, 0.318209, 0.363241, 0.24526, 0.0571076, 0.0806612, 0.0571076, 0.0571076, 0.0571076, 0.062501, 0.0571076, 0.0571076, 0.0571076, 0.0809077, 0.0571295, 0.0571291, 0.0571076, 0.0808143, 0.05713, 0.05713, 0.0571076, 0.0623975, 0.0571076, 0.0571076, 0.0571076};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else if(strcmp(config, "D") == 0)
    {
      double ests[est_size] = {0.0657128, 0.0675463, 0.0699898, 0.073469, 0.0655101, 0.0672622, 0.069611, 0.0729092, 0.0655516, 0.067313, 0.0696602, 0.0729543, 0.0657139, 0.067548, 0.0699918, 0.073471, 0.0655101, 0.0672622, 0.069611, 0.0729092, 0.0653171, 0.0669945, 0.0692542, 0.0723861, 0.0653581, 0.0670439, 0.0693018, 0.0724292, 0.0655112, 0.067264, 0.0696129, 0.0729112, 0.0655516, 0.067313, 0.0696601, 0.0729543, 0.0653581, 0.0670439, 0.0693018, 0.0724292, 0.0653994, 0.0670935, 0.0693495, 0.0724724, 0.0655527, 0.0673148, 0.0696621, 0.0729562, 0.0657139, 0.067548, 0.0699918, 0.073471, 0.0655112, 0.067264, 0.069613, 0.0729112, 0.0655527, 0.0673148, 0.0696621, 0.0729562, 0.065715, 0.0675497, 0.0699939, 0.0734732};
      std::vector<double> estimates(est_size);
      for(int i = 0; i < est_size; ++i)
      {
        estimates[i] = ests[i];
      }

      Estimator estimator(times, power, size, is_verbose);

      PowerOptimizer optimizer(estimator, estimates, is_verbose);
      PowerAllocation alloc = optimizer.optimize(ave_power_per_node, power_inc, is_verbose);
      alloc.print();
    }
    else
    {
      std::cerr<<"Error: unknown configuration "<<config<<"\n";
      printf(usage, argv[0]);
      return EXIT_FAILURE;
    }
  }
}
