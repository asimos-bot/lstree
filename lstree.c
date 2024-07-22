#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_SIZE 4096
#define PADDING 3
#define MAX_DEPTH 1000

void print_tree(char* basepath, unsigned int depth, unsigned int* columns) {
    columns[depth] = 1;
    DIR *directory = opendir(basepath);

    if( directory == NULL ) {
        fprintf(stderr, "\033[1m\033[31mcouldn't open %s/\033[0m\n", basepath);
        return;
    }

    char path[MAX_PATH_SIZE];

    unsigned int nitems = 0;
    unsigned int current_dir_idx = 0;
    unsigned int previuos_dir_idx = 0;
    struct dirent *item;
    for(nitems = 0; (item = readdir(directory)) != NULL; nitems++) {
        if(!strcmp(item->d_name, ".")) current_dir_idx = nitems;
        if(!strcmp(item->d_name, "..")) previuos_dir_idx = nitems;
    }
    int last_idx_is_invalid = ((nitems - 1 == current_dir_idx) || (nitems - 1 == previuos_dir_idx));
    int second_last_idx_is_invalid = ((nitems - 2 == current_dir_idx) || (nitems - 2 == previuos_dir_idx));
    unsigned int last_idx = nitems - (last_idx_is_invalid) - (last_idx_is_invalid && second_last_idx_is_invalid) - 1;

    seekdir(directory, 0);
    for(unsigned int itemcur = 0; (item = readdir(directory)) != NULL; itemcur++) {

        int is_last = itemcur == last_idx;
        if(!strcmp(item->d_name, ".") || !strcmp(item->d_name, "..")) continue;

        for(unsigned int i = 0; i < depth; i++) {
            int has_column = columns[i];
            printf(has_column ? "│%*s" : " %*s", PADDING, " ");
        }

        printf(is_last ? "└── " : "├── ");
        if(is_last) {
            columns[depth] = 0;
        }
        if(item->d_type == DT_DIR) {
            printf("\033[94m\033[1m%s/\n\033[0m", item->d_name);
            strcpy(path, basepath);
            strcat(path, "/");
            strcat(path, item->d_name);
            print_tree(path, depth + 1, columns);
        } else {
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
    unsigned int columns[MAX_DEPTH] = {0};
    printf("%s\n", basedirpath);
    print_tree(basedirpath, 0, columns);
    return 0;
}
