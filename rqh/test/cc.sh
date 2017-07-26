#!/bin/bash


 g++ -g -fno-inline -I../rghlib \
     -std=c++11 jpack_2_test.cpp ../rqhlib/jpack2.cpp ../rqhlib/json_value.cpp.o ../rqhlib/json_reader.cpp.o \
     ../rqhlib/json_writer.cpp.o 
