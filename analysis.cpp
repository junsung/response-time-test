/*
** analysis.cpp
** 
** Author: Junsung Kim 
**
*/
#include "task.h"

Taskset Taskset::scaleTaskSet()
{
    Taskset scaledTaskset;
    sortTasks();
    unsigned int numTasks = vTasks.size();
    unsigned int i;
    int period, wcet, id;
    std::string name;
    Task* tmpTask;
    for(i=0; i<numTasks; i++) {
        period = vTasks.at(i)->getPeriod();
        wcet = vTasks.at(i)->getWcet();
        id = vTasks.at(i)->getId();
        name = vTasks.at(i)->getName();
        tmpTask = new Task(id, name, wcet, period, period);
        scaledTaskset.addTask(tmpTask);
    }
    while((double)scaledTaskset.getTasks().at(0)->getPeriod() 
       < (double)scaledTaskset.getTasks().at(numTasks-1)->getPeriod()/2.0) 
    {
        for(i=0; i<numTasks-1; i++) {
            if(2*scaledTaskset.getTasks().at(i)->getPeriod() < scaledTaskset.getTasks().at(numTasks-1)->getPeriod()) {
                scaledTaskset.getTasks().at(i)->setPeriod(2*scaledTaskset.getTasks().at(i)->getPeriod());
                scaledTaskset.getTasks().at(i)->setDeadline(2*scaledTaskset.getTasks().at(i)->getDeadline());
                scaledTaskset.getTasks().at(i)->setWcet(2*scaledTaskset.getTasks().at(i)->getWcet());
            }
        }
        scaledTaskset.sortTasks();
    }

#ifdef DEBUGPRT
    // just print for checking the results
    printf("Results of scaleTaskSet()\n");
    for(i=0; i<scaledTaskset.getTasks().size(); i++) {
        printf("id: %d, wcet: %d, period: %d \n",scaledTaskset.getTasks().at(i)->getId(), scaledTaskset.getTasks().at(i)->getWcet(), scaledTaskset.getTasks().at(i)->getPeriod());
    }
#endif
    return scaledTaskset;
}

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

bool Taskset::doRBoundTest() 
{
    bool schedulability = true;
    double numTasks = (double)vTasks.size();
    double ratio = (double)vTasks.at(numTasks-1)->getPeriod()/vTasks.at(0)->getPeriod();
    double rBound = (numTasks-1.0)*(pow(ratio,(1.0/(numTasks-1.0)))-1.0)+2.0/ratio-1.0;
    double totalUtil = 0.0;
    for(unsigned int i=0; i<numTasks; i++)
    {
        totalUtil += (double)vTasks.at(i)->getWcet()/vTasks.at(i)->getPeriod();
    }
#ifdef DEBUGPRT
    printf("Starting the r-bound test: r-bound: %f total utilization: %f schedulability: %d ratio: %f\n", rBound, totalUtil, rBound >= totalUtil, ratio);
#endif
    if(totalUtil > rBound)
        schedulability = false;
    return schedulability;
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

