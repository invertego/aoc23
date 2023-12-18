#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

char a[17][17];
int m = 0, n = 0;

int solve(int ignore)
{
    int dims[2] = {m,n};
    for (int d = 0; d < 2; d++)
    for (int i = 1; i < dims[d]; i++) {
        for (int y = 0; y < dims[d]; y++) {
            int y1 = i + y, y2 = i - y - 1;
            if (y1 >= dims[d] || y2 < 0) continue;
            if (d == 0) {
                if (memcmp(&a[y1], &a[y2], n)) {
                    goto fail;
                }
            } else {
                for (int j = 0; j < m; j++) {
                    if (a[j][y1] != a[j][y2]) {
                        goto fail;
                    }
                }
            }
        }

        int score = i;
        if (d == 0) score *= 100;
        if (score != ignore) return score;

        fail: continue;
    }

#if PART == 0
    assert(false);
#endif
    return 0;
}

int main()
{
    FILE* f = fopen("day13.txt", "r");
    char b[256];
    int64_t sum = 0;

    bool done = false;
    while (!done) {
        done = !fgets(b, sizeof(b), f);
        if (done || b[0] == '\n') {
            //printf("%d %d\n", m, n);
            int sols = solve(0);
#if PART == 1
            int ignore = sols;
            for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                char tmp = a[i][j];
                a[i][j] = (tmp == '#') ? '.' : '#';
                sols = solve(ignore);
                if (sols) {
                    for (int u = 0; u < m; u++) {
                        for (int v = 0; v < n; v++) {
                            printf("%c", a[u][v]);
                        }
                        printf("\n");
                    }
                    goto done;
                }
                a[i][j] = tmp;
            }
            assert(false);
            done:
#endif
            printf("%d\n", sols);
            sum += sols;
            m = n = 0;
            continue;
        }
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&a[m], b, n);
        m++;
    }

    printf("\n%lld\n", sum);
    //34312 too high
}
