#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#define MAX_PATH 101
#define MAX_GROUPS 20

int groups_count[MAX_GROUPS + 1];

void usage(char* name)
{
    fprintf(stderr, "USAGE: %s path\n", name);
    exit(EXIT_FAILURE);
}

int check(const char* name)
{
    struct stat filestat;

    if (lstat(name, &filestat))
        ERR("lstat");

    if (S_ISDIR(filestat.st_mode))
        return 2;
    if (S_ISREG(filestat.st_mode))
        return 1;

    return 0;
}

void get_groups_count(const char* dirpath, int counts[MAX_GROUPS + 1]) { 
    printf("Wypisanie grup (TODO)\n"); 

    }

void process_file(const char* filepath, int group_count) { printf("Przetworzenie pliku z ocenami (TODO)\n"); }

void batch_process(const char* dirpath) { printf("Przetworzenie ocen w drzewie katalogow (TODO)\n"); }

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
    }

    char option[10];
    int group_size;

    printf("Option:\n");
    if (!fgets(option, sizeof(option), stdin))
    {
        fprintf(stderr,"%s","fgets");
       // ERR("fgets");
    }
    option[strcspn(option, "\n")] = 0;

    if (strcmp(option, "groups") == 0)
    {
        if (check(argv[1]) == 2)
        {
            get_groups_count(argv[1], groups_count);
        }
        else
        {
            ERR("groups: not dir");
        }
    }
    else if (strcmp(option, "process") == 0)
    {
        if (check(argv[1]) != 1)
        {
            printf("how many studetns in group:\n");
            if (scanf("%d", &group_size) != 1)
            {
                ERR("scanf");
            }
            process_file(argv[1], group_size);
        }
        else
        {
            ERR("process: not file");
        }
    }
    else if (strcmp(option, "batch") == 0)
    {
        if (check(argv[1]) == 2)
        {
            batch_process(argv[1]);
        }
        else
        {
            ERR("batch: not dir");
        }
    }
    else
    {
        ERR("wrong option");
    }

    return EXIT_SUCCESS;
}
