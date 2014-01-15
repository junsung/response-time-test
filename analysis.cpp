/*
** analysis.cpp
** 
** Author: Junsung Kim 
**
*/
#include "analysis.h"

void Taskset::sortTasks()
{
    int  prevT, currT;
    Task*   tmpTask;
    unsigned int     i,j;

    // using bubble sort...
    for(i=0; i<vTasks.size(); i++) {
        for(j=(i+1); j<vTasks.size(); j++) {
            prevT = vTasks.at(i)->getPeriod();
            currT = vTasks.at(j)->getPeriod();
            if(prevT > currT) {
                tmpTask = vTasks.at(i);
                vTasks.at(i) = vTasks.at(j);
                vTasks.at(j) = tmpTask;
            }
        }
    }
#ifdef DEBUGPRT
    // just print for checking the results
    for(i=0; i<vTasks.size(); i++) {
        currT = vTasks.at(i)->getPeriod();
        printf("id: %d, wcet: %d, period: %d \r\n",vTasks.at(i)->getId(), vTasks.at(i)->getWcet(), currT);
    }
#endif
    
}

void Taskset::addTask(Task* pTask) {
    vTasks.push_back(pTask);
}

bool Taskset::doResponseTimeTest() {

    int numTasks = (int)vTasks.size();
    int i,j;
    double responseTime[numTasks];
    double tmpResponseTime, interference;
    bool schedulability = true;

    // calculate response time  
    for(i=0; i<numTasks; i++) {
        tmpResponseTime = responseTime[i] = 0.0f;

        responseTime[i] = (double)vTasks.at(i)->getWcet();

        tmpResponseTime = responseTime[i];
        do {
            responseTime[i] = tmpResponseTime;
            interference = 0;
            for(j=0; j<i; j++) {
                interference+=ceil(tmpResponseTime/vTasks.at(j)->getPeriod())*(double)vTasks.at(j)->getWcet();
            }
            tmpResponseTime = interference + (double)vTasks.at(i)->getWcet();
        } while(tmpResponseTime != responseTime[i]);
#ifdef DEBUGPRT
        printf("id: %d, response: %f\r\n", vTasks.at(i)->getId(), responseTime[i]);
        if((int)responseTime[i] > vTasks.at(i)->getDeadline()) {
            printf("UNSCHEDULABLE!! task: %d, responseTime: %d, deadline: %d\r\n", vTasks.at(i)->getId(), (int)responseTime[i], vTasks.at(i)->getDeadline());
            schedulability = false;
        }
#endif
    }

    return schedulability;
}

