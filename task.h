/*
** task.h
** 
** Author: Junsung Kim 
**
*/

#ifndef _TASK_H_
#define _TASK_H_

#include <algorithm>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>

class Core;

class Task
{
  public:
    Task(int id, std::string name, int wcet, int period, int deadline) { 
        taskId_ = id;
        taskName_ = name;
        wcet_ = wcet;
        period_ = period;
        deadline_ = deadline;
        pCore_ = NULL;
    }
    ~Task() {};
    inline int getId(void) { return taskId_; }
    inline std::string getName(void) { return taskName_; }
    inline int getWcet(void) { return wcet_; }
    inline int getPeriod(void) { return period_; }
    inline int getDeadline(void) { return deadline_; }
    inline Core* getCore(void) { return pCore_; }

    inline void setCore(Core* c) { pCore_ = c; }

  private:
    int taskId_;
    std::string taskName_;
    int wcet_;
    int period_;
    int deadline_;
    class Core* pCore_;
};

class Taskset
{
  public:
    bool addTask(Task* pTask);
    void sortTasks();
    void sortTasksUtil();
    bool doResponseTimeTest(); 
    inline std::vector<Task*> getTasks(void) { return vTasks; }
    void removeTask(Task *t) {
        if(t == NULL)
            return;

        if(t->getCore() != NULL) {
            t->setCore(NULL);
        }
        vTasks.erase(remove(vTasks.begin(), vTasks.end(), t), vTasks.end());
    }
  private:
    std::vector<Task*> vTasks;
};

#endif
