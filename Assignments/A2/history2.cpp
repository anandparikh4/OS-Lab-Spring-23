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
		char buff[4096];
		dq.clear();
		history_cnt=0;
		while(fgets(buff,sizeof(buff),fp)){
			buff[strlen(buff)-1] = '\0';
			dq.push_back(strdup(buff));
			history_cnt++;
			// if(history_cnt==MAX_COMMANDS)break;
		}
		int fl=0;
		if(dq.size()>MAX_COMMANDS)	fl=1;
		while(dq.size()>MAX_COMMANDS){
			if(dq[0]!=NULL && dq[0][0]!=EOF)free(dq[0]);
			dq.pop_front();
			history_cnt--;
		}
		history_idx = history_cnt;
		fclose(fp);
		if(fl){
			fp = fopen(history_file,"w");
			if(fp){
				for(int i=0;i<history_cnt;i++){
					fprintf(fp,"%s\n",dq[i]);
				}
				fflush(fp);
				fclose(fp);
			}
		}
	}
}

shell_history::~shell_history(){
	for(int i=0;i<history_cnt;i++){
		if(dq[i]!=NULL && strlen(dq[i])>0 && dq[i][0]!=EOF)free(dq[i]);
	}
}

void shell_history::manage_history(){
	if(line==NULL || strlen(line)==0)return;
	if(history_cnt==MAX_COMMANDS){
		if(dq[0]!=NULL && dq[0][0]!=EOF)free(dq[0]);
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
	char prompt[1024]="\033[34m",temp[1024];
    if(getcwd(temp , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
	strcat(prompt,temp);
	strcat(prompt,"\033[0m");
	strcat(prompt,"$ ");
	line = readline(prompt);
	return;
}
