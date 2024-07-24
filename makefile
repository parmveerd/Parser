
all:	 hashdemo pish setdemo testParse testScanner

hashdemo:	 hashdemo.o
	g++ -std=c++11 -o hashdemo hashdemo.o

pish:	 pish.o Scanner.o Parse.o
	g++ -std=c++11 -o pish pish.o Scanner.o Parse.o

setdemo:	 setdemo.o
	g++ -std=c++11 -o setdemo setdemo.o

testParse:	 testParse.o Scanner.o Parse.o
	g++ -std=c++11 -o testParse testParse.o Scanner.o Parse.o

testScanner:	 testScanner.o Scanner.o
	g++ -std=c++11 -o testScanner testScanner.o Scanner.o

hashdemo.o:	 hashdemo.cpp hash.h
	g++ -std=c++11 -c hashdemo.cpp

pish.o:	 pish.cpp hash.h Scanner.h Parse.h
	g++ -std=c++11 -c pish.cpp

Scanner.o:	 Scanner.cpp Scanner.h
	g++ -std=c++11 -c Scanner.cpp

Parse.o:	 Parse.cpp Parse.h
	g++ -std=c++11 -c Parse.cpp

setdemo.o:	 setdemo.cpp set.h
	g++ -std=c++11 -c setdemo.cpp

testParse.o:	 testParse.cpp Scanner.h Parse.h
	g++ -std=c++11 -c testParse.cpp

testScanner.o:	 testScanner.cpp Scanner.h
	g++ -std=c++11 -c testScanner.cpp

clean:	
	rm -f core *.o hashdemo pish setdemo testParse testScanner


