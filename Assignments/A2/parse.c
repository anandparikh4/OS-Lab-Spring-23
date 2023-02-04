#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	line = realloc(line,strlen(line));
}

char ***parse(char *line) {
    remove_spaces(line);
    int i,j,k,len = strlen(line);
    int inside_single_quotes=0,inside_double_quotes=0,escape_char=0;
    int pipes_count=0,arg_count=0,start=0, end=0,tem_start;
    char ***commands;

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

    commands = (char ***) malloc((pipes_count + 2) * sizeof(char **));
    commands[pipes_count+1] = NULL;
    for(i=0;i<=pipes_count;i++){
        inside_single_quotes = 0;
        inside_double_quotes = 0;
        escape_char = 0;
        arg_count = 0;
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
        commands[i] = (char **) malloc((arg_count + 1) * sizeof(char *));

        start = tem_start;
        j = start;
        inside_single_quotes = 0;
        inside_double_quotes = 0;
        escape_char = 0;
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

            commands[i][k] = (char *) malloc((j - start + 1) * sizeof(char));

            strncpy(commands[i][k], line + start, j - start);
            commands[i][k][j - start] = '\0';

            start = j + 1;
            j = start;
        }

        commands[i][arg_count] = NULL;

        start = end + 1;
        end = start;
    }

    return commands;
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