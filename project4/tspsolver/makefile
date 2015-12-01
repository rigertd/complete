# Author:  David Rigert
# Created: 11/30/2015
# CS325 Project 4 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x
PROGS = tspsolver tspsolverd
OBJS = tspsolver.o mst_walk.o
DOBJS = tspsolverd.o mst_walkd.o

all: release debug

release: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o tspsolver

debug: $(DOBJS)
	$(CXX) $(CXXFLAGS) $(DOBJS) -o tspsolverd

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

$(DOBJS): $(@:d.o=.cpp)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $(@:d.o=.cpp) -o $@

clean:
	$(RM) -f $(PROGS) $(OBJS) $(DOBJS)

