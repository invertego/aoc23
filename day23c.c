#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

#define A(i,j) a[(i)+1][(j)+1]
char a[141+2][141+2];
int m, n;

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

typedef struct {
    int i, j;
    int d;
    bool v;
} open_t;

#define PUSH(i, j, d) (assert(opencnt < COUNT(open)), open[opencnt++] = (open_t){(i),(j),(d)})
open_t open[10000];
int opencnt;

bool visited[141][141];

int dfs(bool slope)
{
    int maxdist = 0;
    PUSH(0,1,0);

    while (opencnt) {
        open_t* o = &open[opencnt-1];
        if (o->v) {
            if (o->i == m - 1 && o->j == n - 2) {
                if (maxdist < o->d) maxdist = o->d;
            }
            visited[o->i][o->j] = false;
            --opencnt;
            continue;
        }
        o->v = true;
        visited[o->i][o->j] = true;
        char t = A(o->i,o->j);
        for (int k = 0; k < 4; k++) {
            if (slope) {
                if (t == '<' && dj[k] != -1) continue;
                if (t == '>' && dj[k] !=  1) continue;
                if (t == '^' && di[k] != -1) continue;
                if (t == 'v' && di[k] !=  1) continue;
            }
            int vi = o->i + di[k];
            int vj = o->j + dj[k];
            if (A(vi,vj) == '#') continue;
            if (visited[vi][vj]) continue;
            PUSH(vi, vj, o->d + 1);
        }
    }

    return maxdist;
}

int main()
{
    FILE* f = fopen("day23.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    memset(a, '#', sizeof(a));
    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&A(m,0), b, n);
        m++;
    }

    sum = dfs(true);
    printf("\n%lld\n", sum);

    sum2 = dfs(false);
    printf("\n%lld\n", sum2);
}
