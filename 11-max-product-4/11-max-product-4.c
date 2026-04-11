/*
    What is the greatest product of four adjacent numbers in the same direction
        (up, down, left, right, or diagonally)
    in a 20x20 grid?
    Answer: 70600674
    gcc 11-max-product-4.c ../common/common.c -lm
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../common/common.h"

int nums[20][20];
float logs[20][20];

unsigned int max_product_4(int dx, int dy) {
    float m = 0;
    unsigned int product = 0;
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (x+3*dx >= 20 || y+3*dy >= 20 || x+3*dx < 0 || y+3*dy < 0)
                continue;            
            float c = logs[y][x] + logs[y+dy][x+dx] + logs[y+2*dy][x+2*dx] + logs[y+3*dy][x+3*dx];
            if (c > m) {
                m = c;
                product = nums[y][x] * nums[y+dy][x+dx] * nums[y+2*dy][x+2*dx] * nums[y+3*dy][x+3*dx];
            }
        }
    }
    return product;
}

int main() {
    char *str = NULL;
    size_t len = 0;

    for (int row = 0; row < 20; row++) {
        size_t nread = read_line(&str, &len, stdin);
        size_t col = 20;
        char *tokens[20];
        parse_line(str, tokens, &col, " \n");
        if (col != 20) {
            return 1;
        }
        for (col = 0; col < 20; col++) {
            nums[row][col] = atoi(tokens[col]);
            logs[row][col] = log(nums[row][col]);
        }
    }
    free(str);

    unsigned int m = max_product_4(1, 0);
    unsigned int c = max_product_4(0, 1);
    if (m < c) m = c;
    c = max_product_4(1, 1);
    if (m < c) m = c;
    c = max_product_4(-1, 1);
    if (m < c) m = c;

    printf("%u\n", m);

    return 0;
}
