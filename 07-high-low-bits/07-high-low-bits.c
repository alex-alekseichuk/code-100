#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned int n;

    printf("Введите целое число: ");
    scanf("%u", &n);
    int high, low;
    unsigned int x;
    for (x = n, high = 0, low = 0; x > 1; high++, x >>= 1) {
        if (low == 0 && (x & 1))
            low = high;
    }
        
    printf("%d %d\n", high, low);

    return 0;
}

