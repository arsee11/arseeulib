#!/bin/bash


 g++ -g -fno-inline -I/home/arsee_p/lib/jsoncpp-master/include/ -L/home/arsee_p/lib/jsoncpp-master/lib/\
     -std=c++11 jpack_2_test.cpp ../rqhlib/jpack2.cpp -ljsoncpp 
