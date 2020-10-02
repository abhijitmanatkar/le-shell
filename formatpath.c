#include "formatpath.h"
#include "headers.h"

extern char HOMEDIR[DIRNAME_SZ];
extern char LASTDIR[DIRNAME_SZ];

char retstrTR[DIRNAME_SZ];    // return string for to_rel()
char retstrTA[DIRNAME_SZ];    // retrun string for to_abs()

char* to_rel(char* path){
    // Converts absolute paths to paths with ~

    char* temp;
    /* Storing the start pointer of substring = HOMEDIR if found in dirname.
    Stores NULL otherwise. */
    temp = strstr(path, HOMEDIR);

    if(temp != NULL){
        //char* retstr = (char*) malloc(1024);
        strcpy(retstrTR, "~");
        strcat(retstrTR, temp + strlen(HOMEDIR));
        return retstrTR;
    }
    
    return path;
}

char* to_abs(char* path){
    /* Converts paths containing ~, .. or . to absolute paths. (Only works 
    for paths having ~, .. or . at the start) */
    
    if(path[0] == '.'){
        getcwd(retstrTA, DIRNAME_SZ);
        if(path[1] == '.'){
            int last_slash = strlen(retstrTA) - 1;
            while(retstrTA[last_slash] != '/'){
                last_slash--;
            }
            retstrTA[last_slash] = '\0';
            strcat(retstrTA, path + 2);
            return retstrTA;
        }
        strcat(retstrTA, path + 1);
        return retstrTA;
    }

    else if(path[0] == '~'){
        strcpy(retstrTA, HOMEDIR);
        strcat(retstrTA, path + 1);
        return retstrTA;
    }

    return path;
}