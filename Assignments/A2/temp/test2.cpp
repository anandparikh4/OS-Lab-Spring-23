#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <chrono>


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
    std::vector<std::string> history;
    int historyIndex = -1;
    std::string currentLine;
    while (true) {
        
        std::cout << "\33[2K" << "\r";
        std::cout  << "$ "<< currentLine;
        
        int ch = getch();
        if (ch == 27) {
            ch = getch();
            if (ch == 91) {
                ch = getch();
                if (ch == 65) {
                    // Up arrow
                    if (historyIndex > 0) {
                        // history[historyIndex] = currentLine;
                        historyIndex--;
                        currentLine = history[historyIndex];
                        
                    }
                } else if (ch == 66) {
                    // Down arrow
                    if (historyIndex < (int)history.size() - 1) {
                        // history[historyIndex] = currentLine;
                        historyIndex++;
                        currentLine = history[historyIndex];
                        
                    } else {
                        currentLine.clear();
                    }
                }
                
            }
        } else if (ch == '\n') {
            if (!currentLine.empty()) {
                history.push_back(currentLine);
                historyIndex = history.size();
                std::cout << "\nYou entered: " << currentLine << std::endl;
                currentLine.clear();
                int j=0;
                while(j<4){
                    printf("dfkjwk\n");
                    j++;
                }
                
            }
        } else {
            
            if(ch==127)
            {
                currentLine.pop_back();
            }
            else currentLine += (char)ch;
        }
    }
    return 0;
}
