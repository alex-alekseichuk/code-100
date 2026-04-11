#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int is_prime(unsigned long n) {
    if (n <= 1) {
        return 0; // Not prime
    }

    for (unsigned long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0; // Not prime
        }
    }
 
    return 1; // Prime
}

int main(int argc, char *argv[]) {
    int k;

    printf("Введите целое число: ");
    scanf("%d", &k);

    if (is_prime(k)) {
        printf("Это простое число.\n");
    } else {
        printf("Это не простое число.\n");
    }

    return 0;
}
