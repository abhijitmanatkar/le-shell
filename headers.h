#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// Constants
#define MAX_COMMANDS 256
#define MAX_ARGS 512
#define DIRNAME_SZ 1024
#define MAX_FILES_IN_DIR 65536