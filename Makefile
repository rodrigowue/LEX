CXX       := g++
CXX_FLAGS := -std=c++2a -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := stlGraph.cpp transistor.cpp
EXECUTABLE  := a.out


all: $(EXECUTABLE)

run: ./$(EXECUTABLE)

$(EXECUTABLE): *.cpp
	$(CXX) $(CXX_FLAGS) main.cpp $(LIBRARIES)

clean:
	-rm $(EXECUTABLE)