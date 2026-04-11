/**
 * All combinations of elements from a set (max 64 elements)
 *   - w/o null set
 *   - w/o full set
 * build:
 * gcc 09-bsub.c ../common/common.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../common/common.h"

int main() {
    char *str = NULL;
    size_t len = 0;

    printf("Введите элементы: ");
    size_t nread = read_line(&str, &len, stdin);
    
    char *tokens[100];
    size_t count = 100;

    parse_line(str, tokens, &count, " \n");
    if (count == 0 || count > 64) {
        return 1;
    }

    unsigned int mask = pow(2, count) - 1;

    unsigned int x = 1;
    while (x != mask) {
        for (int i = 0; i < count; i++)
            if (x & (1 << i))
                printf("%s ", tokens[i]);
        printf("\n");
        x = (x - mask) & mask;
    }

    free(str);

    return 0;
}
