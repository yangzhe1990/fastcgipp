CXXFLAGS=-g -std=c++11 -I../include
INCLUDES=-I/home/yangzhe/projects/face++/ScalableFaceSearch/Search
LDLIBS=-lboost_system

all: test_add_remove_list test_echo_server test_movable_function_wrapper

test_add_remove_list: test_add_remove_list.cpp ../include/add_remove_list.hpp ../include/add_remove_list.tcc

test_movable_function_wrapper: test_movable_function_wrapper.cpp ../include/movable_function_wrapper.hpp

test_echo_server: test_echo_server.cpp ../include/protocol.hpp ../src/protocol.o /home/yangzhe/projects/face++/ScalableFaceSearch/Search/socket.o
	$(CXX) $(CXXFLAGS) $(LDLIBS) $(INCLUDES) $^ -o $@

.PHONY: all
