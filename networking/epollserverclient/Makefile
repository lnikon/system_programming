CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Werror -O0 -fno-elide-constructors -pedantic-errors -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations 

all: client server 

client: client.cpp utility.o
	$(CXX) $(CXXFLAGS) -o client client.cpp utility.o

server: server.cpp utility.o
	$(CXX) $(CXXFLAGS) -o server server.cpp utility.o

utility: utility.cpp
	$(CXX) $(CXXFLAGS) -o utility.o utility.cpp

epoll_test: epoll_test.cpp
	$(CXX) $(CXXFLAGS) epoll_test epoll_test.cpp

clean:
	rm client server	
