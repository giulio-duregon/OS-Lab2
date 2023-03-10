CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -lstdc++ -Wvariadic-macros
BIN=sde_scheduler

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

	
run: all
	./$(BIN) -v -t -e -p -s E10:5 /Users/giulio/OS-Lab2/input1 rfile

clean:
	rm -f *.o
	rm $(BIN)