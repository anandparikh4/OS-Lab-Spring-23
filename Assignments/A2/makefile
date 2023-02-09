main: main2.cpp parse.o parse.h exec_job.o exec_job.h process.h history2.o history2.h
	g++ -o main2 main2.cpp parse.o exec_job.o history2.o -lreadline
	g++ -o malware_handler malware_handler.cpp
parse.o: parse.cpp parse.h process.h
	g++ -c parse.cpp
exec_job.o: exec_job.cpp exec_job.h process.h 
	g++ -c exec_job.cpp
history2.o: history2.cpp history2.h
	g++ -c history2.cpp
clean:
	rm -f main2 parse.o exec_job.o history2.o
