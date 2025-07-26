#include "history.h"
#include <readline/readline.h>

shell_history::shell_history(){
	strcat(strcpy(history_file, getenv("HOME")), "/.myshell_history");
	latest_command[0] = '\0';
	FILE *fp = fopen(history_file,"r");
	if(!fp){
		fclose(fopen(history_file,"w"));
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
		}
		fclose(fp);
		int fl=0;
		if(dq.size()>MAX_COMMANDS)	fl=1;
		while(dq.size()>MAX_COMMANDS){
			if(dq[0]!=NULL && dq[0][0]!=EOF)free(dq[0]);
			dq.pop_front();
			history_cnt--;
		}
		history_idx = history_cnt;
	}
}

shell_history::~shell_history(){
	FILE *fp = fopen(history_file,"w");
	for(auto &it:dq){
		if(it!=NULL && strlen(it)>0 && it[0]!=EOF){
			fprintf(fp,"%s\n",it);
		}
	}
	fclose(fp);
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
	if(dq.size()>0 && strcmp(dq[dq.size()-1],line)==0){
		history_idx = history_cnt;
		return;
	}
	dq.push_back(line);
	history_cnt++;
	history_idx = history_cnt;

	return;
}

void shell_history :: get_history(){
	char prompt[1024]="",temp[1024];
    if(getcwd(temp , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
	strcat(prompt,temp);

	strcat(prompt,"$ ");
	line = readline(prompt);
	return;
}
