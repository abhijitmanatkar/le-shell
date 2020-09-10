#include "history.h"
#include "headers.h"
#include <fcntl.h>

char HISTORY[MAX_STORED_HISTORY][MAX_COMMAND_LENGTH];
int HIST_CURR = -1;
int HIST_STRT = 0;

extern char HOMEDIR[DIRNAME_SZ];

int history_size(){
    if(HIST_CURR - HIST_STRT == -1 && HIST_CURR != -1) return MAX_STORED_HISTORY;
    return ((HIST_CURR - HIST_STRT) + 1 + MAX_STORED_HISTORY+1) % (MAX_STORED_HISTORY+1);
}

int history_index(int n){
    /* Returns index corresponding to nth history entry.
    Assuing n <= history_size() */
    
    return (HIST_STRT + n) % MAX_STORED_HISTORY; 
}

void show_history(int argc, char* argv[]){
    // Prints history to the terminal
    
    if(argc > 2){
        fprintf(stderr, "history: too many arguments\n");
        return;
    }

    int N;
    if(argc == 1) N = 10;
    else N = atoi(argv[1]);
    int sz = history_size();
    int n = (int) ((N <= sz) ? N : sz);

    for(int i = 0; i < n; i++){
        printf("%s\n", (char*)(HISTORY + history_index(history_size() - n + i)));
    }

}

void add_to_history(char* cmd_str){
    // add the command to history

    int inc_st = 0;     // flag for identifying whether to increment HIST_STRT ir not
    if(history_size() == MAX_STORED_HISTORY){
        inc_st = 1;
    }
    strcpy((char*) (HISTORY + history_index(history_size())), cmd_str);
    HIST_CURR = (HIST_CURR + 1) % MAX_STORED_HISTORY;
    if(inc_st) HIST_STRT = (HIST_STRT + 1) % MAX_STORED_HISTORY;
}

void store_history(){
    // write history to the file
    
    char* write_str;
    char path[DIRNAME_SZ];
    strcpy(path, HOMEDIR);
    strcat(path, "/history.txt");

    FILE* fp;
    fp = fopen(path, "w+");
    if(fp == NULL) {
        fprintf(stderr, "history: couldn't open file for writing\n");
    }

    for(int i = 0; i < history_size(); i++){
        fprintf(fp, "%s\n", (char*)(HISTORY + history_index(i)));
    }
    fclose(fp);

}

void load_history(){
    // load history from the file

    char path[DIRNAME_SZ];
    strcpy(path, HOMEDIR);
    strcat(path, "/history.txt");
    
    FILE* fp;

    size_t INPBUF_SZ = 4096;
    char* cmd = NULL;

    fp = fopen(path, "a+");
    if(fp == NULL) {
        return;
    }

    HIST_STRT = 0;
    HIST_CURR = -1;
    int sz;
    while((sz = getline(&cmd, &INPBUF_SZ, fp)) != -1 && history_size() < MAX_STORED_HISTORY){
        cmd[sz - 1] = '\0';
        strcpy((char*)(HISTORY + history_index(history_size())), cmd);
        HIST_CURR = (HIST_CURR + 1) % MAX_STORED_HISTORY;
    }
    fclose(fp);

}