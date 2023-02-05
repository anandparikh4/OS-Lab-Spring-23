main: main.cpp parse.o parse.h exec_job.o exec_job.h process.h install-readline
	g++ -o main main.cpp parse.o exec_job.o -lreadline
parse.o: parse.cpp parse.h process.h
	g++ -c parse.cpp
exec_job.o: exec_job.cpp exec_job.h process.h 
	g++ -c exec_job.cpp
install-readline:
	@if ! dpkg-query -W libreadline-dev; then \
		sudo apt-get update && sudo apt-get install libreadline-dev; \
	fi
clean:
	rm -f main parse.o exec_job.o
