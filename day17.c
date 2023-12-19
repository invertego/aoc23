#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

#define A(i,j) a[(i)+1][(j)+1]
char a[141+2][141+2];
int m, n;

char input[141+2][141+2];

#define INFINITY 10000
#define UNDEFINED (key_t){.v = ~0}

#define CAP (1 << 22)
//#define KEY(i, j, k, l) (((i) << 12) | ((j) << 4) | ((k) << 2) | (l))
#define KEY(i, j, k, l) ((key_t){{(l),(k),(j),(i)}})

typedef union {
    struct {
        unsigned l : 4;
        unsigned k : 2;
        unsigned j : 8;
        unsigned i : 8;
        unsigned _ : 10;
    };
    unsigned v;
} key_t;

typedef struct {
    int i, j;
} coord_t;

#define PUSH(key) (open[opencnt++] = (key))
key_t open[CAP];
int opencnt;

int dist[CAP];
key_t prev[CAP];

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

#if PART == 0
#define MOVE_MIN 1
#define MOVE_MAX 3
#else
#define MOVE_MIN 4
#define MOVE_MAX 10
//1-3
//4-10
#endif

int solve2()
{
    //int count = 0;
    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
    for (int k = 0; k < 4; k++)
    for (int l = 0; l < MOVE_MAX; l++) {
        key_t key = KEY(i,j,k,l);
        //if (count++ < 100) printf("%d %d %d %d %x\n", key.i, key.j, key.k, key.l, key.v);
        dist[key.v] = INFINITY;
        prev[key.v] = UNDEFINED;
    }
    for (int k = 0; k < 4; k++)
    for (int l = MOVE_MAX - 1; l < MOVE_MAX; l++) {
        key_t key = KEY(0,0,k,l);
        dist[key.v] = 0;
        prev[key.v] = UNDEFINED;
        PUSH(key);
    }

    int mindist = INFINITY;
    int maxdist = 0;
    int count = 0;
    while (opencnt) {
        if (!(count++ % 1024)) printf("opencnt %d\n", opencnt);
        int min = INFINITY*100;
        int uidx = -1;
        key_t u;
        for (int idx = 0; idx < opencnt; idx++) {
            key_t tmp = open[idx];
            int d = dist[tmp.v];
            if (min > d) {
                min = d;
                u = tmp;
                uidx = idx;
            }
        }
        assert(uidx >= 0);
        memmove(&open[uidx], &open[uidx + 1], sizeof(open[0]) * (opencnt - uidx - 1));
        opencnt--;
        
        for (int k = 0; k < 4; k++) {
            key_t v = u;
            v.i += di[k];
            v.j += dj[k];
            v.k = k;
            v.l += 1;
            if (k == u.k) {
                if (v.l == MOVE_MAX) continue;
            } else if (k == (u.k ^ 1)) {
                continue;
            } else {
                if (v.l < MOVE_MIN) continue;
                v.l = 0;
            }
            //test v in Q
            if (v.i < 0 || v.i >= m ||
                v.j < 0 || v.j >= n) {
                continue;
            }
            int alt = dist[u.v] + A(v.i,v.j);//Edges(u, v)
            if (alt < mindist) mindist = alt;
            if (alt > maxdist && alt < INFINITY) maxdist = alt;
            if (alt < dist[v.v]) {
                bool found = false;
                for (int idx = 0; idx < opencnt; idx++) {
                    if (open[idx].v == v.v) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    PUSH(v);
                }
                dist[v.v] = alt;
                prev[v.v] = u;
            }
        }
    }

    int shortest = INFINITY;
    void* backup = malloc(sizeof(input));
    memcpy(backup, input, sizeof(input));
    for (int k = 0; k < 4; k++)
    for (int l = MOVE_MIN - 1; l < MOVE_MAX; l++) {
        memcpy(input, backup, sizeof(input));

        int d = dist[KEY(m-1,n-1,k,l).v];
        if (shortest > d) shortest = d;
        printf("dist %d\n", d);
        key_t S[1000];
        int Sidx = 0;
        key_t u = KEY(m-1,n-1,k,l);
        if (prev[u.v].v != UNDEFINED.v || (u.i == 0 && u.j == 0)) {
            while (u.v != UNDEFINED.v) {
                input[u.i][u.j] =
                    "^v<>"[u.k];
                    //"0123"[u.l];
                S[Sidx++] = u;
                u = prev[u.v];
            }

#if 1
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    printf("%c", input[i][j]);
                }
                printf("\n");
            }
#endif
        }
    }

    printf("min %d max %d\n", mindist, maxdist);

    //return dist[m-1][n-1];

    return shortest;
}

int main()
{
    FILE* f = fopen("day17.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&input[m], b, n);
        for (int i = 0; i < n; i++) {
            A(m,i) = b[i] - '0';
        }
        m++;
    }

    //memset(memo, -1, sizeof(memo));
    //sum = solve(0, 0);
    sum = solve2();

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
