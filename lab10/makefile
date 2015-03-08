# Author:  David Rigert
# Created: 3/7/2015
# Lab 10 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -foptimize-sibling-calls -std=c++0x $(DEBUG)
PROGS = lab10
OUTPUTS = 
OBJS = lab10.o

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
