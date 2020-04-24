test2: test2.o iriscode.o
	g++ -Wall -g test2.o iriscode.o -o test2

iriscode.o: iriscode.cpp ../../include/iriscode.h
	g++ -Wall -g iriscode.cpp -c

test2.o: test2.cpp ../../include/iriscode.h
	g++ -Wall -g test2.cpp -c

clean:
	rm -f test2 *.o *~
