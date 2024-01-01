#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

char a[100][100], a2[100][100];
int m = 0, n = 0;

uint32_t hash()
{
    uint32_t hash = 5381;
    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
        hash = ((hash << 5) + hash) + a[i][j];
    return hash;
}

void rotate()
{
    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
        a2[j][n-1-i] = a[i][j];
    for (int i = 0; i < m; i++)
        memcpy(&a[i], &a2[i], n);
}

void drop()
{
    for (int j = 0; j < n; j++) {
        int blank = -1;
        for (int i = 0; i < m; i++) {
            char c = a[i][j];
            if (c == '.') {
                if (blank < 0) blank = i;
            } else if (c == '#') {
                blank = -1;
            } else if (c == 'O') {
                if (blank >= 0) {
                    a[blank][j] = c;
                    a[i][j] = '.';
                    blank++;
                    assert(a[blank][j] == '.');
                }
            }
        }
    }
}

int main()
{
    FILE* f = fopen("day14.txt", "r");
    char b[256];
    long long sum = 0;

    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&a[m], b, n);
        m++;
    }

#if PART == 0
    drop();
#else
    //(1000000000-119)%42=41
    for (int j = 0; j < 119+41; j++) {
        for (int i = 0; i < 4; i++) {
            drop();
            rotate();
        }
        printf("%08X %d\n", hash(), j);
    }
#endif

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            //printf("%c", a[i][j]);
            if (a[i][j] == 'O') {
                sum += m - i;
            }
        }
        //printf("\n");
    }

    printf("\n%lld\n", sum);
}
