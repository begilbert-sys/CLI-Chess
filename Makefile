CXX = g++
CXXFLAGS = -std=c++20
SRC = $(wildcard chess/*.cpp) slowprint.cpp client.cpp local.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

runchess: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

runserver: server.o
	$(CXX) $(CXXFLAGS) -o runserver server.o
server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp

clean:
	find . -name "*.o" -type f -delete
	rm runserver runchess