#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <set>
#include <sys/signal.h>
#include <iostream>
#include <readline/readline.h>
#include "parse.h" 
#include "exec_job.h"
#include "history2.h"

using namespace std;
#define MAX_COMMANDS 1000

int foreground_pgid;
set <int> fg_procs,bg_run_procs,bg_stop_procs;
shell_history history;

int up_function(int s1,int s2){
	if(history.history_idx==0){
		return 0;
	}
	history.history_idx--;
	rl_replace_line(history.dq[history.history_idx],0);
	rl_redisplay();
	rl_end_of_line(s1,s2);
    return 0;
}

int down_function(int s1,int s2){
	if(history.history_idx == (history.history_cnt)){
		return 0;
	}	
	else if(history.history_idx == (history.history_cnt)-1){
		rl_replace_line("",0); //check
		rl_redisplay();
		rl_end_of_line(s1,s2);
		history.history_idx++;
	}
	else{
		rl_replace_line(history.dq[history.history_idx+1],0);
		rl_redisplay();
        rl_end_of_line(s1,s2);
		history.history_idx++;
	}
    return 0;
}

void sigint_handler(int signum){
    signal(SIGINT,sigint_handler);
    printf("\n");
    char * cwd = (char *) malloc(1024 * sizeof(char));
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    printf("%s" , cwd);
    printf("$ ");
    fflush(stdout);
    return;
}

void sigtstp_handler(int signum){
    signal(SIGTSTP,sigint_handler);
    printf("\n");
    char * cwd = (char *) malloc(1024 * sizeof(char));
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    printf("%s" , cwd);
    printf("$ ");
    fflush(stdout);
    return;
}

void sigchild_handler(int signum){
    signal(SIGCHLD,sigchild_handler);
    while(1){
        int status;
        int cpid = waitpid(-1,&status,WUNTRACED|WNOHANG|WCONTINUED); //WCONTINUED check
        if(cpid<=0)break;
        if(fg_procs.find(cpid)!=fg_procs.end()){
            fg_procs.erase(cpid);
            if(WIFSTOPPED(status))bg_stop_procs.insert(cpid);
        }
        else if(bg_run_procs.find(cpid)!=bg_run_procs.end()){
            bg_run_procs.erase(cpid);
            if(WIFSTOPPED(status))bg_stop_procs.insert(cpid);
        }
    }    
}

void sigchld_blocker(int signal_state){
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set , SIGCHLD);
    sigprocmask(signal_state , &signal_set , NULL);
}

int main(){
    fg_procs.clear();
    bg_run_procs.clear();
    bg_stop_procs.clear();
    
    rl_bind_keyseq("\\e[A",up_function);
    rl_bind_keyseq("\\e[B",down_function);
    rl_bind_key(1,rl_beg_of_line);
    rl_bind_key(5,rl_end_of_line);
    
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    signal(SIGCHLD,sigchild_handler);
    signal(SIGTTOU,SIG_IGN);
    
    foreground_pgid = 0;
    
    while(1){
        history.get_history();
        history.manage_history();
        if(history.line==NULL)exit(0);
        if(strcmp(history.line,"exit")==0)break;
        process *job;
        int n_proc;
        int background = 0;
        
        if(strlen(history.line)==0)continue;
        job = parse(history.line,&n_proc,&background);
        exec_job(job,n_proc,background);       
    }
}