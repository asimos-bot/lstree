#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_SIZE 4096
#define PADDING 2

void print_tree(char* basepath, unsigned int offset, int is_last) {
    DIR *directory = opendir(basepath);

    if( directory == NULL ) {
        fprintf(stderr, "\033[1m\033[31mcouldn't open\033[0m\n");
        return;
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
            print_tree(path, offset + PADDING, itemcur == nitems - 1); } else {
            printf("%s\n", item->d_name);
        }
    }
    closedir(directory);
}

void help() {
    fprintf(stderr, "usage: lstree [DIR]\n");
}

int main(int argc, char** argv) {
    char* basedirpath = argc < 2 ? "." : argv[1];
    printf("%s\n", basedirpath);
    print_tree(basedirpath, 0, 1);
    return 0;
}
