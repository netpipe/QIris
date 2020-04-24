test: test.o filter.o sinwavelet.o coswavelet.o gaussian.o sinusoidal.o
	g++ -Wall -g test.o filter.o sinwavelet.o coswavelet.o gaussian.o sinusoidal.o -o test

test.o: test.cpp ../../include/filter.h ../../include/sinwavelet.h ../../include/coswavelet.h ../../include/gaussian.h
	g++ -Wall -g test.cpp -c
    
filter.o: filter.cpp
	g++ -Wall -g filter.cpp -c

sinusoidal.o: sinusoidal.cpp ../../include/filter.h
	g++ -Wall -g sinusoidal.cpp -c

sinwavelet.o: sinwavelet.cpp ../../include/sinwavelet.h ../../include/sinusoidal.h
	g++ -Wall -g sinwavelet.cpp -c
	
coswavelet.o: coswavelet.cpp ../../include/coswavelet.h ../../include/sinusoidal.h
	g++ -Wall -g coswavelet.cpp -c
	
gaussian.o: gaussian.cpp ../../include/gaussian.h
	g++ -Wall -g gaussian.cpp -c


clean:
	rm -rf test *.o *~
