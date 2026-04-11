/*
Find the sum of all the primes below two million:
./a.out 2000000 | awk '{s += $1} END {print s}'
142913828922
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sieve_eratos(unsigned char *nums, unsigned int n) {
    memset(nums, 0, n + 1);
    for (unsigned int i = 2; i*i < n; i++) {
        if (!nums[i])
            for (unsigned int j = i * i; j <= n; j += i)
                nums[j] = 1;
    }
}

int main(int argc, char *argv[]) {
    unsigned int n;

    if (argc > 1) {
        n = atoi(argv[1]);
    } else {
        printf("Введите целое число: ");
        scanf("%u", &n);
    }

    unsigned char nums[n + 1];
    sieve_eratos(nums, n);

    for (unsigned int i = 2; i <= n; i++)
        if (!nums[i])
            printf("%u\n", i);

    return 0;
}

