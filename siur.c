#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h> //tolower i takie tam

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
void scan_dir()
{
    const char *patterns[] = {"kutas", "cip", "fiut", "siur"};
    DIR *dirp;
    struct dirent *dp;
    if ((dirp = opendir(".")) == NULL)
        ERR("opendir");
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            
            if(check_subs_tab(dp->d_name,patterns,4))
            fprintf(stdout,"%s\n", dp->d_name);
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
   
}

int main(int argc, char **argv)
{
    char name[20];
    strcpy(name,".");
    fprintf(stdout,"KATALOG:%s\nLISTA PLIKOW\n",name);
    scan_dir();
    return EXIT_SUCCESS;
}