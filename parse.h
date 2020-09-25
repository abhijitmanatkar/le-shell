#ifndef __PARSE_H
#define __PARSE_H

int parse_pipe(char* line);
int parse_redir(char* command, int in_fd, int out_fd);

#endif