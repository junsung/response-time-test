/*
** main.cpp
** 
** Author: Junsung Kim 
**
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "analysis.h"

void generateTaskset(int numTasks, int minPeriod, int maxPeriod, double targetUtil) {
    std::fstream outfile("taskset_input.txt",std::fstream::out);
    int period[numTasks], wcet[numTasks];
    int i;
    double util = 0.0f;

    srand(time(NULL));

    for(i=0; i<numTasks; i++) {
        period[i] = rand() % (maxPeriod-minPeriod+1) + minPeriod;
        wcet[i] = rand() % period[i] + 1;
        util += (double)wcet[i]/period[i];
#ifdef DEBUGPRT
        std::cout << wcet[i] << " " << period[i] << " " << util << std::endl;
#endif
    }

    if(util > targetUtil) {
        double ratio = util/targetUtil;
        util = 0.0f;
        for(i=0; i<numTasks; i++) {
            double tmpWcet = (double)wcet[i] / ratio;
            wcet[i] = (int)ceil(tmpWcet);
            util += (double)wcet[i]/period[i];
            if(util > 1.0) {
                numTasks=i;
                break;
            }
#ifdef DEBUGPRT
            std::cout << wcet[i] << " " << period[i] << " " << util << std::endl;
#endif
        }
    }

    for(i=0; i<numTasks; i++) {
        outfile << i+1 << " ";
        outfile << "Task" << i+1 << " ";
        outfile << wcet[i] << " ";
        outfile << period[i] << " " << period[i];
        outfile << std::endl;
    }
}

void readTaskset(Taskset* pTaskset) {
    std::fstream infile("taskset_input.txt",std::fstream::in);
  
    std::string line;
    std::string item;
  
    getline(infile,line);
  
    while(!infile.eof()) {

      int id, wcet, period, deadline;
      std::string name;
      std::istringstream lineStr(line);
  
      lineStr >> item;
      id = atoi(item.c_str());
      lineStr >> item;
      name = item;
      lineStr >> item;
      wcet = atoi(item.c_str());
      lineStr >> item;
      period = atoi(item.c_str());
      lineStr >> item;
      deadline = atoi(item.c_str());
  
      Task *t = new Task(id,name,wcet,period,deadline);
  
      pTaskset->addTask(t);
  
      getline(infile,line);
    }
  
    infile.close();
}

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
