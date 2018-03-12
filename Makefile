all: program
program: prereq main

prereq: *.cpp
	g++ -c *.cpp

main: *.o
	g++ -o main *.o

clean:
	rm main *.o

