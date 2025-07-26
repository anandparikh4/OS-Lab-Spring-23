#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
using namespace std;

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

int main() {
    printf("hi\n");
    vector<string> history;
    int historyIndex = 0;
    string currentLine;
    while (true) {
        
        // std::cout << "\33[2K" << "\r";
        printf("$ ");
        while(true){
            int ch = getch();
            // printf("%d",ch);
            if (ch == 27) {
                ch = getch();
                if (ch == 91) {
                    ch = getch();
                    if (ch == 65) {
                        // printf("hell\n");
                        // Up arrow
                        if (historyIndex > 0) {
                            if(historyIndex==history.size() && !currentLine.empty() && history[historyIndex-1]!=currentLine)    history.push_back(currentLine);
                            else if(!currentLine.empty())
                                history[historyIndex] = currentLine;
                            printf("\33[2K");
                            printf("\r");
                            historyIndex--;
                            currentLine = history[historyIndex];
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
                            printf("$ %s",currentLine.c_str());
                            
                        }
                        else if(historyIndex==(int)history.size()-1){
                            printf("\33[2K");
                            printf("\r");
                            historyIndex++;
                            currentLine.clear();
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
                    historyIndex = (int)history.size();
                    printf("\n");
                    // printf("\nEntered Line: %s\n",currentLine.c_str());
                    // cout << "\nYou entered: " << currentLine << endl;
                    if(currentLine=="exit")
                        return 0;
                    // else if(currentLine=="history"){
                    //     for(int i=0;i<history.size();i++)
                    //         printf("%s, ",history[i].c_str());
                    // }
                    currentLine.clear();
                    break;
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
    }
    return 0;
}
