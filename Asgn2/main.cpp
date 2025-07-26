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
#include "history.h"

using namespace std;

int foreground_pgid;
set <int> fg_procs,bg_run_procs,bg_stop_procs;
shell_history history;
int ctrl_z_received = 0;

int up_function(int s1,int s2){
	if(history.history_idx==0){
		return 0;
	}
    else if(history.history_cnt == history.history_idx){
        strcpy(history.latest_command,rl_line_buffer);
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
		rl_replace_line(history.latest_command,0); //check
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
    char cwd[1024];
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    // printf("\033[34m");
    printf("%s" , cwd);
    // printf("\033[0m");
    printf("$ ");
    fflush(stdout);
    return;
}

void sigtstp_handler(int signum){
    signal(SIGTSTP,sigtstp_handler);
    printf("\n");
    char cwd[1024];
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    // printf("\033[34m");
    printf("%s" , cwd);
    // printf("\033[0m");
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
            if(WIFSTOPPED(status)){
                bg_stop_procs.insert(cpid);
                ctrl_z_received = 1;
            }
        }
        else if(bg_run_procs.find(cpid)!=bg_run_procs.end()){
            bg_run_procs.erase(cpid);
            if(WIFSTOPPED(status))bg_stop_procs.insert(cpid);
        }
        else if(bg_stop_procs.find(cpid)!=bg_stop_procs.end()){
            bg_stop_procs.erase(cpid);
            if(WIFCONTINUED(status))bg_run_procs.insert(cpid);
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

    char **lsof = (char **)malloc(6*sizeof(char *));
    lsof[0] = strdup("lsof");
    lsof[1] = strdup("-t");
    lsof[2] = strdup("-f");
    lsof[3] = strdup("--");
    lsof[5] = NULL;
    
    while(1){
        history.get_history();
        history.manage_history();
        if(history.line==NULL)exit(0);
        if(strcmp(history.line,"exit")==0)break;
        process *job;
        int n_proc;
        int background = 0;
        
        if(history.line==NULL || strlen(history.line)==0){
            if(history.line!=NULL)
                free(history.line);
            continue;
        }
        job = parse(history.line,&n_proc,&background);
        if(strcmp(job[0].args[0],"delep")==0){
            lsof[4] = strdup(job[0].args[1]);
            int infd=0,outfd,testfd;
            int fd[2];
            if(pipe(fd) < 0){
                perror("pipe");
                exit(0);
            }
            outfd = fd[1];
            testfd = fd[0];
            int c_pid = fork();
            if(c_pid == 0){
                dup2(outfd,STDOUT_FILENO);
                close(outfd);
                execvp(lsof[0],lsof);
                perror("execvp");
                exit(0);
            }
            else{
                close(outfd);
                char buf[MAX_RES_LEN];
                int n = read(testfd , buf , MAX_RES_LEN);
                buf[n] = '\0';
                close(testfd);
                // lsof -e /run/user/125/gvfs -t -f -- ./t3.txt
                // lsof -t -f -- ./t3.txt
                if(n <= 1)
                    printf("No process has currently opened the file or held a lock.\n");
                else{
                    printf("Processes currently opening the file or holding a lock over it:\n\nPIDs\n");
                    printf("%s\n",buf);
                    printf("Do you want to kill all the processes using the file (yes/no)? ");
                    char ans[10];
                    fgets(ans,10,stdin);
                    if(strcmp(ans,"yes\n") == 0){
                        char * token = strtok(buf , "\n");
                        while(token != NULL){
                            printf("Killed: %s\n",token);
                            int pid = atoi(token);
                            kill(pid , SIGKILL);
                            token = strtok(NULL , "\n");
                        }
                        remove(job[0].args[1]);
                    } 
                }
            }
            free(lsof[4]);
            for(int i=0;i<n_proc;i++){
                for(int j=0;j<job[i].n_args;j++){
                    free(job[i].args[j]);
                }
                free(job[i].args);
            }
        }
        else
            exec_job(job,n_proc,background);   
    }
}