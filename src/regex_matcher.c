#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "regex_ndiu.h"

static void list_files(void)
{
    DIR *directory = opendir("./txt");
    struct dirent *ptr = readdir(directory);
    size_t i = 1;

    for (; ptr != NULL; i++) {
        if (strncmp(ptr->d_name, ".", 1) == 0) {
            continue;
        }
        printf("[%ld] %s\n", i, ptr->d_name);
        ptr = readdir(directory);
    }
}

static size_t get_file_size(char *filename)
{
    struct stat fileStat;

    if (stat(filename, &fileStat) != 0) {
        perror(filename);
        return 1;
    }
    return fileStat.st_size;
}

static unsigned int *review_file(char *filename, char **keywords)
{
    char **tokns = tokn_array(filename, ' ');
    size_t size = get_array_size(keywords);
    size_t j = 0;
    unsigned int *count = malloc(sizeof(unsigned int *) * size);

    for(size_t i = 0; tokns[i]; i++) {
        for(; keywords[j]; j++) {
            if (strcmp(tokns[i], keywords[j]) == 0) {
                count[j] += 1;
            }
        }
        j = 0;
    }
    return count;
}

static void dump_results(char **keywords, unsigned int *match, size_t i)
{
    printf("index [%ld] :\n", i);
    for(size_t j = 0; keywords[i]; i++) {
        printf("\tFound %u matches for the word %s\n", match[i], keywords[j]);
    }
}

static unsigned int **auto_review(unsigned int begin, unsigned int end,  char **keywords)
{
    DIR *directory = opendir("./txt");
    struct dirent *ptr = readdir(directory);
    char *buff = NULL;
    size_t size = 0;
    int fd = 0;
    size_t file_nbr = end - begin;
    unsigned int **matches = malloc(sizeof(unsigned int *) * file_nbr + 1);

    for(size_t i = 1; ptr != NULL; i++) {
        ptr = readdir(directory);
        while (i >= begin && i <= end) {
            fd = open(ptr->d_name, S_IRWXU);
            size = get_file_size(ptr->d_name);
            buff = malloc(sizeof(char) * size + 1);
            read(fd, buff, size);
            matches[i] = review_file(buff, keywords);
            dump_results(keywords, matches[i], i);
            free(buff);
        }
    }
    return matches;
}

static char **get_keywords(void)
{
    char *config_path = ".config/keywords.txt";
    int fd = open(config_path, S_IRWXU);
    size_t size = get_file_size(config_path);
    char *buff = malloc(sizeof(char) * size + 1);
    char **keywords = NULL;

    read(fd, buff, size);
    keywords = tokn_array(buff, ' ');
    return keywords;
}

void regex_matcher(void)
{
    char *indexes = 0;
    char **keywords = get_keywords();
    unsigned int begin_index = 0;
    unsigned int end_index = 0;
    printf("Welcome to the N.D.I.U Regex matcher\n");
    list_files();
    printf("Please enter the index(es) of the file you want to review automatically.(ex 1, 1-15)\nindex:");
    while (1) {
        scanf("%s", indexes);
        if (strlen(indexes) != 1) {
            end_index = atoi(indexes + 2);
        }
        begin_index = atoi(indexes);
        auto_review(begin_index, end_index, keywords);
    }
}
