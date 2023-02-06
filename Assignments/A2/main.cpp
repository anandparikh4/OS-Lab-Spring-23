#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <set>
#include <sys/signal.h>
#include <iostream>
#include <termios.h>
#include <deque>
#include "parse.h" 
#include "exec_job.h"
#include "history.h"

using namespace std;

int foreground_pgid;
set <int> fg_procs,bg_run_procs,bg_stop_procs;

void sigint_handler(int signum){
    signal(SIGINT,sigint_handler);
    printf("\n");
    char * cwd = (char *) malloc(1024 * sizeof(char));
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    printf("\033[34m");
    printf("%s" , cwd);
    printf("\033[0m");
    printf("$ ");
    fflush(stdout);
    free(cwd);
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
    printf("\033[34m");
    printf("%s" , cwd);
    printf("\033[0m");
    printf("$ ");
    fflush(stdout);
    free(cwd);
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
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
    signal(SIGCHLD,sigchild_handler);
    signal(SIGTTOU,SIG_IGN);
    foreground_pgid = 0;
    deque<string> history;
    int historyIndex = 0;
    string currentLine = "";
    char **lsof = (char **)malloc(8*sizeof(char *));
    lsof[0] = strdup("lsof");
    lsof[1] = strdup("-e");
    lsof[2] = strdup("/run/user/125/gvfs");
    lsof[3] = strdup("-t");
    lsof[4] = strdup("-f");
    lsof[5] = strdup("--");
    lsof[7] = NULL;

    // rl_bind_key('1', rl_beg_of_line);
    // rl_bind_key('9', rl_end_of_line);
    
    while(1){
        currentLine.clear();
        getHistory(history,historyIndex,currentLine);
        if(currentLine.size() == 0)continue;
        char *line = (char *)malloc((currentLine.size() + 1)*sizeof(char));
        strcpy(line,currentLine.c_str());
        if(strcmp(line,"exit")==0)break;
        process *job;
        int n_proc;
        int background = 0;

        job = parse(line,&n_proc,&background);
        free(line);
        if(strcmp(job[0].args[0],"delep")==0){
            lsof[6] = strdup(job[0].args[1]);
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
                // int n = read(testfd , buf , MAX_RES_LEN);
                // buf[n] = '\0';
                // close(testfd);
                // printf("%s",buf);
                int n = read(testfd , buf , MAX_RES_LEN);
                buf[n] = '\0';
                close(testfd);
                // close(outfd);
                // close(STDIN_FILENO);
                // close(STDOUT_FILENO);
                // infd = open("/dev/tty", O_RDONLY);
                // if (infd == -1) {
                //     perror("open");
                //     return;
                // }
                // outfd = open("/dev/tty", O_WRONLY);
                // if (outfd == -1) {
                //     perror("open");
                //     return;
                // }
                // dup2(STDIN_FILENO,testfd);
                // lsof -e /run/user/125/gvfs -t -f -- ./t3.txt

                printf("%s",buf);
                if(n <= 1)
                    printf("No such process\n");
                else{
                    printf("Do you want to kill all the processes using the file (yes/no)? ");
                    char ans[10];
                    fgets(ans,10,stdin);
                    // fflush(stdin);
                    // cin>>ans;
                    if(strcmp(ans,"yes\n") == 0){
                        char * token = strtok(buf , "\n");
                        while(token != NULL){
                            printf("Killed: %s\n",token);
                            int pid = atoi(token);
                            kill(pid , SIGKILL);
                            token = strtok(NULL , "\n");
                        }
                        // printf("job[0].args[6] = %s\n",job[0].args[6]);
                        remove(job[0].args[1]);
                    } 
                }
            }
            free(lsof[6]);
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