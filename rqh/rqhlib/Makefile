SRC=./
Inc=../
JSONInc=-I./
JSONLib=json_reader.cpp.o json_value.cpp.o json_writer.cpp.o
CXX=g++
FLAGS=-g -std=c++11 
OBJ=../../lib/librqh.a

$(OBJ): jpack2.o 
	ar rs $(OBJ) jpack2.o $(JSONLib)

jpack2.o: jpack2.cpp $(Inc)pack_2.h $(Inc)../namespdef.h
	g++ $(FLAGS) $(JSONInc) -I$(Inc) -c $(SRC)jpack2.cpp

clean:
	rm -f jpack2.o $(OBJ)
