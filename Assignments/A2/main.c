#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#include "parse.h" 



#define MAX_LINE_LEN 1024






int main(){
    char *line = malloc(MAX_LINE_LEN * sizeof(char)); 
    // strcpy(line,"ls -l|grep 'hello     world'|wc -l -c");
    size_t max_line_len = MAX_LINE_LEN;
    printf("Enter the command: ");
    getline(&line, &max_line_len, stdin);
    char ***commands = parse(line);
    int i, j;

    for (i = 0; commands[i] != NULL; i++) {
        for (j = 0; commands[i][j] != NULL; j++)
            printf("%s ", commands[i][j]);
        printf("\nNumber of arguments = %d\n\n",j);
    }
}