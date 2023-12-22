#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

char a[131+2][131+2];
int m, n;

#if PART == 0
#define A(i,j) a[(i)+1][(j)+1]
#else
#define ABS(x,y) ((x) < 0 ? (x)+((y)<<23) : (x))
#define A(i,j) a[ABS(i,m)%m][ABS(j,n)%n]
#endif

typedef struct {
    int i, j;
} coord_t;

#define QUEUE_CAP 100000
typedef struct {
    coord_t el[QUEUE_CAP];
    int cnt;
} queue_t;

queue_t qs[2];
int qidx;

const int di[] = {-1,1,0,0};
const int dj[] = {0,0,-1,1};

#define V(i,j) vm[(i)&V_MASK][(j)&V_MASK]
#define V_MASK 16383
char vm[V_MASK + 1][V_MASK + 1];

#define CNT_CAP 32
#define CNT(i,j) cnts[(((i-si)/m)+CNT_CAP/2)&(CNT_CAP-1)][(((j-sj)/n)+CNT_CAP/2)&(CNT_CAP-1)]
int cnts[CNT_CAP][CNT_CAP];

int64_t solve(int64_t x)
{
    return (15567 * x * x + 250 * x + 3) / 4;
}

int main()
{
    FILE* f = fopen("day21.txt", "r");
    char b[256];
    int si = 0, sj = 0;
    int64_t sum = 0, sum2 = 0;

    int i = 0;
    while (fgets(b, sizeof(b), f)) {
        char* ctx;
        strtok_s(b, "\n", &ctx);
        char* s = strchr(b, 'S');
        if (s) { si = i; sj = s - b; *s = '.'; }
        m = n = strlen(b);
        memcpy(&A(i,0), b, n);
        i++;
    }

#if 0
    int inputs[] = {
#if PART == 0
        1,2,3,6,64
#else
        6,10,50,100,500,1000,5000
#endif
    };
    for (int k = 0; k < sizeof(inputs) / sizeof(inputs[0]); k++) {
        int steps = inputs[k];
#else
    for (int k = 5; k <= 17; k += 2) {
        int steps = (k * m - 1) / 2;
#endif

        sum = sum2 = 0;
        qs[0].cnt = qs[1].cnt = 0;
        qs[qidx].el[qs[qidx].cnt++] = (coord_t){si,sj};
        memset(vm, 0, sizeof(vm));
#if PART == 0
        for (int i = 0; i < m; i++) { V(i, -1) = ~0; V(i, n) = ~0; }
        for (int j = 0; j < n; j++) { V(-1, j) = ~0; V(m, j) = ~0; }
#endif
        memset(cnts, 0, sizeof(cnts));

        for (int depth = 0; depth < steps; depth++) {
            queue_t* q = &qs[qidx];
            queue_t* p = &qs[qidx ^ 1];

            while (q->cnt) {
                coord_t u = q->el[--q->cnt];
                for (int d = 0; d < 4; d++) {
                    coord_t v = u;
                    v.i += di[d & 3];
                    v.j += dj[d & 3];
                    if (A(v.i,v.j) == '#') continue;
                    int mask = 1 << (depth & 1);
                    if (V(v.i,v.j) & mask) continue;
                    V(v.i,v.j) |= mask;
                    assert(p->cnt + 1 < QUEUE_CAP);
                    p->el[p->cnt++] = v;
                    if (depth & 1) sum += 1; else sum2 += 1;
                    if ((depth ^ steps) & 1) CNT(v.i,v.j)++;
                }
            }

            qidx ^= 1;
        }

        printf("k %d steps %d sum %lld sum2 %lld solve %lld\n", k, steps, sum, sum2, solve(k));

#if 0
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                char c;
                switch (V(i,j)) {
                    case 0: c = A(i,j); break;
                    case 1: c = 'O'; break;
                    case 2: c = 'E'; break;
                    case 3: c = 'X'; break;
                }
                printf("%c", c);
            }
            printf("\n");
        }
#endif

#if 0
        int64_t cntsum = 0;
        for (int i = 0; i < CNT_CAP; i++) {
            for (int j = 0; j < CNT_CAP; j++) {
                printf("%5d ", cnts[i][j]);
                cntsum += cnts[i][j];
            }
            printf("\n");
        }
        printf("cntsum %lld\n", cntsum);
#endif
    }

    printf("\nsolve %lld\n", solve((26501365*2+1)/131));
}
