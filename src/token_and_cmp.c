#include <string.h>

static size_t get_delim_nbr(char *str, char delim)
{
    size_t count = 0;

    for (size_t i = 0; str[i]; i++) {
        if (str[i] == delim) {
            count++;
        }
    }
    return count;
}

size_t get_array_size(char **array)
{
    size_t i = 0;

    for (; array[i]; i++);
    return i;
}

char **tokn_array(char *str, char delim)
{
    size_t size = get_delim_nbr(str, delim);
    char **tokns = malloc(sizeof(char *) * size + 1);

    tokns[0] = strdup(strtok(str, delim));
    for (size_t i = 0; tokns[i]; i++) {
        tokns[i] = strdup(strtok(NULL, delim));
    }
    return tokns;
}
