response-time-test
==================
Usage: 
>$ Usage: schedAnalysis numTests numTasks minPeriod maxPeriod targetUtil
, where 
  1) numTests: the number of iterations for benchmarking
  2) numTasks: the maximum number of tasks that are generated for one test
  3) minPeriod: the minimum value of task periods (should be larger than 1)
  4) maxPeriod: the maximum value of task periods
  5) targetUtil: the total utilizations of the generated tasks for one test
                 actual value could be larger than this (should be less than 1)

Note:
When numTasks increases, the algorithm takes longer.
When maxPeriod increases, the algorithm tasks longer.
targetUtil is an important factor to make a taskset (un)schedulable.

Some examples:
./schedAnalysis 10 10 2 100 0.8
0.000141859 seconds elapsed to run response-time test 10 times.
./schedAnalysis 10 1000 2 100 0.8
0.00316095 seconds elapsed to run response-time test 10 times.
./schedAnalysis 10 1000 2 10000 0.8
5.56471 seconds elapsed to run response-time test 10 times.

