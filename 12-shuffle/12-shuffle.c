/*
Fisher-Yates Shuffle
Randomly choose the latest element in the loop.
1 2 3 4 5 6 7 8 9 10
4 9 8 1 6 10 3 2 5 7
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int n;
    if (argc < 2) {
        printf("Enter elements number: ");
        scanf("%d", &n);
    } else {
        n = atoi(argv[1]);
    }

    int nums[n];
    for (int i = 0; i < n; ++i) {
        nums[i] = i + 1;
        printf("%d ", nums[i]);
    }
    printf("\n");

    srand(time(NULL));

    for (int last = n-1; last > 1; --last) {
        int i = rand() % last;
        int temp = nums[last];
        nums[last] = nums[i];
        nums[i] = temp;
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    return 0;
}
