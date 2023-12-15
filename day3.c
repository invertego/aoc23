#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int prod;
    int cnt;
} gear;

#define N 140
#define M 140
char grid[M + 2][N + 2];
gear gears[M + 2][N + 2];

#define G(x,y) grid[(x + 1)][(y + 1)]
#define GEAR(x,y) gears[(x + 1)][(y + 1)]

bool check(int x, int y, int val)
{
    char c = G(x,y);
    if (ispunct(c) && c != '.') {
        if (c == '*') {
            gear* g = &GEAR(x,y);
            if (!g->cnt++) {
                g->prod = 1;
            }
            g->prod *= val;
        }
        return true;
    }
    return false;
}

int main()
{
    int sum = 0, sum2 = 0;
    FILE* f = fopen("day3.txt", "r");
    char b[256];
    int m = 0, n, i;
    while (fgets(b, sizeof(b), f)) {
        memcpy(&G(m, 0), b, N);
        ++m;
    }
    for (m = 0; m < M; m++) {
        for (n = 0; n < N; ) {
            for (i = n; isdigit(G(m,i)); i++) ;
            int len = i - n;
            if (len == 0) { n++; continue; }
            int val;
            sscanf(&G(m,n), "%d", &val);
            printf("%d %d\n", val, len);
            bool adj = false;
            for (i = n - 1; i <= n + len; i++) {
                adj |= check(m-1,i,val);
                adj |= check(m+1,i,val);
            }
            //for (i = m - 1; i <= m + 1; i++) {
                adj |= check(m,n-1,val);
                adj |= check(m,n+len,val);
            //}
            if (adj) sum += val;
            n += len;
        }
    }
    for (m = 0; m < M; m++) {
        for (n = 0; n < N; n++) {
            gear* g = &GEAR(m,n);
            //printf("%d", g->cnt);
            if (g->cnt == 2) {
                sum2 += g->prod;
            }
        }
        //printf("\n");
    }
    printf("%d\n", sum);
    printf("%d\n", sum2);
}
