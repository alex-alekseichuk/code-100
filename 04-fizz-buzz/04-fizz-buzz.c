#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void fizz_buzz(uint32_t n) {
    for (uint32_t i = 1; i <= n; i++) {
        if (i % 3 == 0 && i % 5 == 0) {
            printf("FizzBuzz\n");
        } else if (i % 3 == 0) {
            printf("Fizz\n");
        } else if (i % 5 == 0) {
            printf("Buzz\n");
        } else {
            printf("%u\n", i);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No argument provided\n");

        return 1;
    }

    int n = atoi(argv[1]);
    fizz_buzz(n);

    return 0;
}

