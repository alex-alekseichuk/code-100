/**
 * An Egyptian fraction is a mathematical representation of a positive rational number
 * as the sum of distinct unit fractions (fractions with a numerator of 1).
 * E.g., 5/6 = 1/2 + 1/3
 *
 * build:
 * gcc 16-egyptian-fraction.c ../common/common.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
    int n,m;

    printf("Enter a common fraction as n/m: ");
    int n_entered = scanf("%d/%d", &n, &m);

    if (n_entered != 2 || n > m)
    {
        fprintf(stderr, "Incorrect input\n");
        exit(1);
    }

    printf("%d/%d = ", n, m);

    int is_first_time = 1;
    int k;
    for (k=2; n>0 && k < 30000;) {
        while (m > n*k && k < 30000) k++;
        if (is_first_time) {
            is_first_time = 0;
        } else {
            printf(" + ");
        }
        printf("%d/%d", 1, k);
        n = k*n - m;
        m *= k;
    }

    return 0;
}
