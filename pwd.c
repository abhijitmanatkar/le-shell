#include "pwd.h"
#include "headers.h"

int pwd(int argc){
    // print current working directory

    if(argc > 1){
        printf("pwd: too many arguments\n");
        return 1;
    }
    char dirname[1024];
    getcwd(dirname, 1024);
    printf("%s\n", dirname);

    return 0;
}