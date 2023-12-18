#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PART 1

typedef union {
    struct { int i, j; };
    int c[2];
} coord_t;

coord_t open[100000];
int opencnt;
const int expand = (PART == 0) ? 2 : 1000000;

#define PUSH(x,y) (open[opencnt++] = (coord_t){{(x),(y)}})

int main()
{
    FILE* f = fopen("day11.txt", "r");
    char b[256];

    int i = 0, j = 0, k = 0;
    int dims[2] = {0};
    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        dims[1] = strlen(b);
        for (j = 0; j < dims[1]; j++) {
            if (b[j] == '#') {
                PUSH(i,j);
            }
        }
        dims[0] = ++i;
    }

    for (j = 0; j < 2; j++) {
        for (i = 0; i < dims[j]; i++) {
            bool any = false;
            for (k = 0; k < opencnt; k++) {
                if (open[k].c[j] == i) {
                    any = true;
                    break;
                }
            }
            if (!any) {
                for (k = 0; k < opencnt; k++) {
                    if (open[k].c[j] > i) {
                        open[k].c[j] += expand - 1;
                    }
                }
                i += expand - 1;
                dims[j] += expand - 1;
            }
        }
    }

    long long sum = 0;
    for (i = 0; i < opencnt; i++) {
        for (j = i + 1; j < opencnt; j++) {
            sum += abs(open[i].i - open[j].i);
            sum += abs(open[i].j - open[j].j);
        }
    }

    printf("%lld\n", sum);
}
