CXX = g++
CXXFLAGS = -Wall

driver: swarm.o  swarm.cpp
	$(CXX) $(CXXFLAGS) swarm.o driver.cpp -o driver

mytest: swarm.o  swarm.cpp
	$(CXX) $(CXXFLAGS) swarm.o mytest.cpp -o mytest

swarm.o: swarm.cpp swarm.h
	$(CXX) $(CXXFLAGS) -c swarm.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./driver

runTest:
	./mytest
