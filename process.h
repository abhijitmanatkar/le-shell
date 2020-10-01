#ifndef __PROCESS_H
#define __PORCESS_H

typedef struct process{
    int pid;
    int job_num;
    char name[256];
    struct process* next;
} process;

void print_processes(process* head);

process* add_process(int pid, char* name, process* head);

process* del_process_by_pid(int pid, process* head);

process* del_process_by_job_num(int job_num, process* head);

process* get_process_by_job_num(int job_num, process* head);

process* get_process_by_pid(int pid, process* head);

#endif