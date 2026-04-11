#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned int n;

    printf("Введите целое число: ");
    scanf("%u", &n);
    int count = 0;
    for (unsigned int x = n; x > 0; x &= x - 1) {
        count++;
    }
        
    printf("%d\n", count);

    return 0;
}

