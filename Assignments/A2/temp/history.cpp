#include "history.h"

int getch() {
    int ch;
    struct termios t_old, t_new;
    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

void getHistory(deque<string> &history, int &historyIndex, string &currentLine){
    char * cwd = (char *) malloc(1024 * sizeof(char));
    if(getcwd(cwd , 1024) == NULL){
        perror("getcwd");
        exit(0);
    }
    printf("\033[34m");
    printf("%s" , cwd);
    printf("\033[0m");
    printf("$ ");
    while(true){
        int ch = getch();
        // printf("%d",ch);
        if(ch==1){
            // printf("Ctrl+A\n");
            printf("\033[");
            printf("%d",currentLine.size());
            printf("D");
            continue;
        }
        else if(ch==2){
            printf("\033[");
            printf("%d",currentLine.size());
            printf("C");
            continue;
        }
        if (ch == 27) {
            ch = getch();
            if (ch == 91) {
                ch = getch();
                if (ch == 65) {
                    // printf("hell\n");
                    // Up arrow
                    if (historyIndex > 0) {
                        if(historyIndex==history.size() && !currentLine.empty() && history[historyIndex-1]!=currentLine){
                            history.push_back(currentLine);
                            if(history.size()>MAX_COMMANDS){
                                history.pop_front();
                                historyIndex--;
                            }
                        }    
                        else if(!currentLine.empty())
                            history[historyIndex] = currentLine;
                        printf("\33[2K");
                        printf("\r");
                        if(historyIndex>0)
                            historyIndex--;
                        currentLine = history[historyIndex];
                        printf("\033[34m");
                        printf("%s" , cwd);
                        printf("\033[0m");
                        printf("$ %s",currentLine.c_str());
                    }
                } else if (ch == 66) {
                    // Down arrow
                    if(historyIndex==0 && history.size()==0)
                        continue;
                    if (historyIndex < (int)history.size()-1) {
                        printf("\33[2K");
                        printf("\r");
                        // history[historyIndex] = currentLine;
                        historyIndex++;
                        currentLine = history[historyIndex];
                        printf("\033[34m");
                        printf("%s" , cwd);
                        printf("\033[0m");
                        printf("$ %s",currentLine.c_str());
                        
                    }
                    else if(historyIndex==(int)history.size()-1){
                        printf("\33[2K");
                        printf("\r");
                        historyIndex++;
                        currentLine.clear();
                        printf("\033[34m");
                        printf("%s" , cwd);
                        printf("\033[0m");
                        printf("$ ");
                    }
                }
            }
        } else if (ch == '\n') {
            if (!currentLine.empty()) {
                if(history.size()>0 && history[history.size()-1]!=currentLine)
                    history.push_back(currentLine);
                else if(history.size()==0)
                    history.push_back(currentLine);
                if(history.size()>MAX_COMMANDS) history.pop_front();
                historyIndex = (int)history.size();
                printf("\n");
                free(cwd);
                return;
            }
            else {
                free(cwd);
                printf("\n");
                return;
            }   
        } else {
            
            if(ch==127)
            {
                if(currentLine.size()>0){
                    currentLine.pop_back();
                    printf("\b \b");
                }
            }
            else{
                printf("%c",ch);
                currentLine += (char)ch;
            } 
        }
    }
    free(cwd);
}