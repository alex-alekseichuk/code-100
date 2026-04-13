/**
for 64 bits:
max correct time_t = 67767976233521999 = 0xF0C29D868BD34F
  Tue Dec 31 23:59:59 2147483647
to be able to put as year in ctime()

it's not max int64_t:
  2^63 - 1 = 9,223,372,036,854,775,807
  292277026642 year

for 32 bits:
  max correct time_t: 2^31 - 1 = 2,147,483,647
  Tue Jan 19 03:14:07 2038
  LONG_MAX from limits.h
*/
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <string.h>

int main() {
    time_t ts[] = {LONG_MAX, 0x7FFFFFFF, 0xF0C29D868BD34F, 0xF0C29D868BD350};
    printf("size of time_t: %lu bytes\n", sizeof(time_t)); // 8
    char *str;

    for (int i = 0; i < 4; i++) {
        str = ctime(&ts[i]);
        printf("%ld\n", ts[i]);
        if (str == NULL) {
            printf("\tIncorrect time_t\n");
        } else {
            printf("\t%s", str);
        }
    }

    return 0;
}

