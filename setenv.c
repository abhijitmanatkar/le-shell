#include "setenv.h"
#include "headers.h"

void check_env(char* name){
    char* env_val = getenv(name);
    printf("$%s: %s\n", name, env_val);
}

int set_env(int argc, char* argv[]){
    
    if(argc == 1 || argc > 3){
        fprintf(stderr, "setenv: incorrect number of arguments");
        return 1;
    }

    if(argc == 2) argv[2] = "";

    if(setenv(argv[1], argv[2], 1) < 0){
        perror("setenv");
        return 1;
    }
    //check_env(argv[1]);
    return 0;
}

int unset_env(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "unsetenv: incorrect number of arguments\n");
        return 1;
    }

    if(unsetenv(argv[1]) < 0){
        perror("setenv");
        return 1;
    }
    //check_env(argv[1]);
    return 0;
}
