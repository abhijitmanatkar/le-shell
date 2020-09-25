#include "headers.h"
#include "prompt.h"
#include "formatpath.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"
#include "execute.h"
#include "pinfo.h"
#include "ls.h"
#include "history.h"
#include "parse.h"

extern int errno;

char HOMEDIR[DIRNAME_SZ];
char* command_list[MAX_COMMANDS];
char* arg_list[MAX_ARGS];

/*
void exit_fn(){
    // store history and exit
    store_history();
    exit(0);
}
*/

int main(){

    getcwd(HOMEDIR, 1024);
    load_history();

    while(1){
        
        // define buffer for input
        size_t INPBUF_SZ = 4096;
        char* inpbuf = (char*) malloc(INPBUF_SZ);

        prompt();
        
        // get input
        getline(&inpbuf, &INPBUF_SZ, stdin);
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
            parse_pipe(command_list[i]);
        }
    }

    return 0;
}
