#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return 0;
    }

    while (--argc > 0) {
        printf("%s\n", *++argv);
    }

    return 0;
}
