/*
** core.h
** 
** Author: Junsung Kim 
**
*/

#ifndef _CORE_H_
#define _CORE_H_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include "task.h"

class Core
{
  public:
    Core(int id, std::string name, double max_util) {
      coreId_ = id;
      maxUtil_ = max_util;
      coreName_ = name;
      curUtil_ = 0.0;
    }
    ~Core() {};

    inline int getId(void) { return coreId_; }
    inline std::string getName(void) { return coreName_; }
    inline double getMaxUtil(void) { return maxUtil_; }
    inline double getCurUtil(void) { return curUtil_; }
    inline int isSchedulable(void) { return tasks_.doResponseTimeTest(); }
    inline Taskset* getTasks(void) { return &tasks_; }

    inline int setCurUtil(double v) { if(v > maxUtil_) return -1; curUtil_ = v; return 0; }

    void bindTask(Task* pTask) {
        if(pTask != NULL) {
            if(tasks_.addTask(pTask)) 
                pTask->setCore(this);
        }
    }

  private:
    int coreId_;
    std::string coreName_;
    double maxUtil_;
    double curUtil_;
    Taskset tasks_;
};

class Coreset
{
  public:
    Coreset(int numCores, bool expandible) {
        vCores.clear();
        setNumCores(numCores);
        expandible_=expandible;
    }
    ~Coreset() {};

    inline int getNumCores(void) { return (int)vCores.size(); }
    inline int getNumActiveCores(void) {
        int numActiveCores = 0;
        for(int i=0; i<(int)vCores.size(); i++) {
            if(vCores.at(i)->getCurUtil() > 0)
                numActiveCores++;
        }
        return numActiveCores;
    }
    inline std::vector<Core*> getCores(void) { return vCores; }
    inline void setExpandible(bool v) { expandible_ = v; }
    void setNumCores(int v) { 
        int curNumCores = (int)vCores.size();
        if(v > curNumCores) {
            for(int i=0; i<v - curNumCores; i++) {
                int id = curNumCores+1+i;
                std::string name = "Core"+id;
                Core* c = new Core(id, name, 1.0);
                vCores.push_back(c);
            }
        }
        numCores_ = v; 
    }

    void sortBinsAscending();
    void sortBinsDescending();

    bool doFFD(Taskset* pTasks);
    bool doBFD(Taskset* pTasks);
    bool doWFD(Taskset* pTasks);

  private:
    int numCores_;
    bool expandible_;
    std::vector<Core*> vCores;
};

#endif
