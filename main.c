#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include "estimator.h"
struct adjustment
{
  int from;
  int to;
  float amount;
  float current_max_time;
};


int main(int argc, char** argv)
{
  const static int size = 52;
  double power[size] = {115, 114, 113, 112, 111, 
                     110, 109, 108, 107, 106, 
                     105, 104, 103, 102, 101, 
                     100, 99, 98, 97, 96, 95, 
                     94, 93, 92, 91, 90, 89, 
                     88, 87, 86, 85, 84, 83, 
                     82, 81, 80, 79, 78, 77, 
                     76, 75, 74, 73, 72, 71, 
                     70, 69, 68, 67, 66, 65, 64};

  double times[size] = {163.21, 163.133, 162.991, 163.239, 
                        163.555, 163.328, 163, 162.979, 163.467, 
                        163.007, 163.251, 163.172, 163.508, 
                        163.237, 163.148, 163.04, 163.96, 
                        165.358, 165.717, 166.549, 167.118, 
                        167.887, 168.771, 170.231, 171.461, 
                        172.481, 173.051, 173.822, 174.663, 
                        176.108, 176.753, 178.275, 179.087, 
                        180.204, 181.856, 183.431, 184.244, 
                        185.467, 186.802, 188.932, 191.233, 
                        192.752, 195.378, 198.025, 201.18, 
                        204.024, 208.127, 211.4, 215.313, 
                        219.024, 223.137, 227.409};

    double ests[4] = {1.64479, 1.65966, 1.66107, 2.27343};

    
    Estimator estimator(times, power, size);
    double new_est = estimator.estimate(65, ests[0]);
    printf("Original time %f new time %f\n", ests[0], new_est);
 
}
