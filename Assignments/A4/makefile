main: main.cpp userSimulator.o pushUpdate.o readPost.o defs.o defs.h
	g++ -o main main.cpp userSimulator.o pushUpdate.o readPost.o defs.o -lpthread

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