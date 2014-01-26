/*
** utility.h
** 
** Author: Junsung Kim 
**
*/

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "task.h"
#include "core.h"

void generateTaskset(int numTasks, int minPeriod, int maxPeriod, double targetUtil);
void readTaskset(Taskset* pTaskset);

#endif
