CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := map.cpp transistor.cpp
EXECUTABLE  := a.out


all: $(EXECUTABLE)

run: ./$(EXECUTABLE)

$(EXECUTABLE): *.cpp
	$(CXX) $(CXX_FLAGS) main.cpp $(LIBRARIES)

clean:
	-rm $(EXECUTABLE)