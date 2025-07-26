#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glob.h>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include "parse.h"

#define MAX_LINE_LEN 1024

using namespace std;

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


// Function to remove back slashes from a string
void remove_back_slashes(string &a){
    int i=0,j=0,k=a.size();
	if(k==0)	return;
	char b[k+1];
	while(a[i]!='\0'){
		if(a[i++]=='\\')	continue;
        b[j++] = a[i-1];
	}
    b[j] = '\0';
	a = b;
}

// Function to remove quotes from a string, if any at start and end
void remove_quotes(string &a){
    int i=0,j=0,k=a.size();
	if(k==0)	return;
	char b[k+1];
    if(a[0]=='\"' || a[0]=='\'')    i++;
    while(a[i]!='\0')
        b[j++] = a[i++];
    if(b[j-1]=='\"' || b[j-1]=='\'')    b[j-1]='\0';
    else
        b[j] = '\0';
    a = b;
}

// Function to convert 2-d character array to list of strings
void convert_to_list(char **a, int n, list<string> &l){
    for(int i=0;i<n;i++){
        l.push_back(a[i]);
    }
}


// Function to handle wildcards like * and ?
char ** handle_wildcards(char **a, int &n){
    list<string> l,m;
    convert_to_list(a,n,l);
    int i=0;
    for(i=0;i<n;i++){
        free(a[i]);
    }
    for(auto it = l.begin(); it != l.end(); it++){
        if((*it)[0]=='\"' || (*it)[0]=='\''){
            remove_quotes(*it);
            m.push_back(*it);
        }
        else{
            glob_t glob_result;
            if(glob((*it).c_str(),0,NULL,&glob_result)!=0){
                remove_back_slashes(*it);
                m.push_back(*it);
            }
            else{
                for(int j=0;j<glob_result.gl_pathc;j++){
                    m.push_back(glob_result.gl_pathv[j]);
                }
            }  
            globfree(&glob_result);
        }
    }
    n = m.size();
    a = (char **) realloc(a,(n+1)*sizeof(char *));
    i=0;
    for(auto it = m.begin(); it != m.end(); it++){
        a[i++] = strdup((*it).c_str());
    }
    a[i] = NULL;
    return a;
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


    job = (process *) malloc((pipes_count + 1) * sizeof(process));
    *n_proc = pipes_count + 1;
    // Loop over the commands
    for(i=0;i<=pipes_count;i++){
        inside_single_quotes = 0;
        inside_double_quotes = 0;
        escape_char = 0;
        arg_count = 0;

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

            start = j + 1;
            j = start;
        }

        job[i].args=handle_wildcards(job[i].args,job[i].n_args);

        if(*background==1)
            break;
        
        start = end + 1;
        end = start;

    }
    return job;
}
