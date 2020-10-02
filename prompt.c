#include "prompt.h"
#include "headers.h"
#include "formatpath.h"

extern char HOMEDIR[DIRNAME_SZ];
extern int EXIT_STATUS;

void prompt() {
    // display the shell prompt
    char* username = getenv("USER");    // taken from environment variable
    char hostname[DIRNAME_SZ];
    char dirname[DIRNAME_SZ];
    getcwd(dirname, DIRNAME_SZ);
    char* reldirname = to_rel(dirname);
    gethostname(hostname, 1000);
    if(EXIT_STATUS != -1){
        if(EXIT_STATUS == 0) printf(":')");
        else printf(":'(");
    }
    printf("<\033[1;32m%s@%s:\033[34m%s\033[0m> ", username, hostname, reldirname);
}
