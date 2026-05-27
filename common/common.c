#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

size_t read_line(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) return -1;

    size_t size = (*lineptr && *n > 0) ? *n : 128;

    if (*lineptr == NULL) {
        *lineptr = malloc(size);
        if (!*lineptr) return -1;
        *n = size;
    }

    size_t len = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {
        if (len + 1 >= *n) {
            size_t new_size = *n * 2;
            char *tmp = realloc(*lineptr, new_size);
            if (!tmp) return -1;

            *lineptr = tmp;
            *n = new_size;
        }

        (*lineptr)[len++] = (char)c;

        if (c == '\n')
            break;
    }

    if (len == 0 && c == EOF)
        return -1;

    (*lineptr)[len] = '\0';
    return (size_t)len;
}

size_t parse_line(char *str, char *tokens[], size_t *count, const char *delimiters) {
    size_t _count = 0;

    char *token = strtok(str, delimiters);
    while (token != NULL && _count < *count) {
        tokens[_count++] = token;
        token = strtok(NULL, delimiters);
    }
    *count = _count;

    return _count;
}

void print_log(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vfprintf(stderr, format, args);

    fprintf(stderr, "\n");

    va_end(args);
}
