#include "headers.h"
#include "init.h"
#include "prompt.h"
#include "formatpath.h"
#include "history.h"
#include "parse.h"
#include "process.h"

extern int errno;

char HOMEDIR[DIRNAME_SZ];
char LASTDIR[DIRNAME_SZ];

char* command_list[MAX_COMMANDS];
char* arg_list[MAX_ARGS];

int shell_terminal;
process* PROCESSES;

size_t INPBUF_SZ = 4096;
char* inpbuf;

int EXIT_STATUS;

int main(){

    init();

    while(1){

        prompt();
        
        // get input
        int inp_size = getline(&inpbuf, &INPBUF_SZ, stdin);
        if(inp_size < 1) quit();
        inpbuf[strlen(inpbuf) - 1] = '\0';

        // seperating commands
        char* delim = ";";
        char* command_str;
        command_str = strtok(inpbuf, delim);
        int command_no = 0;
        while(command_str != NULL && command_no < MAX_COMMANDS){
            command_list[command_no] = command_str;
            add_to_history(command_str);
            command_str = strtok(NULL, delim);
            command_no++;
        }

        for(int i = 0; i < command_no; i++){
            EXIT_STATUS = parse_pipe(command_list[i]);
        }
    }

    return 0;
}
