#include <unistd.h>
#include <fcntl.h>
#include "process.h"

// ## CHECK permission for read and write files - Currently allowing owner to do all. And group members/other users can only read/write, not execute (for obvious reasons)

// char ** redirect(process * p , infd , outfd){       // DO add extra NULL at the end
//     // first, redirect to infd and outfd unconditionally
//     // then loop over the arguments of the given process to find any < or > symbols
//     // if found, again dup2 the stdout and stdin to the file names, after opening and closing them appropriately
//     // return the final arguments after stripping off <,>
// }

char ** redirect(struct process * proc , int infd , int outfd){
    // initial unconditional redirects
    if(infd != STDIN_FILENO){
        close(STDIN_FILENO);            // ## need error handling here? or will it be pedantic?
        dup2(infd , STDIN_FILENO);
    }
    if(outfd != STDOUT_FILENO){
        close(STDOUT_FILENO);
        dup2(outfd , STDOUT_FILENO);
    }
    
    // check for explicit redirects to specific file names AFTER (possible) piping
    // this is the behavior that actual shells express. Pipe redirects have LESS priority over file redirects
    for(int i=0;i<proc->n_args;i++){

        // check for input redirect
        if(strcmp(proc->args[i] , "<")){
            i++;            // assuming next arg as input file path
            int new_infd;
            if((new_infd = open(proc->args[i], O_RDONLY)) < 0){      // ## CHECK permissions!
                perror("open");
                exit(0);
            }
            close(infd);
            dup2(new_infd , infd);
        }

        // check for output redirect
        else if(strcmp(proc->args[i] , ">")){
            i++;            // assuming next arg as output file path
            int new_outfd;
            if((new_outfd = open(proc->args[i] , O_WRONLY | O_TRUNC | O_CREAT , 0766)) < 0){        // ## CHECK permissions!
                perror("open");
                exit(0);
            }
            close(outfd);
            dup2(new_outfd , outfd);
        }
    }
}

// following error cases need to be handled during parsing
// 1) > (or < )without any file name after it
// 2) no space between < (or >) and file name

// Do we need to close the other end of the pipe (in the successive sibling process, if any at all) in case a user does both ">" withing a proc and uses "|" after a proc??
// Means, do we need to do this ourselves to avoid a pipe burst or is it handled by default behaviour?