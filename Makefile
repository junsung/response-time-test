# Makefile for the response-time test implementation
# Author: Junsung Kim
CC=g++
CFLAGS=-Wall

SCHED_SRCS=analysis.cpp utility.cpp main.cpp
SCHED_OBJS=$(SCHED_SRCS:.cpp=.o)

ALLOC_SRCS=allocation.cpp utility.cpp analysis.cpp
ALLOC_OBJS=$(ALLOC_SRCS:.cpp=.o)

all: schedAnalysis coreAllocation

schedAnalysis: $(SCHED_OBJS)
	$(CC) $(CFLAGS) $(SCHED_OBJS) -o $@

coreAllocation: $(ALLOC_OBJS)
	$(CC) $(CFLASG) $(ALLOC_OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(SCHED_OBJS) $(ALLOC_OBJS) schedAnalysis coreAllocation
