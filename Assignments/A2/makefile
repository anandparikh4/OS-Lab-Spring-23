main: main.cpp parse.o parse.h exec_job.o exec_job.h process.h history.o history.h
	g++ -o main main.cpp parse.o exec_job.o history.o
parse.o: parse.cpp parse.h process.h
	g++ -c parse.cpp
exec_job.o: exec_job.cpp exec_job.h process.h 
	g++ -c exec_job.cpp
history.o: history.cpp history.h
	g++ -c history.cpp
clean:
	rm -f main parse.o exec_job.o
