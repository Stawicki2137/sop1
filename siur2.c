#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH 101

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void usage(char *pname)
{
    fprintf(stderr, "USAGE:%s -p NAME \n", pname);
    exit(EXIT_FAILURE);
}

int check_substring(const char* name, const char* pattern){
    if(strstr(name,pattern)!=NULL)
    return 0;
    return -1;
}

void scan_dir(FILE * f, char* extension) {
    DIR* dirp;
    struct dirent *dp;
    struct stat filestat;

    if((dirp=opendir("."))==NULL)
    ERR("opendir");

    do
    {
        errno=0;
        if((dp=readdir(dirp))!=NULL){
        
            if(lstat(dp->d_name,&filestat))
            ERR("lsstat");
            if(S_ISREG(filestat.st_mode))
            {
                if(extension!=NULL){
                if(check_substring(dp->d_name,extension)==0)
            fprintf(f,"%s %ld\n",dp->d_name,filestat.st_size);
                }
                else {
            fprintf(f,"%s %ld\n",dp->d_name,filestat.st_size);

                }
            }
        }
        
    } while (dp!=NULL);
    if(errno!=0)
    ERR("readir");

    if(closedir(dirp))
    ERR("closedir");
}
int main(int argc, char**argv){
    char **katalogi=malloc(sizeof(char*)*(argc-1));
    int c;
    int k=0;
    char * extension=NULL;
    while((c=getopt(argc,argv,"p:e:"))!=-1)
    {
        switch(c){
            case 'p':
            katalogi[k]=optarg;
            k++;
            break;
            case 'e':
            extension=optarg;
            break;
            case '?':
            default:
            usage(*(argv));
        }
    }
        char path[MAX_PATH];
    if ((getcwd(path, MAX_PATH)) == NULL)
        ERR("getcwd");
     FILE * f=stdout;
     
         for (int i = 0; i < k; i++)
    {
        if (chdir(katalogi[i]))
            ERR("chdir");
        fprintf(f,"path:%s:\n", katalogi[i]);
        
        scan_dir(f,extension);
        if (chdir(path))
            ERR("chdir");
    }
    
    free(katalogi);
    return EXIT_SUCCESS;

}