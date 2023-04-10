a.out: library mergesort.o
	g++ mergesort.o -L. -lgoodmalloc

library: goodmalloc.o
	ar rcs libgoodmalloc.a goodmalloc.o

mergesort.o: mergesort.cpp goodmalloc.h
	g++ -c mergesort.cpp

goodmalloc.o: goodmalloc.cpp goodmalloc.h
	g++ -c goodmalloc.cpp

clean:
	rm -rf mergesort.o goodmalloc.o

distclean:
	rm -rf mergesort.o goodmalloc.o libgoodmalloc.a a.out