CXX = g++
CXXFLAGS = -std=c++20
SRC = $(wildcard chess/*.cpp) slowprint.cpp client.cpp local.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

