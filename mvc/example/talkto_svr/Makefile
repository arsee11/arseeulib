CC=g++
CXX=g++
Inc=../../
NetInc=-I../../../
JsonInc=-I/home/arsee_p/lib/jsoncpp-master/include/
Libs=-lmymvc
Libdir=-L../../../lib/
FLAGS= -I$(Inc) --std=c++11 -g
TARGT=talto
OBJS=main.o logics.o member.o
DEBUG=-DDEBUG

$(TARGT): $(OBJS)
	$(CXX) -o $(TARGT) $(OBJS) -static $(Libdir) $(Libs) 

main.o: logics.h member.h controls.h mysession.h $(Inc)server.h 
	$(CC) $(DEBUG) $(NetInc) $(JsonInc) $(FLAGS) -c main.cpp

logics.o:logics.cpp logics.h
	$(CC) $(FLAGS) -c logics.cpp

member.o: member.h $(Inc)mvcobject.h
	$(CC) $(FLAGS) -c member.cpp

clean:
	rm -f $(TARGT) $(OBJS)
