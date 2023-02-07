#include <iostream>
#include <unistd.h>
#include <cstring>
using namespace std;

int main(){
	char *args[2];
	args[0] = strdup("./malware");
	args[1] = NULL;
	int cpid = fork();	
	if(cpid==0){
		execvp(args[0],args);
		perror("execvp");
		exit(0);
	}
	else{
		sleep(120);
		int cpid = fork();	
		if(cpid==0){
			execvp(args[0],args);
			perror("execvp");
			exit(0);
		}
		sleep(100000);	
	}
}