#include <unistd.h>
#include <fcntl.h>
#include "process.h"
#include <string.h>

void redirect(process * proc , int infd , int outfd){

    char ** final_args = NULL;
    int final_nargs = proc->n_args;

    // Wrong, will not work for cases like: grep -i "hello" > out.txt
    for(int i=0;i<proc->n_args;i++){
        if(strcmp(proc->args[i] , "<")==0){
            i++;
            final_nargs -= 2;
        }
        else if(strcmp(proc->args[i] , ">")==0){
            i++;
            final_nargs -= 2;
        }
    }

    final_args = (char **) malloc((final_nargs+1) * sizeof(char *));    // free after execvp in exec_proc
    final_nargs = 0;

    // initial unconditional redirects
    if(infd != STDIN_FILENO){
        dup2(infd , STDIN_FILENO);
        close(infd);
    }
    if(outfd != STDOUT_FILENO){
        dup2(outfd , STDOUT_FILENO);
        close(outfd);
    }
    
    // check for explicit redirects to specific file names AFTER (possible) piping
    // this is the behavior that actual shells express. Pipe redirects have LESS priority over file redirects
    for(int i=0;i<proc->n_args;i++){

        // check for input redirect
        if(strcmp(proc->args[i] , "<")==0){
            i++;            // assuming next arg as input file path
            int new_infd;
            if((new_infd = open(proc->args[i], O_RDONLY)) < 0){      // ## CHECK permissions!
                perror("open");
                exit(0);
            }
            dup2(new_infd , infd);
            close(new_infd);
            free(proc->args[i-1]);
            free(proc->args[i]);
        }

        // check for output redirect
        else if(strcmp(proc->args[i] , ">")==0){
            i++;            // assuming next arg as output file path
            int new_outfd;
            if((new_outfd = open(proc->args[i] , O_WRONLY | O_TRUNC | O_CREAT , 0766)) < 0){        // ## CHECK permissions!
                perror("open");
                exit(0);
            }
            dup2(new_outfd , outfd);
            close(new_outfd);
            free(proc->args[i-1]);
            free(proc->args[i]);
        }

        else{
            final_args[final_nargs++] = strdup(proc->args[i]);      // else, make a deep copy of the arguments
            free(proc->args[i]);        // free the original argument (if not a redirect)
        }
    }

    final_args[final_nargs] = NULL;         // argument list end sentinel (for execvp's use)
    int k=0;
    while(final_args[k]!=NULL){
        proc->args[k] = final_args[k];
        k++;
    }
    proc->n_args = final_nargs;
    // Do not uncomment the following lines. They will free the arguments in the process structure, which is not what we want
    // for(k=0;k<=final_nargs;k++){
    //     free(final_args[k]);
    // }
    free(final_args);
}

// following error cases need to be handled during parsing
// 1) > (or < )without any file name after it
// 2) no space between < (or >) and file name

// Do we need to close the other end of the pipe (in the successive sibling process, (if any at all) in case a user does both ">" withing a proc and uses "|" after a proc??
// Means, do we need to do this ourselves to avoid a pipe burst or is it handled by default behaviour?