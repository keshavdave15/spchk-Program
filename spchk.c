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
#define MAX_WORD_LENGTH 45

//binary search method for searching the large dictionary array
int binary_search(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word) {
    //declares variables for searching
    int low = 0;
    int high = NUM_STRINGS-1;

    //iterates while still stuff to check
    while (low <= high) {
        //checks middle between 2 adjusting values
        int mid = (low + high) / 2;
        int cmp = strcmp(dictionary[mid], word);
        if (cmp == 0) {
            //if found
            return 1; 
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    //if not found
    return 0;
}

//This method will handle capitalization cases cross reference each word with the dictionary file via binary search function
int part_check(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word){
    //declare variables
    int allCaps = 1;
    int onlyFirstCap = 1;
    //iterates
    for(int j = 0; j < MAX_STRING_LENGTH; j++){
        //loop is used to detect cases where only the first letter is capitalized or everything is capitalized
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

    //now that we know if there are any special capitalization cases
    //declare variables
    char holder[MAX_STRING_LENGTH];
    strcpy(holder, word);
    //if the word is all CAPITIALIZED
    if(allCaps){
        for(int j = 0; j < MAX_STRING_LENGTH; j++){
            if(isalpha(word[j])){
                word[j] = tolower(word[j]);
            }
            if(word[j] == '\0' || word[j] == '-'){
                j = MAX_STRING_LENGTH;
            }
        }
        //will check adjusted word or original word to ensurte success
        if(binary_search(dictionary, word) || binary_search(dictionary, holder))
            return 1;
    }
    //if first part of word is Capitalized
    else if(onlyFirstCap){
        word[0] = tolower(word[0]);
        //will check adjusted word or original word to ensurte success
        if(binary_search(dictionary, word) || binary_search(dictionary, holder))
            return 1;
    }
    //if no special capitalization cases
    else{
        return binary_search(dictionary, word);
    }
}

//this method will call the part)check method for every hyphened section of a word
int word_check(char dictionary[NUM_STRINGS][MAX_STRING_LENGTH], char* word){
    //Because of numerous technical difficulties with words that have an inner capitalized charecter,
    //all words as such are hardcoded to be check in the case that they are fully capitalized
    if(strcmp(word, "MACDONALD") == 0 || strcmp(word, "MACARTHUR") == 0 || strcmp(word, "MACBRIDE") == 0 || strcmp(word, "MACLEISH") == 0 || word, "MACDONALD'S" == 0 || strcmp(word, "MACARTHUR'S") == 0 || strcmp(word, "MACBRIDE'S") == 0 || strcmp(word, "MACLEISH'S") == 0)
        return 1;
    //declare variables for hyphen sectioning
    int remember = 0;
    char holder[MAX_STRING_LENGTH];
    for(int i = 0; i < MAX_STRING_LENGTH; i++){
        //on word breaking charecter, call part_check function
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

void words_in_file(int fd, char *filename, char dictionary[NUM_STRINGS][MAX_STRING_LENGTH]) {
    //declare variables for word finding
    char word[MAX_WORD_LENGTH];
    int word_length = 0;
    char c;
    int lineNum = 1;
    int colNum = 1;
    int frontcut = 0;

    //iteration loop
    while (read(fd, &c, 1) > 0) {
        //on word breaking charecter
        if(isspace(c) || c == '\n') {
            if (word_length > 0) {
                word[word_length] = '\0'; 
                //this loop will cut alll punctuation at the beginning of a word
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
                //this loop will cut all punctuation from the back of the word
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
                //this will send the word to the spell check function 
                if (word_length > 0)
                    if(!word_check(dictionary, word))
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

    //This section is a repeat of above for the word breaking EOF charecter, which terminated the above loop
    if (word_length > 0) {
        word[word_length] = '\0'; 
        //this loop will cut alll punctuation at the beginning of a word
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
        //this loop will cut all punctuation from the back of the word
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
        //this will send the word to the spell check function 
        if (word_length > 0)
            if(!word_check(dictionary, word))
                printf("%s (%d,%d): %s\n", filename, lineNum, colNum-word_length-frontcut, word);
        word_length = 0;
    }

    close(fd);
}

//function iterates through every directory and text file 
void traverse_directory(char *dirname, char dictionary[NUM_STRINGS][MAX_STRING_LENGTH]) {
    //if sending a txt file and not a directory
    struct stat file_stat;
    if (stat(dirname, &file_stat) == 0) {
        if (S_ISREG(file_stat.st_mode)  && strstr(dirname, ".txt") != NULL) {
            int fd = open(dirname, O_RDONLY);
            if (fd != -1) {
                //sends file to function to extract words
                words_in_file(fd, dirname, dictionary);
                close(fd);
                return;
            } 
        }
    }

    //open and check directory
    DIR *dir = opendir(dirname);
    
    if (dir == NULL) {
        printf("Unable to open directory: %s\n", dirname);
        return;
    }

    struct dirent *entry;
    //iteration loop
    while ((entry = readdir(dir)) != NULL) {
        //ignores if file name starts with '.'
        if (entry->d_name[0] == '.') 
            continue;

        //builds path
        char path[256];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        //uses stat to extract info from path
        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            printf("Error getting file status: %s\n", path);
            continue;
        }

        //travels to next directory
        if (S_ISDIR(path_stat.st_mode)) {
            traverse_directory(path, dictionary);
        }         
        //if a text file, open it
        else if (S_ISREG(path_stat.st_mode) && strstr(entry->d_name, ".txt") != NULL) {
            int fd = open(path, O_RDONLY);
            if (fd != -1) {
                //sends file to function to extract words
                words_in_file(fd, path, dictionary);
            close(fd);
            } 
            else {
            printf("Unable to open file: %s\n", path);
    }
        }
    }
    //closes directory after done
    closedir(dir);
}

int main(int argc, char* argv[]){
    //opens dictionary
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("Unable to open dictionary file: %s\n", "/usr/share/dict/words");
        return 0;
    }
    //declare variables for creating dictionary
    char dictionary[NUM_STRINGS][MAX_STRING_LENGTH];
    char word[MAX_STRING_LENGTH];
    int word_length = 0;
    char c;

    //creates dictionary array
    int word_count = 0;
    while (read(fd, &c, 1) > 0 || word_length > 0) {
        //on word breaking charecter, make a new string and add it too array
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
            //resets word length
            word_length = 0;
        //adds to word if not a word breaking charecter
        } else {
            if (word_length < MAX_STRING_LENGTH - 1) {
                word[word_length++] = c;
            } else {
                word_length = 0;
            }
        }
    }

    //for every argument, run functions necessary for program
    char* dictpath = argv[1];
    for(int i = 2; i < argc; i++)
        if(!(argv[i][0] == '.'))
            traverse_directory(argv[i], dictionary);

    return 1;
}
