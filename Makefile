IDIR =./inc
CC=g++-7
CFLAGS=-I$(IDIR) -O3 -march=native #-g
DEFINES=# -DDEBUG

SDIR=./src

BIN=bin/sim

DEPS =$(shell ls $(IDIR)/*.h) # Brkga.h DummyModel.h TasksToComputersModel.h NursesModel.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(shell ls $(SDIR)/*.cpp) 
OBJ = $(patsubst %.cpp,%.o,$(_OBJ))


%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

all: $(OBJ) main 

main: $(OBJ) 
	$(CC) -o $(BIN) $^ $(CFLAGS) $(DEFINES)

.PHONY: clean

clean:
	rm -f $(SDIR)/*.o $(BIN)
