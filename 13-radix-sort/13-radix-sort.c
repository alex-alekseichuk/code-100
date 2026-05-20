/**
 * build:
 * gcc 13-radix-sort.c ../common/common.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../common/common.h"

void counting_sort_by_digit(int a[], int n, int exp) {
    int output[n];
    int count[10] = {0};

    // 1. Count digits
    for (int i = 0; i < n; i++) {
        int d = (a[i] / exp) % 10;
        count[d]++;
    }

    // 2. Prefix sums
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // 3. Build output (RIGHT TO LEFT!!!)
    // This makes it stable: If you go left -> right -- broken radix sort
    for (int i = n - 1; i >= 0; i--) {
        int d = (a[i] / exp) % 10;
        output[--count[d]] = a[i];
    }

    // 4. Copy sorted array back into original
    for (int i = 0; i < n; i++)
        a[i] = output[i];
}

int main() {
    char *str = NULL;
    size_t len = 0;

    printf("Enter numbers: ");
    size_t nread = read_line(&str, &len, stdin);
    
    char *tokens[100];
    size_t count = 100;

    parse_line(str, tokens, &count, " \n");
    if (count == 0 || count > 64) {
        return 1;
    }

    int nums[count];
    for (int i = 0; i < count; i++) {
        nums[i] = atoi(tokens[i]);
        // printf("%d ", nums[i]);
    }
    // printf("\n");

    free(str);

    // find max to know the number of digits
    int max = nums[0];
    for (int i = 1; i < count; i++)
        if (nums[i] > max) max = nums[i];

    // exp = 1 → units
    // exp = 10 → tens
    // exp = 100 → hundreds
    for (int exp = 1; max / exp > 0; exp *= 10)
        counting_sort_by_digit(nums, count, exp);

    for (int i = 0; i < count; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    return 0;
}
