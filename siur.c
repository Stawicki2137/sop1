#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h> //tolower i takie tam
#include <unistd.h> //getopt

#define MAX_PATH 256
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void count_letters(const char *str, int *counts){
    for(int i=0; i<26; i++)
    counts[i]=0;

    for(int i=0; str[i]!='\0'; i++){
        if(isalpha(str[i])){
            char letter = tolower(str[i]);
            counts[letter - 'a']++;
        }
    }
}

int is_palindrom(const char *str){
    int left=0;
    int right=strlen(str)-1;
    while(left<right){
    while(left<right && !isalpha(str[left])) left++;
    while(left<right && !isalpha(str[right])) right++;
    //powyzej wypierdalam wszystkie nie litery 
    if(tolower(str[left])!=tolower(str[right]))
    return -1; //nie jest palindroemm
    left++;
    right++;
    }
    return 0; //success
}

int check_subs_tab(const char* name,const char** patterns, int count){
    for(int i=0; i<count; i++) {
        if(strstr(name,*(patterns+i)))
        return 0; 
    }
    return -1;
}

int check_substring(const char* name, const char* pattern){
    if(strstr(name,pattern)!=NULL)
    return 0;
    return -1;
}
int check(char*name, char pattern){
    int ans=-1;
    for(int i=0; i<strlen(name); i++){
        if(pattern==name[i])
        {
            return ans=0;
        }
    }
    return ans;
}
void usage(char *pname)
{
    fprintf(stderr, "USAGE:%s -p dirname\n", pname);
    exit(EXIT_FAILURE);
}
void scan_dir(FILE*f)
{
    const char *patterns[] = {"kutas", "cip", "fiut", "siur", "chuj"};
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    if ((dirp = opendir(".")) == NULL)
        ERR("opendir");
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if(lstat(dp->d_name,&filestat))
            ERR("lsat");
            if(check_subs_tab(dp->d_name,patterns,5))
            fprintf(f,"%s %ld\n", dp->d_name,filestat.st_size);
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
   
}

int main(int argc, char **argv)
{
   // char name[20];
   // strcpy(name,".");

    char path[MAX_PATH];
    int c;
    char * filename=NULL;
    int count=0;

    char **dirs=malloc(sizeof(char*)*(argc-1));
    int i=0;

    while((c=getopt(argc,argv,"p:o:"))!=-1){
        switch (c){
            case 'p':
            *(dirs+i)=optarg;
            i++;
            break;
            case 'o':
            filename=optarg;
            count++;
            break;          
            case '?':
            default:
            usage(*(argv));
        }
    }
    if(count>1) usage(*(argv));
     FILE *f=stdout;
     printf("%d",i);
    if(filename!=NULL){
        if((f=fopen(filename,"w"))==NULL)
        ERR("fopen");
     printf("%s",filename);

    }
       
        if((getcwd(path,MAX_PATH))==NULL)
        ERR("getcwd");
    
        for(int j=0; j<i; ++j){
            fprintf(f,"SCIEZKA:%s\nLISTA PLIKOW:\n",dirs[j]);
            if(chdir(dirs[j]))
            ERR("chdir");
            scan_dir(f);
            if(chdir(path))
            ERR("chdir");
        }

    if(fclose(f))
    ERR("fclose");
    free(dirs);
    return EXIT_SUCCESS;
}