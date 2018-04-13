IDIR =./inc
CC=g++-7
CFLAGS=-I$(IDIR) -O3 -march=native #-g
DEFINES= -DDEBUG

SDIR=./src

DEPS =$(shell ls $(IDIR)/*.h) # Brkga.h DummyModel.h TasksToComputersModel.h NursesModel.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(shell ls $(SDIR)/*.cpp) 
OBJ = $(patsubst %.cpp,%.o,$(_OBJ))


%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

all: $(OBJ) 

parallel: CFLAGS += -fopenmp
parallel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DEFINES)

dummy: INSTANCE = dummy
dummy: src/dummy.o
dummy: main

tasksc: INSTANCE = tasks_to_computer
tasksc: src/tasks_to_computer.o
tasksc: main

nurses: INSTANCE = nurses
nurses: CFLAGS += -fopenmp
nurses: src/nurses.o
nurses: main

main: $(OBJ) 
	$(CC) -o $(INSTANCE) src/$(INSTANCE).o $^ $(CFLAGS) $(DEFINES)

generator: src/instance_generator.cpp
	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(SDIR)/*.o
