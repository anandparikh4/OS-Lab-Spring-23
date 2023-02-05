main: main.cpp parse.o parse.h exec_job.o exec_job.h
	g++ -o main main.cpp parse.o exec_job.o
parse.o: parse.cpp parse.h process.h
	g++ -c parse.cpp
exec_job.o: exec_job.cpp exec_job.h process.h 
	g++ -c exec_job.cpp
process.o: process.cpp process.h
	g++ -c process.cpp
clean:
	rm -f main parse.o process.o exec_job.o
