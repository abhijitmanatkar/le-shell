#include "ls.h"
#include "headers.h"
#include "formatpath.h"
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

DIR* dir_read;
struct dirent* dir_entry;
char* filenames[MAX_FILES_IN_DIR];

char* MONTHS[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int get_term_width(){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
}

void print_permissions(struct stat* sb){
    // print permissions for a file corresponding to the stat buffer sb in the -rwxrwxrwx format

    switch(sb->st_mode & S_IFMT){
        case S_IFSOCK:
            printf("s");
            break;
        case S_IFLNK:
            printf("l");
            break;
        case S_IFREG:
            printf("-");
            break;
        case S_IFBLK:
            printf("b");
            break;
        case S_IFDIR:
            printf("d");
            break;
        case S_IFCHR:
            printf("c");
            break;
        case S_IFIFO:
            printf("p");
            break;
        default:
            printf("?");
            break;
    }
    int permissions[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    char rwx[] = {'r', 'w', 'x'};
    for(int i = 0; i < 9; i++){
        if(sb->st_mode & permissions[i]) printf("%c", rwx[i % 3]);
        else printf("-");
    }
}

void print_files(char* dirname, int L_FLAG, int A_FLAG){
    // print files in a single directory

    struct stat sb;
    char filename[DIRNAME_SZ];
    int fid;
    int n_files = 0;
    int max_name_len = 1;
    int blk_cnt = 0;
    
    // going over all files in the directory
    dir_entry = readdir(dir_read);
    while(dir_entry != NULL){
        // handling hidden files
        if(dir_entry->d_name[0] == '.' && A_FLAG == 0){
            dir_entry = readdir(dir_read);
            continue;
        }
        filenames[n_files] = dir_entry->d_name;

        // Count number of blocks occupied by each file. Used if L_FLAG is set.
        strcpy(filename, dirname);
        strcat(filename, "/");
        strcat(filename, filenames[n_files]);
        if(stat(filename, &sb) >= 0){
            blk_cnt += sb.st_blocks;
        }
        
        int name_len = strlen(dir_entry->d_name);
        // getting maximum name length for formatting in columns later
        if(name_len > max_name_len) max_name_len = name_len;
        n_files++;
        dir_entry = readdir(dir_read);
    }
    closedir(dir_read);
    
    // L_FLAG not set, print just names in columns
    if(L_FLAG == 0){
        int term_width = get_term_width();
        int n_cols = (term_width + 1) / (max_name_len + 1);
        fid = 0;
        while(fid < n_files){
            for(int i = 0; i < n_cols - 1 && fid < n_files; i++){
                printf("%-*s", max_name_len + 1, filenames[fid]);
                fid++;
            }
            if(fid < n_files){
                printf("%-*s", max_name_len, filenames[fid]);
                fid++;
            }
            printf("\n");
        }
        return;
    }
    
    // L_FLAG is set
    struct passwd* pwd;
    struct group* grp;
    struct tm* last_mod_time;

    printf("total %d\n", blk_cnt / 2);
    for(fid = 0; fid < n_files; fid++){
        
        // getting actual file path
        strcpy(filename, dirname);
        strcat(filename, "/");
        strcat(filename, filenames[fid]);
        if(stat(filename, &sb) < 0){
            //perror(filenames[fid]);
            continue;
        }
        
        print_permissions(&sb);
        printf("%6ld ", sb.st_nlink);
        
        // owner name
        pwd = getpwuid(sb.st_uid);
        printf("%-8s ", pwd->pw_name);
        
        // group name
        grp = getgrgid(sb.st_gid);
        printf("%-8s ", grp->gr_name);

        // file size
        printf("%10ld ", sb.st_size);

        // last time of modification
        last_mod_time = localtime(&(sb.st_mtim.tv_sec));
        printf("%s %2d %2.2d:%2.2d ", MONTHS[last_mod_time->tm_mon], last_mod_time->tm_mday, last_mod_time->tm_hour, last_mod_time->tm_min);

        // file name
        printf("%s\n", filenames[fid]);
    }
}

int ls(int argc, char* argv[]){
    int A_FLAG = 0;
    int L_FLAG = 0;

    char* dir_args[512];
    int dir_args_no = 0;
    
    // seperate directory arguments and set flags
    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            for(int j = 0; j < strlen(argv[i]); j++){
                if(argv[i][j] == 'a') A_FLAG = 1;
                if(argv[i][j] == 'l') L_FLAG = 1;
            }
        }
        else{
            dir_args[dir_args_no] = argv[i];
            dir_args_no++;
        }
    }

    // if no directory arguments provided, use curent directory
    if(dir_args_no == 0){
        char currdir[1024];
        getcwd(currdir, 1024);
        dir_args[0] = currdir;
        dir_args_no++;
    }   

    int exit_status = 0;
    for(int i = 0; i < dir_args_no; i++){
        //printf("%s\n", to_abs(dir_args[i]));
        dir_read = opendir(to_abs(dir_args[i]));
        if(dir_read == NULL){
            perror(dir_args[i]);
            exit_status = 1;
            continue;
        }
        if(i > 0) printf("\n");
        if(dir_args_no > 1) printf("%s:\n", dir_args[i]);
        print_files(to_abs(dir_args[i]), L_FLAG, A_FLAG);
    }
    return exit_status;
}
