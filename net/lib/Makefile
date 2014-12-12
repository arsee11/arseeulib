SRC=./
Inc=../
CXX=g++
FLAGS=--std=c++11
Target=../../lib/libmynet.a
OBJS= udpsock.o tcpsock.o

$(Target):udpsock.o tcpsock.o
	ar -rs $(Target) $(OBJS) 

udpsock.o: $(Inc)addr.h $(Inc)udpsock.h 
	$(CXX) -c $(FLAGS) udpsock.cpp

tcpsock.o: $(Inc)addr.h $(Inc)tcpsock.h 
	$(CXX) -c $(FLAGS) tcpsock.cpp


clean:
	rm $(Target) $(OBJS)
