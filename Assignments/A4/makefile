main: main.o userSimulator.o defs.o
	g++ -o main main.o userSimulator.o defs.o -lpthread

main.o: main.cpp defs.h
	g++ -c main.cpp

userSimulator.o: userSimulator.cpp defs.h
	g++ -c userSimulator.cpp

defs.o: defs.cpp defs.h
	g++ -c defs.cpp

clean:
	rm -rf main main.o userSimulator.o defs.o