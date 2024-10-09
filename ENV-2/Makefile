all: program 

program: main.o complex.o sort.o
	g++ -o program main.o complex.o sort.o
	
main.o: main.cpp complex.h sort.h
	g++ -c main.cpp -o main.o
	
complex.o: complex.cpp complex.h
	g++ -c complex.cpp -o complex.o
	
sort.o: sort.cpp sort.h
	g++ -c sort.cpp -o sort.o
	
clean:
	rm -f main.o complex.o sort.o program 
	
