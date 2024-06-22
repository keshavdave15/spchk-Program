#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define NUM_STRINGS 104334
#define MAX_STRING_LENGTH 45

int binary_search(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word) {
    int low = 0;
    int high = NUM_STRINGS-1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(dictionary[mid], word);
        if (cmp == 0) {
            return 1; 
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return 0;
}


int part_check(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word){
    int allCaps = 1;
    int onlyFirstCap = 1;
    for(int j = 0; j < MAX_STRING_LENGTH; j++){
        if(isalpha(word[j])){
            if(!isupper(word[j]))
                allCaps = 0;
            if(j == 0){
                if(!isupper(word[j]))
                    onlyFirstCap = 0;
            }
            else{
                if(isupper(word[j]))
                    onlyFirstCap = 0;
            }
        }
        if(word[j] == '\0' || word[j] == '-'){
            j = MAX_STRING_LENGTH;
        }
    }

    char holder[MAX_STRING_LENGTH];
    strcpy(holder, word);
    if(allCaps){
        for(int j = 0; j < MAX_STRING_LENGTH; j++){
            if(isalpha(word[j])){
                word[j] = tolower(word[j]);
            }
            if(word[j] == '\0' || word[j] == '-'){
                j = MAX_STRING_LENGTH;
            }
        }
        if(binary_search(dictionary, word) || binary_search(dictionary, holder))
            return 1;
    }
    else if(onlyFirstCap){
        word[0] = tolower(word[0]);
        if(binary_search(dictionary, word) || binary_search(dictionary, holder))
            return 1;
    }
    
    else{
        return binary_search(dictionary, word);
    }
}

int word_check(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word){
    if(strcmp(word, "MACDONALD") == 0 || strcmp(word, "MACARTHUR") == 0 || strcmp(word, "MACBRIDE") == 0 || strcmp(word, "MACLEISH") == 0 || word, "MACDONALD'S" == 0 || strcmp(word, "MACARTHUR'S") == 0 || strcmp(word, "MACBRIDE'S") == 0 || strcmp(word, "MACLEISH'S") == 0)
        return 1;
    int remember = 0;
    char holder[MAX_STRING_LENGTH];
    for(int i = 0; i < MAX_STRING_LENGTH; i++){
        if(word[i] == '\0' || word[i] == '-'){
            if(!(remember == i)){
                strcpy(holder, word+remember);
                holder[i-remember] = '\0';
                if(!part_check(dictionary, holder)){
                    return 0;
                }

            }
            remember = i+1;
            if(word[i] == '\0')
                i = MAX_STRING_LENGTH;
        }
    }
    return 1;
}

int main(int argc, char* argv[]){
    int fd = open("/usr/share/dict/words", O_RDONLY);
    if (fd == -1) {
        printf("Unable to open dictionary file: %s\n", "/usr/share/dict/words");
        return 0;
    }
    char dictionary[NUM_STRINGS][MAX_STRING_LENGTH];
    char word[MAX_STRING_LENGTH];
    int word_length = 0;
    char c;

    int word_count = 0;
    while (read(fd, &c, 1) > 0 || word_length > 0) {
        if (isspace(c) || c == '\n' || c == EOF) {
            if (word_length > 0) {
                if (word_count < NUM_STRINGS) {
                    word[word_length] = '\0';
                    strcpy(dictionary[word_count], word);
                    word_count++;
                } else {
                    printf("Maximum word count exceeded.\n");
                    break;
                }
            }
            word_length = 0;
        } else {
            if (word_length < MAX_STRING_LENGTH - 1) {
                word[word_length++] = c;
            } else {
                word_length = 0;
            }
        }
    }

    if(!word_check(dictionary, "happy-Happy-HAPPY"))
        printf("Misspelled word: %s\n", "happy-Happy-HAPPY");
    if(!word_check(dictionary, "HELLO"))
        printf("Misspelled word: %s\n", "HELLO");
    if(!word_check(dictionary, "hello"))
        printf("Misspelled word: %s\n", "hello");
    if(!word_check(dictionary, "Hello"))
        printf("Misspelled word: %s\n", "Hello");
    if(!word_check(dictionary, "heLLo"))
        printf("Misspelled word: %s\n", "heLLo");
    if(!word_check(dictionary, "mesh"))
        printf("Misspelled word: %s\n", "mesh");
    if(!word_check(dictionary, "mes$h"))
        printf("Misspelled word: %s\n", "mes$h");
    if(!word_check(dictionary, "test"))
        printf("Misspelled word: %s\n", "test");
    if(!word_check(dictionary, "tesst"))
        printf("Misspelled word: %s\n", "tesst");
    if(!word_check(dictionary, "I"))
        printf("Misspelled word: %s\n", "I");
    if(!word_check(dictionary, "am"))
        printf("Misspelled word: %s\n", "am");
    if(!word_check(dictionary, "happyy"))
        printf("Misspelled word: %s\n", "happyy");
    if(!word_check(dictionary, "today"))
        printf("Misspelled word: %s\n", "today");
    if(!word_check(dictionary, "round-about"))
        printf("Misspelled word: %s\n", "round-about");
    if(!word_check(dictionary, "roundd-about"))
        printf("Misspelled word: %s\n", "roundd-about");
    if(!word_check(dictionary, "round"))
        printf("Misspelled word: %s\n", "round");
    if(!word_check(dictionary, "round-about-around"))
        printf("Misspelled word: %s\n", "round-about-around");
    if(!word_check(dictionary, "wHAT"))
        printf("Misspelled word: %s\n", "wHAT");
    if(!word_check(dictionary, "MACDONALD"))
        printf("Misspelled word: %s\n", "MACDONALD");
    if(!word_check(dictionary, "MacDonald"))
        printf("Misspelled word: %s\n", "MacDonald");
    if(!word_check(dictionary, "macDonald"))
        printf("Misspelled word: %s\n", "macDonald");
    if(!word_check(dictionary, "macdonald"))
        printf("Misspelled word: %s\n", "macdonald");
    if(!word_check(dictionary, "AAA"))
        printf("Misspelled word: %s\n", "AAA");
    
    return 1;
}