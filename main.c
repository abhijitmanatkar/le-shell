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

extern int errno;

char HOMEDIR[DIRNAME_SZ];
char* command_list[MAX_COMMANDS];
char* arg_list[MAX_ARGS];

void exit_fn(){
    // store history and exit
    store_history();
    exit(0);
}

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

        // parsing commands
        delim = " ";
        char* arg_str;
        for(int i = 0; i < command_no; i++){
            arg_str = strtok(command_list[i], delim);
            int arg_no = 0;
            while(arg_str != NULL && arg_no < MAX_ARGS){
                arg_list[arg_no] = arg_str;
                arg_str = strtok(NULL, delim);
                arg_no++;
            }
            
            // built in and user defined commands
            if(strcmp(arg_list[0], "echo") == 0){
                echo(arg_no, arg_list);
            }
            else if(strcmp(arg_list[0], "pwd") == 0){
                pwd(arg_no);
            }
            else if(strcmp(arg_list[0], "cd") == 0){
                cd(arg_no, arg_list);
            }
            else if(strcmp(arg_list[0], "pinfo") == 0){
                pinfo(arg_no, arg_list);
            }
            else if(strcmp(arg_list[0], "ls") == 0){
                ls(arg_no, arg_list);
            }
            else if(strcmp(arg_list[0], "history") == 0){
                show_history(arg_no, arg_list);
            }
            else if(strcmp(arg_list[0], "exit") == 0){
                exit_fn();
            }

            // system commands
            else{
                int bg = 0;
                if(strcmp(arg_list[arg_no - 1], "&") == 0){
                    arg_no--;
                    bg = 1;
                }
                arg_list[arg_no] = NULL;
                execute(arg_list, bg);
            }
        }

    }

    return 0;
}
