main: main.cpp parse.o parse.h exec_job.o exec_job.h process.h history.o history.h install-readline
	g++ -o main main.cpp parse.o exec_job.o history.o -lreadline
	g++ -o malware_handler malware_handler.cpp
parse.o: parse.cpp parse.h process.h
	g++ -c parse.cpp
exec_job.o: exec_job.cpp exec_job.h process.h 
	g++ -c exec_job.cpp
history.o: history.cpp history.h
	g++ -c history.cpp
install-readline:
	@if ! dpkg-query -W libreadline-dev; then \
		sudo apt-get update && sudo apt-get install libreadline-dev; \
	fi
clean:
	rm -f main malware_handler parse.o exec_job.o history.o
