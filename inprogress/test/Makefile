test : main.o buffered_test.o
	g++ -std=c++11 -o test main.o buffered_test.o

main.o:main.cpp
	g++ -std=c++11 -c main.cpp

buffered_test.o:buffered_test.cpp
	g++ -std=c++11 -c buffered_test.cpp
	
clean:
	rm *.gch *.o
