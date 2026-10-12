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

#define MAX_DENOMINATOR 30000

int main() {
    int n, m;

    printf("Enter a common fraction as n/m: ");
    // Use a more robust check for the input format
    if (scanf("%d/%d", &n, &m) != 2 || n <= 0 || m <= 0 || n > m) {
        fprintf(stderr, "Incorrect input. Please enter a positive fraction where n < m.\n");
        exit(1);
    }

    printf("%d/%d = ", n, m);

    int first_term = 1;
    int k = 2;

    while (n > 0 && k < MAX_DENOMINATOR) {
        // Find the smallest k such that 1/k <= n/m
        // This is equivalent to finding the smallest k such that k >= m/n
        while (k < MAX_DENOMINATOR && m > n * k) {
            k++;
        }

        if (!first_term) {
            printf(" + ");
        }
        
        printf("1/%d", k);
        first_term = 0;

        // Update the remaining fraction: (n/m) - (1/k) = (nk - m) / mk
        n = (k * n) - m;
        m = m * k;
    }

    printf("\n");
    return 0;
}
