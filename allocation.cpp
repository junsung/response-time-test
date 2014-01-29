/*
** allocation.cpp
** 
** Author: Junsung Kim 
**
*/
#include <iostream>
#include "core.h"
#include "task.h"
#include "utility.h"

void Coreset::sortBinsAscending() 
{
    double  prevU, currU;
    Core*   tmpCore;
    unsigned int     i,j;

    // using bubble sort...
    for(i=0; i<vCores.size(); i++) {
        for(j=(i+1); j<vCores.size(); j++) {
            prevU = (double)vCores.at(i)->getCurUtil();
            currU = (double)vCores.at(j)->getCurUtil();
            if(prevU < currU) {
                tmpCore = vCores.at(i);
                vCores.at(i) = vCores.at(j);
                vCores.at(j) = tmpCore;
            }
        }
    }
#ifdef DEBUGPRT
    // just print for checking the results
    printf("Results of sortBinsAscending()\n");
    for(i=0; i<vCores.size(); i++) {
        printf("id: %d, utilization: %f \n",vCores.at(i)->getId(), (double)vCores.at(i)->getCurUtil());
    }
#endif

}

void Coreset::sortBinsDescending() 
{
    double  prevU, currU;
    Core*   tmpCore;
    unsigned int     i,j;

    // using bubble sort...
    for(i=0; i<vCores.size(); i++) {
        for(j=(i+1); j<vCores.size(); j++) {
            prevU = (double)vCores.at(i)->getCurUtil();
            currU = (double)vCores.at(j)->getCurUtil();
            if(prevU > currU) {
                tmpCore = vCores.at(i);
                vCores.at(i) = vCores.at(j);
                vCores.at(j) = tmpCore;
            }
        }
    }
#ifdef DEBUGPRT
    // just print for checking the results
    printf("Results of sortBinsDescending()\n");
    for(i=0; i<vCores.size(); i++) {
        printf("id: %d, utilization: %f \n",vCores.at(i)->getId(), (double)vCores.at(i)->getCurUtil());
    }
#endif

}

