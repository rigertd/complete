# Author:  David Rigert
# Created: 3/30/2015
# Assignment0 makefile

CXX = gcc
DEBUG = -O0 -g
CXXFLAGS = -Wall $(DEBUG)
PROGS = cm2inch inch2cm

all: $(PROGS)

$(PROGS): $(addsuffix .o,$(PROGS))
	$(CXX) $(CXXFLAGS) $(addsuffix .o,$@) -o $@

$(addsuffix .o,$(PROGS)): $(@:.o=.c)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.c)

clean:
	$(RM) $(PROGS) $(addsuffix .o,$(PROGS))
