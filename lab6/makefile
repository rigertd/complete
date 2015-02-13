# Author:  David Rigert
# Created: 2/13/2015
# Lab 6 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
PROGS = lab6
OUTPUTS = 
OBJS = lab6.o

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
