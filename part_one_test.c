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

void traverse_directory(char *dirname) {
    struct stat file_stat;
    if (stat(dirname, &file_stat) == 0) {
        if (S_ISREG(file_stat.st_mode)  && strstr(dirname, ".txt") != NULL) {
            int fd = open(dirname, O_RDONLY);
            if (fd != -1) {
                printf("Opened file: %s\n", dirname);
                close(fd);
                return;
            } 
        }
    }

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        printf("Unable to open directory: %s\n", dirname);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') 
            continue;

        char path[256];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            printf("Error getting file status: %s\n", path);
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            if(path[0] == '.')
                continue;
            traverse_directory(path);
        } 
        else if (S_ISREG(path_stat.st_mode) && strstr(entry->d_name, ".txt") != NULL) {
            int fd = open(path, O_RDONLY);
            if (fd != -1) {
                printf("Opened file: %s\n", path);
            close(fd);
            } 
            else {
            printf("Unable to open file: %s\n", path);
    }
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
    for(int i = 1; i < argc; i++)
        if(!(argv[i][0] == '.'))
            traverse_directory(argv[i]);
    return 1;
}