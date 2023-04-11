mergesort: libgoodmalloc.a mergesort.o
	g++ -o mergesort mergesort.o -L. -lgoodmalloc

libgoodmalloc.a: goodmalloc.o
	ar rcs libgoodmalloc.a goodmalloc.o

mergesort.o: mergesort.cpp goodmalloc.h
	g++ -c mergesort.cpp

goodmalloc.o: goodmalloc.cpp goodmalloc.h
	g++ -c goodmalloc.cpp

clean:
	rm -f goodmalloc.o mergesort.o

distclean:
	rm -f goodmalloc.o mergesort.o libgoodmalloc.a mergesort