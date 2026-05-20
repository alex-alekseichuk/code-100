/*
For 64 disks:
2^64−1 ≈ 1.8×10^19
Even for 1 step per second, it takes ~585 billions years
a[0] = 0             basis
a[n] = 2*a[n-1]-1    recurrent
a[n] = 2^n-1         analytical
*/
#include <stdio.h>

void move(int from, int to, unsigned int n) {
    if (n == 1) {
        printf("%c -(0)-> %c\n", from + 'A', to + 'A');
        return;
    }
    int temp = ~(from ^ to) & 3;
    move(from, temp, n-1);
    printf("%c -(%d)-> %c\n", from + 'A', n-1, to + 'A');
    move(temp, to, n-1);
}

int main() {
    unsigned int n;
    printf("Enter number of disks: ");
    scanf("%u", &n);
    
    move(0, 2, n);

    return 0;
}
