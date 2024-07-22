#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_SIZE 1000
#define PADDING 2

int print_tree(char* basepath, unsigned int offset) {
    DIR *directory = opendir(basepath);

    if( directory == NULL ) {
        return 1;
    }

    char path[MAX_PATH_SIZE];

    unsigned int nitems = 0;
    struct dirent *item;
    for(nitems = 0; (item = readdir(directory)) != NULL; nitems++);

    seekdir(directory, 0);
    for(unsigned int itemcur = 0; (item = readdir(directory)) != NULL; itemcur++) {

        if(!strcmp(item->d_name, ".") || !strcmp(item->d_name, "..")) continue;
        for(int columns = offset; columns > 0; columns -= PADDING) {
            printf("│%*s", PADDING, " ");
        }
        printf(itemcur == nitems - 1 ? "└─ " : "├─ ");
        if(item->d_type == DT_DIR) {
            printf("\033[94m\033[1m%s/\n\033[0m", item->d_name);
            strcpy(path, basepath);
            strcat(path, "/");
            strcat(path, item->d_name);
            if( print_tree(path, offset + PADDING) ) {
                return 1;
            }
        } else {
            printf("%s\n", item->d_name);
        }
    }
    closedir(directory);
    return 0;
}

int main() {
    printf(".\n");
    return print_tree(".", 0);
}
