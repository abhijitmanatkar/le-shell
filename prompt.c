#include "prompt.h"
#include "headers.h"
#include "formatpath.h"

extern char HOMEDIR[1024];
extern int EXIT_STATUS;

void prompt() {
    // display the shell prompt
    char* username = getenv("USER");    // taken from environment variable
    char hostname[1024];
    char dirname[1024];
    getcwd(dirname, 1024);
    char* reldirname = to_rel(dirname);
    gethostname(hostname, 1000);
    if(EXIT_STATUS != -1){
        if(EXIT_STATUS == 0) printf(":')");
        else printf(":'(");
    }
    printf("<\033[1;32m%s@%s:\033[34m%s\033[0m> ", username, hostname, reldirname);
}
