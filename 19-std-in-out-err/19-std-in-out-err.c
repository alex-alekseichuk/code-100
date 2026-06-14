#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *line = NULL;
    size_t size = 0;

    if (getline(&line, &size, stdin) == -1) {
        return 1;
    }

    line[strcspn(line, "\r\n")] = '\0';

    printf("%s\n", line);
    fprintf(stderr, "%s\n", line);

    free(line);

    return 0;
}
