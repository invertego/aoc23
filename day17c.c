#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 0

#define A(i,j) a[(i)+1][(j)+1]
char a[141+2][141+2];
int m, n;

char input[141+2][141+2];

#define INFINITY 10000
#define UNDEFINED (key_t){.v = ~0}

#define CAP (1 << 20)
//#define KEY(i, j, k, l) (((i) << 12) | ((j) << 4) | ((k) << 2) | (l))
#define KEY(i, j, k, l) ((key_t){{(l),(k),(j),(i)}})

typedef union {
    struct {
        unsigned l : 2;
        unsigned k : 2;
        unsigned j : 8;
        unsigned i : 8;
        unsigned _ : 12;
    };
    unsigned v;
} key_t;

typedef struct {
    int i, j;
} coord_t;

//#define PUSH(key) (open[opencnt++] = (key))
//key_t open[CAP];
int opencnt;

#define KEY_CNT (1 << 9)
typedef struct {
    key_t keys[KEY_CNT];
    int cnt;
} bucket_t;

#define BUCKET_CNT (1 << 11)
bucket_t buckets[BUCKET_CNT];

void push(key_t key, int dist)
{
    assert(dist < BUCKET_CNT - 1 || dist >= INFINITY);
    if (dist >= INFINITY) dist = (BUCKET_CNT) - 1;
    bucket_t* b = &buckets[dist];
    b->keys[b->cnt++] = key;
    assert(b->cnt < KEY_CNT);
    opencnt++;
}

void qremove(key_t key, int dist)
{
    assert(dist < BUCKET_CNT - 1 || dist >= INFINITY);
    if (dist >= INFINITY) dist = (BUCKET_CNT) - 1;
    bucket_t* b = &buckets[dist];
    for (int i = 0; i < b->cnt; i++) {
        if (b->keys[i].v == key.v) {
            memmove(&b->keys[i], &b->keys[i + 1], sizeof(key_t) * (b->cnt - i - 1));
            b->cnt--;
            opencnt--;
            break;
        }
    }
    //assert(false);
}
//min 4 max 1261
//1238

int dist[CAP];
key_t prev[CAP];

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

int solve2()
{
    //int count = 0;
    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
    for (int k = 0; k < 4; k++)
    for (int l = 0; l < 3; l++) {
        key_t key = KEY(i,j,k,l);
        //if (count++ < 100) printf("%d %d %d %d %x\n", key.i, key.j, key.k, key.l, key.v);
        dist[key.v] = INFINITY;
        prev[key.v] = UNDEFINED;
    }
    for (int k = 0; k < 4; k++)
    for (int l = 0; l < 3; l++) {
        key_t key = KEY(0,0,k,l);
        dist[key.v] = 0;
        prev[key.v] = UNDEFINED;
        push(key, 0);
    }

    int mindist = INFINITY;
    int maxdist = 0;
    int count = 0;
    while (opencnt) {
        if (!(count++ % 1024)) printf("opencnt %d\n", opencnt);
        key_t u;
        for (int bi = 0; bi < BUCKET_CNT; bi++) {
            bucket_t* b = &buckets[bi];
            if (b->cnt != 0) {
                u = b->keys[--b->cnt];
                opencnt--;
                break;
            }
        }
        
        for (int k = 0; k < 4; k++) {
            key_t v = u;
            v.i += di[k];
            v.j += dj[k];
            v.k = k;
            if (k == u.k) {
                if (++v.l == 3) continue;
            } else if (k == (u.k ^ 1)) {
                continue;
            } else {
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
                qremove(v, dist[v.v]);
                push(v, alt);
                dist[v.v] = alt;
                prev[v.v] = u;
            }
        }
    }

    int shortest = INFINITY;
    void* backup = malloc(sizeof(input));
    memcpy(backup, input, sizeof(input));
    for (int k = 0; k < 4; k++)
    for (int l = 0; l < 3; l++) {
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
        
#if 0
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
