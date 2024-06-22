#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 45

void words_in_file(int fd, char *filename) {
    char word[MAX_WORD_LENGTH];
    int word_length = 0;
    char c;
    int lineNum = 1;
    int colNum = 1;
    int frontcut = 0;
    while (read(fd, &c, 1) > 0) {
        if(isspace(c) || c == '\n') {
            if (word_length > 0) {
                word[word_length] = '\0'; 
                for(int i = 0; i < MAX_WORD_LENGTH && word_length > 0; i++){
                    if(!isalpha(word[0]) || word[0] == '\"' || word[0] == '\'' || word[0] == '[' || word[0] == '(' || word[0] == '{'){
                        strncpy(word, word + 1, word_length - 1);
                        word[word_length - 1] = '\0';
                        word_length = word_length-1;
                        frontcut++;
                    }
                    else 
                        i = MAX_WORD_LENGTH;
                }
                for(int i = 0; i < MAX_WORD_LENGTH && word_length > 0; i++){
                    if(!isalpha(word[word_length-1])){
                        strncpy(word, word, word_length - 1);
                        word[word_length - 1] = '\0';
                        word_length = word_length-1;
                        frontcut++;
                    }
                    else    
                        i = MAX_WORD_LENGTH;
                }
                if (word_length > 0)
                    printf("%s (%d,%d): %s\n", filename, lineNum, colNum-word_length-frontcut, word);
                word_length = 0;
            }

            if(c == '\n'){
                lineNum++;
                colNum = 1;
            }
            else
                colNum++;
        }
        else{
            if (word_length < MAX_WORD_LENGTH - 1) {
                word[word_length++] = c;
            } else {
                word_length = 0;
            }
            colNum++;
        }
        frontcut = 0;
    }

    if (word_length > 0) {
        word[word_length] = '\0'; 
        for(int i = 0; i < MAX_WORD_LENGTH && word_length > 0; i++){
            if(!isalpha(word[0]) || word[0] == '\"' || word[0] == '\'' || word[0] == '[' || word[0] == '(' || word[0] == '{'){
                strncpy(word, word + 1, word_length - 1);
                word[word_length - 1] = '\0';
                word_length = word_length-1;
                frontcut++;
            }
            else 
                i = MAX_WORD_LENGTH;
        }
        for(int i = 0; i < MAX_WORD_LENGTH && word_length > 0; i++){
            if(!isalpha(word[word_length-1])){
                strncpy(word, word, word_length - 1);
                word[word_length - 1] = '\0';
                word_length = word_length-1;
                frontcut++;
            }
            else    
                i = MAX_WORD_LENGTH;
        }
        if (word_length > 0)
            printf("%s (%d,%d): %s\n", filename, lineNum, colNum-word_length-frontcut, word);
        word_length = 0;
    }

    close(fd);
}

int main(int argc, char* argv[]){
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("Unable to open file: %s\n", argv[1]);
        return 0;
    }
    words_in_file(fd, argv[1]);
    return 1;
}