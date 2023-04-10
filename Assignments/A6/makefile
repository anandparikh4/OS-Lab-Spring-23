mergesort: libgoodmalloc.a mergesort.cpp goodmalloc.h
	g++ -o mergesort mergesort.cpp -L. -lgoodmalloc

libgoodmalloc.a: goodmalloc.o
	ar rcs libgoodmalloc.a goodmalloc.o

goodmalloc.o: goodmalloc.cpp goodmalloc.h
	g++ -c goodmalloc.cpp

clean:
	rm -f goodmalloc.o mergesort

distclean:
	rm -f goodmalloc.o libgoodmalloc.a mergesort