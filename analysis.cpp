/*
** analysis.cpp
** 
** Author: Junsung Kim 
**
*/
#include "task.h"

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
    printf("Results of sortTasks()\n");
    for(i=0; i<vTasks.size(); i++) {
        currT = vTasks.at(i)->getPeriod();
        printf("id: %d, wcet: %d, period: %d \n",vTasks.at(i)->getId(), vTasks.at(i)->getWcet(), vTasks.at(i)->getPeriod());
    }
#endif
    
}

void Taskset::sortTasksUtil()
{
    double  prevU, currU;
    Task*   tmpTask;
    unsigned int     i,j;

    // using bubble sort...
    for(i=0; i<vTasks.size(); i++) {
        for(j=(i+1); j<vTasks.size(); j++) {
            prevU = (double)vTasks.at(i)->getWcet()/vTasks.at(i)->getPeriod();
            currU = (double)vTasks.at(j)->getWcet()/vTasks.at(j)->getPeriod();
            if(prevU < currU) {
                tmpTask = vTasks.at(i);
                vTasks.at(i) = vTasks.at(j);
                vTasks.at(j) = tmpTask;
            }
        }
    }
#ifdef DEBUGPRT
    // just print for checking the results
    printf("Results of sortTasksUtil()\n");
    for(i=0; i<vTasks.size(); i++) {
        printf("id: %d, utilization: %f \n",vTasks.at(i)->getId(), (double)vTasks.at(i)->getWcet()/vTasks.at(i)->getPeriod());
    }
#endif
    
}

bool Taskset::addTask(Task* pTask) {
    bool ret = false;
    if(pTask == NULL)
        return ret;

    std::vector<Task*>::iterator result;
    result = std::find(vTasks.begin(), vTasks.end(), pTask);
    if(result != vTasks.end())
        return ret;

    vTasks.push_back(pTask);
    ret = true;
    return ret;
}

bool Taskset::doResponseTimeTest() {

    int numTasks = (int)vTasks.size();
    int i,j;
    int loopCount;
    double responseTime[numTasks];
    double tmpResponseTime, interference;
    bool schedulability = true;

#ifdef DEBUGPRT
    printf("Starting the response time test\n");
#endif
    // calculate response time  
    for(i=0; i<numTasks; i++) {
        tmpResponseTime = responseTime[i] = 0.0f;

        for(j=0; j<=i; j++) {
            responseTime[i] += (double)vTasks.at(j)->getWcet();
        }

        tmpResponseTime = responseTime[i];

        loopCount = 0;
        do {
            responseTime[i] = tmpResponseTime;
            interference = 0;
            for(j=0; j<i; j++) {
                interference+=ceil(tmpResponseTime/vTasks.at(j)->getPeriod())*(double)vTasks.at(j)->getWcet();
            }
            tmpResponseTime = interference + (double)vTasks.at(i)->getWcet();
            loopCount++;
        } while(tmpResponseTime != responseTime[i] && loopCount < 100000);
#ifdef DEBUGPRT
        printf("id: %d, response: %f\n", vTasks.at(i)->getId(), responseTime[i]);
#endif
        if((int)responseTime[i] > vTasks.at(i)->getDeadline()) {
#ifdef DEBUGPRT
            printf("UNSCHEDULABLE!! task: %d, responseTime: %d, deadline: %d\n", vTasks.at(i)->getId(), (int)responseTime[i], vTasks.at(i)->getDeadline());
#endif
            schedulability = false;
            break;
        }
    }

#ifdef DEBUGPRT
    printf("Completing the response time test.. Schedulability: %d\n", schedulability);
#endif

    return schedulability;
}

