# Author:  David Rigert
# Created: 2/14/2015
# Assignment 3 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
PROGS = combat
OUTPUTS = 
OBJS = Dice.o Character.o Barbarian.o BlueMen.o Goblin.o ReptilePeople.o TheShadow.o combat.o

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
