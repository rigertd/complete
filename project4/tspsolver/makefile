# Author:  David Rigert
# Created: 11/30/2015
# CS325 Project 4 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x
PROGS = tspsolver tspsolver_d optimized
OBJS = tspsolver.o mst_walk.o aco.o common.o
DOBJS = tspsolver_d.o mst_walk_d.o aco_d.o common_d.o

all: release

optimized:
	$(CXX) $(CXXFLAGS) optimized.cpp -o optimized

release: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o tspsolver

debug: $(DOBJS)
	$(CXX) $(CXXFLAGS) $(DOBJS) -o tspsolver_d

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

$(DOBJS): $(@:_d.o=.cpp)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $(@:_d.o=.cpp) -o $@

clean:
	$(RM) -f $(PROGS) $(OBJS) $(DOBJS)

