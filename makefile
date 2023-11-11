all: main.o
	g++ -g main.o -o main

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o
	rm main
	rm edge*.raw
	rm gradient*.raw

run: all main
	./main
