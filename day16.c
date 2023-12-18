#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 0

enum {
    N, S, W, E,
};

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

#define M(x) (1 << (x))

typedef struct {
    char c;
    struct {
        char a, b;
    } map[4];
} mirror_t;

const mirror_t mirrors[] = {
    { '.', {
        { N, -1 },
        { S, -1 },
        { W, -1 },
        { E, -1 },
    } },
    { '/', {
        { E, -1 }, // N
        { W, -1 }, // S
        { S, -1 }, // W
        { N, -1 }, // E
    } },
    { '\\', {
        { W, -1 }, // N
        { E, -1 }, // S
        { N, -1 }, // W
        { S, -1 }, // E
    } },
    { '|', {
        { N, -1 }, // N
        { S, -1 }, // S
        { N,  S }, // W
        { N,  S }, // E
    } },
    { '-', {
        { W,  E }, // N
        { W,  E }, // S
        { W, -1 }, // W
        { E, -1 }, // E
    } },
};

typedef struct {
    int i, j, d;
} beam_t;

#define PUSH(x,y,d) (open[opencnt++] = (beam_t){(x),(y),(d)})
#define POP() open[--opencnt]
beam_t open[10000];
int opencnt;

char a[110][110];
char v[110][110];
char w[110][110];
int m = 0, n = 0;

int cast(int si, int sj, int sd)
{
    memset(v, 0, sizeof(v));
    memcpy(w, a, sizeof(a));

    PUSH(si,sj,sd);

    while (opencnt) {
        beam_t x = POP();
        //printf("pop %d %d %c\n", x.i, x.j, "NSWE"[x.d]);
        v[x.i][x.j] |= M(x.d);
        if (w[x.i][x.j] == '.') w[x.i][x.j] = "^V<>"[x.d];
        char c = a[x.i][x.j];
        char d[2] = {-2, -2};
        for (int i = 0; i < sizeof(mirrors) / sizeof(mirrors[0]); i++) {
            if (mirrors[i].c == c) {
                d[0] = mirrors[i].map[x.d].a;
                d[1] = mirrors[i].map[x.d].b;
                break;
            }
        }
        assert(d[0] >= 0 && d[1] >= -1);
        for (int i = 0; i < 2; i++) {
            int nd = d[i];
            if (nd < 0) continue;
            int ni = x.i + di[nd];
            int nj = x.j + dj[nd];
            if (ni >= 0 && ni < m &&
                nj >= 0 && nj < n &&
                !(v[ni][nj] & M(nd))) {
                //printf("psh %d %d %c\n", ni, nj, "NSWE"[nd]);
                PUSH(ni, nj, nd);
            }
        }
        //printf("%d\n", opencnt);
    }

    int sum = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            sum += v[i][j] != 0;
            //printf("%c", w[i][j]);
        }
        //printf("\n");
    }

    return sum;
}

int main()
{
    FILE* f = fopen("day16.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&a[m], b, n);
        m++;
    }

    sum = cast(0,0,E);
    
    int e;
    #define UPD() if (e > sum2) sum2 = e
    for (int i = 0; i < m; i++) {
        e = cast(i, 0, E); UPD();
        e = cast(i, n-1, W); UPD();
    }
    for (int j = 0; j < n; j++) {
        e = cast(0, j, S); UPD();
        e = cast(m-1, j, N); UPD();
    }

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
