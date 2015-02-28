# Author:  David Rigert
# Created: 2/27/2015
# Assignment 4 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
PROGS = combat
OUTPUTS = 
OBJS = Barbarian.o BlueMen.o Character.o combat.o Dice.o Goblin.o ReptilePeople.o TheShadow.o Tournament.o utility.o

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
