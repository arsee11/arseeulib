SRC=./
Inc=../
JSONInc=-I/home/arsee_p/lib/jsoncpp-master/include/
JSONLib=json_reader.cpp.o json_value.cpp.o json_writer.cpp.o
CXX=g++
FLAGS=--std=c++11
OBJ=../../lib/libmymvc.a

$(OBJ): jpack.o udpsock.o
	ar rs $(OBJ) jpack.o udpsock.o $(JSONLib)

jpack.o: $(Inc)jpack.h $(Inc)../namespdef.h
	g++ $(FLAGS) $(JSONInc) -I$(Inc) -c $(SRC)jpack.cpp

clean:
	rm -f jpack.o udpsock.o