bool Coreset::doFFD(Taskset* pTasks) {
    pTasks->sortTasksUtil();

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        unsigned int j=0;
        for(j=0; j<vCores.size(); j++) {
#ifdef DEBUGPRT
            printf("Task %d is considered on Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
            vCores.at(j)->bindTask(pTasks->getTasks().at(i));
            vCores.at(j)->getTasks()->sortTasks();
            if(vCores.at(j)->getTasks()->doResponseTimeTest() == false) {
                vCores.at(j)->getTasks()->removeTask(pTasks->getTasks().at(i));
            }
            else {
#ifdef DEBUGPRT
                printf("Task %d is allocated to Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
                double newUtil = vCores.at(j)->getCurUtil() + 
                                 (double)pTasks->getTasks().at(i)->getWcet()/pTasks->getTasks().at(i)->getPeriod();
                vCores.at(j)->setCurUtil(newUtil);
                break;
            }
        }
        if(j==vCores.size()) {
            if(expandible_) {
#ifdef DEBUGPRT
                printf("One more core is added to allocate the remaining tasks.\n");
#endif
                setNumCores(getNumCores()+1);
                i--;
            }
            else {
#ifdef DEBUGPRT
                printf("We cannot add more cores, so stop. The current number of cores: %d\n", getNumCores());
#endif
                return false;
            }
        }
    }

    return true;
}

bool Coreset::doBFD(Taskset* pTasks) {
    pTasks->sortTasksUtil();

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        unsigned int j=0;
        sortBinsAscending();
        for(j=0; j<vCores.size(); j++) {
#ifdef DEBUGPRT
            printf("Task %d is considered on Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
            vCores.at(j)->bindTask(pTasks->getTasks().at(i));
            vCores.at(j)->getTasks()->sortTasks();
            if(vCores.at(j)->getTasks()->doResponseTimeTest() == false) {
                vCores.at(j)->getTasks()->removeTask(pTasks->getTasks().at(i));
            }
            else {
#ifdef DEBUGPRT
                printf("Task %d is allocated to Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
                double newUtil = vCores.at(j)->getCurUtil() +
                                 (double)pTasks->getTasks().at(i)->getWcet()/pTasks->getTasks().at(i)->getPeriod();
                vCores.at(j)->setCurUtil(newUtil);
                break;
            }
        }
        if(j==vCores.size()) {
            if(expandible_) {
#ifdef DEBUGPRT
                printf("One more core is added to allocate the remaining tasks.\n");
#endif
                setNumCores(getNumCores()+1);
                i--;
            }
            else {
#ifdef DEBUGPRT
                printf("We cannot add more cores, so stop. The current number of cores: %d\n", getNumCores());
#endif
                return false;
            }
        }
    }
    return true;
}

bool Coreset::doWFD(Taskset* pTasks) {
    pTasks->sortTasksUtil();

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        unsigned int j=0;
        sortBinsDescending();
        for(j=0; j<vCores.size(); j++) {
#ifdef DEBUGPRT
            printf("Task %d is considered on Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
            vCores.at(j)->bindTask(pTasks->getTasks().at(i));
            vCores.at(j)->getTasks()->sortTasks();
            if(vCores.at(j)->getTasks()->doResponseTimeTest() == false) {
                vCores.at(j)->getTasks()->removeTask(pTasks->getTasks().at(i));
            }
            else {
#ifdef DEBUGPRT
                printf("Task %d is allocated to Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
                double newUtil = vCores.at(j)->getCurUtil() +
                                 (double)pTasks->getTasks().at(i)->getWcet()/pTasks->getTasks().at(i)->getPeriod();
                vCores.at(j)->setCurUtil(newUtil);
                break;
            }
        }
        if(j==vCores.size()) {
            if(expandible_) {
#ifdef DEBUGPRT
                printf("One more core is added to allocate the remaining tasks.\n");
#endif
                setNumCores(getNumCores()+1);
                i--;
            }
            else {
#ifdef DEBUGPRT
                printf("We cannot add more cores, so stop. The current number of cores: %d\n", getNumCores());
#endif
                return false;
            }
        }
    }
    return true;

}

bool Coreset::doRBoundFFD(Taskset* pTasks) {
    Taskset scaledTaskSet;

    scaledTaskSet = pTasks->scaleTaskSet();
    pTasks = &scaledTaskSet;

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        unsigned int j=0;
        for(j=0; j<vCores.size(); j++) {
#ifdef DEBUGPRT
            printf("Task %d is considered on Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
            vCores.at(j)->bindTask(pTasks->getTasks().at(i));
            vCores.at(j)->getTasks()->sortTasks();
            if(vCores.at(j)->getTasks()->doRBoundTest() == false) {
                vCores.at(j)->getTasks()->removeTask(pTasks->getTasks().at(i));
            }
            else {
#ifdef DEBUGPRT
                printf("Task %d is allocated to Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
                double newUtil = vCores.at(j)->getCurUtil() + 
                                 (double)pTasks->getTasks().at(i)->getWcet()/pTasks->getTasks().at(i)->getPeriod();
                vCores.at(j)->setCurUtil(newUtil);
                break;
            }
        }
        if(j==vCores.size()) {
            if(expandible_) {
#ifdef DEBUGPRT
                printf("One more core is added to allocate the remaining tasks.\n");
#endif
                setNumCores(getNumCores()+1);
                i--;
            }
            else {
#ifdef DEBUGPRT
                printf("We cannot add more cores, so stop. The current number of cores: %d\n", getNumCores());
#endif
                for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
                    delete pTasks->getTasks().at(i);
                }
                return false;
            }
        }
    }

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        delete pTasks->getTasks().at(i);
    }

    return true;
}

bool Coreset::doRBoundNFD(Taskset* pTasks) {
    Taskset scaledTaskSet;

    scaledTaskSet = pTasks->scaleTaskSet();
    pTasks = &scaledTaskSet;

    unsigned int j=0;
    unsigned int lastCore=0;
    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        for(j=lastCore; j<vCores.size(); j++) {
#ifdef DEBUGPRT
            printf("Task %d is considered on Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
            vCores.at(j)->bindTask(pTasks->getTasks().at(i));
            vCores.at(j)->getTasks()->sortTasks();
            if(vCores.at(j)->getTasks()->doRBoundTest() == false) {
                vCores.at(j)->getTasks()->removeTask(pTasks->getTasks().at(i));
            }
            else {
#ifdef DEBUGPRT
                printf("Task %d is allocated to Core %d.\n",pTasks->getTasks().at(i)->getId(),vCores.at(j)->getId());
#endif
                double newUtil = vCores.at(j)->getCurUtil() + 
                                 (double)pTasks->getTasks().at(i)->getWcet()/pTasks->getTasks().at(i)->getPeriod();
                vCores.at(j)->setCurUtil(newUtil);
                lastCore = j;
                break;
            }
        }
        if(j==vCores.size()) {
            if(expandible_) {
#ifdef DEBUGPRT
                printf("One more core is added to allocate the remaining tasks.\n");
#endif
                setNumCores(getNumCores()+1);
                lastCore = j;
                i--;
            }
            else {
#ifdef DEBUGPRT
                printf("We cannot add more cores, so stop. The current number of cores: %d\n", getNumCores());
#endif
                for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
                    delete pTasks->getTasks().at(i);
                }
                return false;
            }
        }
    }

    for(unsigned int i=0; i<pTasks->getTasks().size(); i++) {
        delete pTasks->getTasks().at(i);
    }

    return true;
}

int main() {
    int numFFD, numBFD, numWFD;
    int numTests = 0;
    numFFD = numBFD = numWFD = 0;
    do {
        numTests++;
        generateTaskset(100, 10, 1000, 4.0);
        Taskset taskSet;
        readTaskset(&taskSet);
        taskSet.sortTasksUtil();
/*
    Coreset coreSet = Coreset(1,true);
    if(coreSet.doBFD(&taskSet)) {
        printf("allocated!! The number of cores: %d\n", coreSet.getNumActiveCores());
    }
    else {
        printf("failed :'(\n");
    }
*/
//        Coreset coreSetFFD = Coreset(1,true);
        Coreset coreSetBFD = Coreset(1,true);
        Coreset coreSetWFD = Coreset(1,true);
//        coreSetFFD.doFFD(&taskSet);
        coreSetBFD.doRBoundFFD(&taskSet);
        coreSetWFD.doRBoundNFD(&taskSet);
//        numFFD = coreSetFFD.getNumActiveCores();
        numBFD = coreSetBFD.getNumActiveCores();
        numWFD = coreSetWFD.getNumActiveCores();
        for(unsigned int i=0; i<taskSet.getTasks().size(); i++) {
            delete taskSet.getTasks().at(i);
        }
        for(unsigned int i=0; i<coreSetBFD.getCores().size(); i++) {
            delete coreSetBFD.getCores().at(i);
        }
        for(unsigned int i=0; i<coreSetWFD.getCores().size(); i++) {
            delete coreSetWFD.getCores().at(i);
        }
    } while (numBFD <= numWFD && numTests < 1000000);
    std::cout << "Num Cores: " << numFFD;
    std::cout << " " << numBFD;
    std::cout << " " << numWFD << std::endl;
    std::cout << "Num Tests: " << numTests << std::endl;
    return 0;
}
