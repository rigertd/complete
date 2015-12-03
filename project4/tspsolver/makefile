# Author:  David Rigert
# Created: 11/30/2015
# CS325 Project 4 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x -pthread
PROGS = tspsolver tspsolver_d
OBJS = tspsolver.o mst_walk.o common.o bruteforce.o
DOBJS = tspsolver_d.o mst_walk_d.o common_d.o bruteforce_d.o

all: release

release: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o tspsolver

debug: $(DOBJS)
	$(CXX) $(CXXFLAGS) $(DOBJS) -o tspsolver_d

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

$(DOBJS): $(@:_d.o=.cpp)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $(@:_d.o=.cpp) -o $@

clean:
	$(RM) $(PROGS) $(OBJS) $(DOBJS)

