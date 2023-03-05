main: main.o userSimulator.o pushUpdate.o readPost.o defs.o
	g++ -o main main.o userSimulator.o pushUpdate.o readPost.o defs.o -lpthread

main.o: main.cpp defs.h
	g++ -c main.cpp

userSimulator.o: userSimulator.cpp defs.h
	g++ -c userSimulator.cpp

pushUpdate.o: pushUpdate.cpp defs.h
	g++ -c pushUpdate.cpp

readPost.o: readPost.cpp defs.h
	g++ -c readPost.cpp

defs.o: defs.cpp defs.h
	g++ -c defs.cpp

clean:
	rm -rf main main.o userSimulator.o pushUpdate.o readPost.o defs.o