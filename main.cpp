/*
** main.cpp
** 
** Author: Junsung Kim 
**
*/

#include <time.h>
#include <sys/time.h>
#include "task.h"
#include "core.h"
#include "utility.h"

int main(int argc, char* argv[]) {

    if(argc != 6) {
        std::cout << "Usage: schedAnalysis numTests numTasks minPeriod maxPeriod targetUtil" << std::endl;
        return -1;
    }

    int numTests = atoi(argv[1]);
    int numTasks = atoi(argv[2]);
    int minPeriod = atoi(argv[3]);
    int maxPeriod = atoi(argv[4]);
    double targetUtil = atof(argv[5]);
    double timeDiff = 0.0f;

    for(int i=0; i<numTests; i++) {
        generateTaskset(numTasks,minPeriod,maxPeriod,targetUtil);

        Taskset taskSet;
        readTaskset(&taskSet);

        taskSet.sortTasks();

        // response-time test
        timeval tim1,tim2;
        double t1,t2;
        gettimeofday(&tim1, NULL);
        taskSet.doResponseTimeTest();
        gettimeofday(&tim2, NULL);
        t1 = tim1.tv_sec+(tim1.tv_usec/1000000.0);
        t2 = tim2.tv_sec+(tim2.tv_usec/1000000.0);
        timeDiff += (t2-t1);
    }

    std::cout << timeDiff << " seconds elapsed to run response-time test " << numTests << " times." << std::endl; 

    return 0;
}
