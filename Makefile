# Makefile for the response-time test implementation
# Author: Junsung Kim
CC=g++
CFLAGS=-Wall

SCHED_SRCS=analysis.cpp main.cpp
SCHED_OBJS=$(SCHED_SRCS:.cpp=.o)

all: schedAnalysis

schedAnalysis: $(SCHED_OBJS)
	$(CC) $(CFLAGS) $(SCHED_OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(SCHED_OBJS) schedAnalysis
