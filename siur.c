#include <dirent.h>
#include <errno.h>
#include <fcntl.h> // Zawiera funkcje open i flagi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h> // tolower i inne
#include <unistd.h> // getopt, close

#define MAX_PATH 256
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void count_letters(const char *str, int *counts) {
    for (int i = 0; i < 26; i++)
        counts[i] = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            char letter = tolower(str[i]);
            counts[letter - 'a']++;
        }
    }
}

int is_palindrom(const char *str) {
    int left = 0;
    int right = strlen(str) - 1;
    while (left < right) {
        while (left < right && !isalpha(str[left])) left++;
        while (left < right && !isalpha(str[right])) right--;
        if (tolower(str[left]) != tolower(str[right]))
            return -1; // Nie jest palindromem
        left++;
        right--;
    }
    return 0; // Success
}

int check_subs_tab(const char *name, const char **patterns, int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(name, *(patterns + i)))
            return 0;
    }
    return -1;
}

int check_substring(const char *name, const char *pattern) {
    return (strstr(name, pattern) != NULL) ? 0 : -1;
}

int check(char *name, char pattern) {
    for (int i = 0; i < strlen(name); i++) {
        if (pattern == name[i]) {
            return 0;
        }
    }
    return -1;
}

void usage(char *pname) {
    fprintf(stderr, "USAGE: %s -p dirname [-o filename]\n", pname);
    exit(EXIT_FAILURE);
}

void scan_dir(int fd) { // Zmieniamy na użycie deskryptora pliku
    const char *patterns[] = {"kutas", "cip", "fiut", "siur", "chuj"};
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    if ((dirp = opendir(".")) == NULL)
        ERR("opendir");

    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            if (lstat(dp->d_name, &filestat))
                ERR("lstat");
            if (check_subs_tab(dp->d_name, patterns, 5)) {
                char buffer[512];
                snprintf(buffer, sizeof(buffer), "%s %ld\n", dp->d_name, filestat.st_size);
                if (write(fd, buffer, strlen(buffer)) == -1)
                    ERR("write");
            }
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
}

int main(int argc, char **argv) {
    char path[MAX_PATH];
    int c;
    char *filename = NULL;
    int count = 0;

    char **dirs = malloc(sizeof(char*) * (argc - 1));
    if (dirs == NULL)
        ERR("malloc");

    int i = 0;

    while ((c = getopt(argc, argv, "p:o:")) != -1) {
        switch (c) {
            case 'p':
                dirs[i] = optarg;
                i++;
                break;
            case 'o':
                filename = optarg;
                count++;
                break;
            case '?':
            default:
                usage(argv[0]);
        }
    }
    if (count > 1)
        usage(argv[0]);

    int fd = STDOUT_FILENO; // Domyślnie ustawiamy na standardowe wyjście
    if (filename != NULL) {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            ERR("open");
    }

    if (getcwd(path, MAX_PATH) == NULL)
        ERR("getcwd");

    for (int j = 0; j < i; ++j) {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "SCIEZKA: %s\nLISTA PLIKOW:\n", dirs[j]);
        if (write(fd, buffer, strlen(buffer)) == -1)
            ERR("write");

        if (chdir(dirs[j]))
            ERR("chdir");

        scan_dir(fd); // Przekazujemy deskryptor pliku

        if (chdir(path))
            ERR("chdir");
    }

    if (fd != STDOUT_FILENO) // Zamykamy plik, jeśli nie jest to stdout
        close(fd);

    free(dirs);
    return EXIT_SUCCESS;
}