#include "echo.h"
#include "headers.h"

int echo(int argc, char* argv[]){
    // prints arguments to stdout
    
    for(int i = 1; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");
    
    return 0;
}