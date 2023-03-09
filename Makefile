CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := map.cpp transistor.cpp
EXECUTABLE  := s2cae


all: $(EXECUTABLE)

run: ./$(EXECUTABLE)

$(EXECUTABLE): *.cpp
	$(CXX) $(CXX_FLAGS) main.cpp $(LIBRARIES) -o $(EXECUTABLE)

clean:
	-rm $(EXECUTABLE)