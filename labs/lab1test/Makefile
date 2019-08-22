CXX := g++
CXXFLAGS := -std=c++17 -O0 -g

.PHONY: all zip clean clean-all

all: BellmanFord

BellmanFord: BellmanFord.o PathFinder.o Graph.o 
	$(CXX) $(CXXFLAGS) $^ -o $@

BellmanFord.o: BellmanFord.cpp PathFinder.h Graph.h BellmanFord.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

PathFinder.o: PathFinder.cpp PathFinder.h Graph.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Graph.o: Graph.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) BellmanFord *.o

clean-all: clean
	$(RM) report.pdf William_Paape_lab1.zip

