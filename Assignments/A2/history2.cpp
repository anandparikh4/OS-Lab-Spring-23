#include "history2.h"
#include <readline/readline.h>

shell_history::shell_history(){
	strcat(strcpy(history_file, getenv("HOME")), "/.myshell_history");

	FILE *fp = fopen(history_file,"r");
	if(!fp){
		history_cnt=0;
		history_idx=0;
		dq.clear();
	}
	else{
		char buff[1024];
		dq.clear();
		history_cnt=0;
		while(fgets(buff,sizeof(buff),fp)){
			buff[strlen(buff)-1] = '\0';
			dq.push_back(strdup(buff));
			history_cnt++;
			if(history_cnt==1000)break;
		}
		history_idx = history_cnt;
		fclose(fp);
	}
}

shell_history::~shell_history(){
	for(int i=0;i<history_cnt;i++){
		if(dq[i]!=NULL && strlen(dq[i])>0 && dq[i][0]!=EOF)free(dq[i]);
	}
}

void shell_history::manage_history(){
	if(line==NULL || strlen(line)==0)return;
	if(history_cnt==1000){
		dq.pop_front();
		history_cnt--;
	}
	dq.push_back(line);
	history_cnt++;
	history_idx = history_cnt;
	FILE* fp = fopen(history_file,"a+");
	if(fp==NULL){
		perror("Unable to write to history");
		exit(0);
	}
	fprintf(fp,"%s\n",line);
	fflush(fp);
	fclose(fp);
	return;
}

void shell_history :: get_history(){
	char prompt[1024];
    if(getcwd(prompt , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
	strcat(prompt,"$ ");
	line = readline(prompt);
	return;
}
