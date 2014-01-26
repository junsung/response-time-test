/*
** utility.cpp
** 
** Author: Junsung Kim 
**
*/

#include "utility.h"

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

