#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glob.h>
#include "parse.h"

#define MAX_LINE_LEN 1024

void remove_spaces(char *line){
	int i=0,j=0,k=strlen(line),f=1;
	if(k==0)	return;
    int inside_single_quotes=0,inside_double_quotes=0,escape_char=0;
	char b[k+1];
	while(line[i]!='\0'){
        if (line[i] == '\'' && !inside_double_quotes && !escape_char)
            inside_single_quotes = !inside_single_quotes;
        else if (line[i] == '\"' && !inside_single_quotes && !escape_char)
            inside_double_quotes = !inside_double_quotes;
        else if (line[i] == '\\' && !escape_char)
            escape_char = 1;
        else if (line[i] == ' ' && !inside_single_quotes && !inside_double_quotes && !escape_char && f)
        {
            i++;
            continue;
        }
        else
            escape_char = 0;
        b[j++] = line[i];
		if(line[i]==' ')    f=1;
        else f=0;
        i++;
	}
    if(b[j-1]==' ') b[j-1]='\0';
	else b[j] = '\0';
	strcpy(line,b);
	//line = realloc(line,strlen(line));
}

// void init_proc(process * p){     // initialize process structure content
//     for(int i=0;i<MAX_ARGS;i++){
//         p->args[i] = NULL;
//     }
//     p->n_args = 0;
//     return;
// }

// Function to remove back slashes from a string
void remove_back_slashes(char *a){
    int i=0,j=0,k=strlen(a);
	if(k==0)	return;
	char b[k+1];
	while(a[i]!='\0'){
		if(a[i++]=='\\')	continue;
        b[j++] = a[i-1];
	}
    b[j] = '\0';
	strcpy(a,b);
}

// Function to remove quotes from a string, if any at start and end
void remove_quotes(char *a){
    int i=0,j=0,k=strlen(a);
    if(k==0)	return;
    char b[k+1];
    if(a[0]=='\"' || a[0]=='\'')    i++;
    while(a[i]!='\0')
        b[j++] = a[i++];
    if(b[j-1]=='\"' || b[j-1]=='\'')    b[j-1]='\0';
    else
        b[j] = '\0';
    strcpy(a,b);
}


process * parse(char *line , int * n_proc , int * background) {
    remove_spaces(line);
    int i,j,k,len = strlen(line);
    int inside_single_quotes=0,inside_double_quotes=0,escape_char=0;
    int pipes_count=0,arg_count=0,start=0, end=0,tem_start;
    // char ***commands;
    process * job;
    for(i=0;i<len;i++){
        if (line[i] == '\'' && !inside_double_quotes && !escape_char)
            inside_single_quotes = !inside_single_quotes;
        else if (line[i] == '\"' && !inside_single_quotes && !escape_char)
            inside_double_quotes = !inside_double_quotes;
        else if (line[i] == '\\' && !escape_char)
            escape_char = 1;
        else if (line[i] == '|' && !inside_single_quotes && !inside_double_quotes && !escape_char)
            pipes_count++;
        else
            escape_char = 0;
    }

    // commands = (char ***) malloc((pipes_count + 2) * sizeof(char **));
    // commands[pipes_count+1] = NULL;
    job = (process *) malloc((pipes_count + 1) * sizeof(process));
    *n_proc = pipes_count + 1;
    // Loop over the commands
    for(i=0;i<=pipes_count;i++){
        inside_single_quotes = 0;
        inside_double_quotes = 0;
        escape_char = 0;
        arg_count = 0;
        // init_proc(&job[i]);
        //Find the end of the command
        for (end = start; end < len; end++) {
            if (line[end] == '\'' && !inside_double_quotes && !escape_char)
                inside_single_quotes = !inside_single_quotes;
            else if (line[end] == '\"' && !inside_single_quotes && !escape_char)
                inside_double_quotes = !inside_double_quotes;
            else if (line[end] == '\\' && !escape_char)
                escape_char = 1;
            else if (line[end] == '|' && !inside_single_quotes && !inside_double_quotes && !escape_char)
                break;
            else
                escape_char = 0;
        }
        if(line[start]==' ')    start++;
        tem_start = start;
        // Count the number of arguments
        for (j = start; j < end; j++) {
            if (line[j] == '\'' && !inside_double_quotes && !escape_char)
                inside_single_quotes = !inside_single_quotes;
            else if (line[j] == '\"' && !inside_single_quotes && !escape_char)
                inside_double_quotes = !inside_double_quotes;
            else if (line[j] == '\\' && !escape_char)
                escape_char = 1;
            else if (line[j] == ' ' && !inside_single_quotes && !inside_double_quotes && !escape_char)
                arg_count++;
            else
                escape_char = 0;
        }
        if(line[j-1]!=' ')
            arg_count++;
        // commands[i] = (char **) malloc((arg_count + 1) * sizeof(char *));
        job[i].args = (char **) malloc((arg_count+1) * sizeof(char *));
        job[i].n_args = arg_count;

        start = tem_start;
        j = start;
        inside_single_quotes = 0;
        inside_double_quotes = 0;
        escape_char = 0;
        // Loop over the arguments and copy them
        for (k = 0; k < arg_count; k++) {
            while(j<end){
                if (line[j] == '\'' && !inside_double_quotes && !escape_char)
                    inside_single_quotes = !inside_single_quotes;
                else if (line[j] == '\"' && !inside_single_quotes && !escape_char)
                    inside_double_quotes = !inside_double_quotes;
                else if (line[j] == '\\' && !escape_char)
                    escape_char = 1;
                else if (line[j] == ' ' && !inside_single_quotes && !inside_double_quotes && !escape_char)
                    break;
                else
                    escape_char = 0;
                j++;
            }

            job[i].args[k] = (char *) malloc((j - start + 1) * sizeof(char));

            strncpy(job[i].args[k], line + start, j - start);
            job[i].args[k][j - start] = '\0';
            if(strcmp(job[i].args[k],"&")==0){
                *background = 1;
                free(job[i].args[k]);
                job[i].n_args = k;
                break;
            }
            if(job[i].args[k][0]=='\"' || job[i].args[k][0]=='\'')
                remove_quotes(job[i].args[k]);
            else
                remove_back_slashes(job[i].args[k]);
            start = j + 1;
            j = start;
        }

        job[i].args[job[i].n_args] = NULL;
        if(*background==1)
            break;
        
        start = end + 1;
        end = start;
        // printf("Number of arguments: %d\n",job[i].n_args);
        // for(int p=0;p<job[i].n_args;p++){
        //     printf("%s ",job[i].args[p]);
        // }
        // printf("\n\n");
    }

    return job;
}


// int main(){
//     char *line = malloc(MAX_LINE_LEN * sizeof(char)); 
//     // strcpy(line,"ls -l|grep 'hello     world'|wc -l -c");
//     int max_line_len = MAX_LINE_LEN;
//     printf("Enter the command: ");
//     getline(&line, &max_line_len, stdin);
//     char ***commands = parse(line);
//     int i, j;

//     for (i = 0; commands[i] != NULL; i++) {
//         for (j = 0; commands[i][j] != NULL; j++)
//             printf("%s ", commands[i][j]);
//         printf("\nNumber of arguments = %d\n\n",j);
//     }
// }