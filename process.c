#include "process.h"
#include "headers.h"
#include "pinfo.h"

/*
typedef struct process{
    int pid;
    int job_num;
    char name[256];
    struct process* next;
} process;
*/

int NEXT_JOB_NUM = 1;
extern char* proc_terms[64];

process* add_process(int pid, char* name, process* head){
    
    process* new_process = (process*) malloc(sizeof(process));
    new_process->pid = pid;
    new_process->job_num = NEXT_JOB_NUM;
    NEXT_JOB_NUM++;
    strcpy(new_process->name, name);
    new_process->next = NULL;

    if(head == NULL){
        head = new_process;
        return head;
    }

    process* tmp = head;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = new_process;
    return head;
}

void print_processes(process* head){

    process* curr;
    for(curr = head; curr != NULL; curr = curr->next){
        if(get_pinfo(curr->pid, 0))
            continue;
        
        printf("[%d] ", curr->job_num);
        if(proc_terms[2][0] == 'T') printf("Stopped ");
        else printf("Running "); 
        printf("%s [%d]\n", curr->name, curr->pid);
    }
}

process* del_process_by_pid(int pid, process* head){
    
    process* to_del;
    if(head->pid == pid){
        to_del = head;
        head = to_del->next;
        free(to_del);
        return head;
    }

    process* curr = head;
    while(curr->next != NULL && (curr->next)->pid != pid){
        curr = curr->next;
    }

    if(curr->next == NULL)
        return head;

    to_del = curr->next;
    curr->next = to_del->next;
    free(to_del);
    return head;
}

process* del_process_by_job_num(int job_num, process* head){
    
    process* to_del;
    if(head->job_num == job_num){
        to_del = head;
        head = to_del->next;
        free(to_del);
        return head;
    }

    process* curr = head;
    while(curr->next != NULL && (curr->next)->job_num != job_num){
        curr = curr->next;
    }

    if(curr->next == NULL)
        return head;

    to_del = curr->next;
    curr->next = to_del->next;
    free(to_del);
    return head;
}

/*
int main(){
    
    process* head = NULL;
    
    char inp;
    while(scanf("%c", &inp)){
        if(inp == 'a'){
            char* name; int pid;
            scanf("%d", &pid);
            scanf("%s", name);
            head = add_process(pid, name, head);
        }
        else if(inp == 'd'){
            int pid;
            scanf("%d", &pid);
            head = del_process_by_pid(pid, head);
        }
        else if(inp == 'p'){
            print_processes(head);
        }
    }

}
*/