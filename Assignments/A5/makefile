main: main.cpp guest.o cleaner.o defs.o defs.h
	g++ -o main main.cpp guest.o cleaner.o defs.o -lpthread
guest.o: guest.cpp defs.h
	g++ -c guest.cpp
cleaner.o: cleaner.cpp defs.h
	g++ -c cleaner.cpp
defs.o: defs.cpp defs.h
	g++ -c defs.cpp
clean:
	rm -f main guest.o cleaner.o defs.o