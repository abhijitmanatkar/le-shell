#ifndef __HISTORY_H
#define __HISTORY_H

int show_history(int argc, char* argv[]);

void load_history();

void store_history();

void add_to_history(char* cmd);

#endif