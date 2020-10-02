#include "cd.h"
#include "headers.h"
#include "formatpath.h"
#include <errno.h>

extern int errno;

extern char HOMEDIR[DIRNAME_SZ];
extern char LASTDIR[DIRNAME_SZ];

int cd(int argc, char* argv[]){
    // changes current working directory to specified argument
    char lastdir_tmp[DIRNAME_SZ];

    if(argc > 2){
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    if(argc == 1){
        // change to home directory if no arguments are given
        getcwd(lastdir_tmp, DIRNAME_SZ);
        if(chdir(HOMEDIR) < 0){
            perror("cd");
            return 1;
        }
        strcpy(LASTDIR, lastdir_tmp);
        return 0;
    }
    char * path_abs;
    if(strcmp("-", argv[1]) == 0){
        path_abs = LASTDIR;
    }
    else path_abs = to_abs(argv[1]);
    getcwd(lastdir_tmp, DIRNAME_SZ);
    if(chdir(path_abs) < 0){
        perror("cd");
        return 1;
    }
    strcpy(LASTDIR, lastdir_tmp);
    return 0;
}