/*
** analysis.h
** 
** Author: Junsung Kim 
**
*/

#ifndef _TASK_H_
#define _TASK_H_

#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>

class Task
{
  public:
    Task(int id, std::string name, int wcet, int period, int deadline) { 
        taskId_ = id;
        taskName_ = name;
        wcet_ = wcet;
        period_ = period;
        deadline_ = deadline;
    }
    ~Task() {};
    inline int getId(void) { return taskId_; }
    inline std::string getName(void) { return taskName_; }
    inline int getWcet(void) { return wcet_; }
    inline int getPeriod(void) { return period_; }
    inline int getDeadline(void) { return deadline_; }

  private:
    int taskId_;
    std::string taskName_;
    int wcet_;
    int period_;
    int deadline_;
};

class Taskset
{
  public:
    void addTask(Task* pTask);
    void sortTasks();
    bool doResponseTimeTest(); 
  private:
    std::vector<Task*> vTasks;
};

#endif
