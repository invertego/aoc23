#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

const char dirs[] = "UDLR";

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

char a[512][512];

#define A(i,j) a[(i)&511][(j)&511]

typedef struct {
    int i, j;
} coord_t;

coord_t open[300000];
int opencnt;

int verti[1024];
int vertcnt;

int verticmp(const void* va, const void* vb)
{
    const int* a = va;
    const int* b = vb;
    return *a - *b;
}

typedef struct {
    int i[2], j[2];
} edge_t;

edge_t edge[1024];
int edgecnt;

int edgecmp(const void* va, const void* vb)
{
    const edge_t* a = va;
    const edge_t* b = vb;
    if (a->j[0] != b->j[0])
        return a->j[0] - b->j[0];
    else
        return a->j[1] - b->j[1];
}

int main()
{
    FILE* f = fopen("day18.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    memset(a, ' ', sizeof(a));
    int i = 0, j = 0;
    int min[2] = {INT_MAX,INT_MAX};
    int max[2] = {INT_MIN,INT_MIN};
    verti[vertcnt++] = 0;
    while (fgets(b, sizeof(b), f)) {
        //D 5 (#0dc571)
#if PART == 0
        int dir = strchr(dirs, b[0]) - dirs;
        int dist;
        sscanf(b + 1, "%d", &dist);
        for (int k = 0; k < dist; k++) {
            int ni = i + di[dir] * (k + 1);
            int nj = j + dj[dir] * (k + 1);
            A(ni,nj) = '#';
        }
#else
        char* t = strchr(b, '#') + 1;
        char hex[6] = {0};
        memcpy(hex, t, 5);
        int dist;
        sscanf(hex, "%x", &dist);
        char dirc = "RDLU"[t[5]-'0'];
        int dir = strchr(dirs, dirc) - dirs;
#endif
        //printf("%c %d\n", dirc, dist);
        /*if(!di[dir]) {
            seg_t* s = &hsegs[hsegcnt++];
            s->i = i;
            s->b = j;
            s->e = j + dj[dir] * dist;
        } else {
            seg_t* s = &vsegs[vsegcnt++];
            s->i = j;
            s->b = i;
            s->e = i + di[dir] * dist;
        }*/
        //int mi = i + di[dir];
        //int mj = j + dj[dir];
        int ni = i + di[dir] * dist;
        int nj = j + dj[dir] * dist;
        edge_t* e = &edge[edgecnt++];
        e->i[0] = MIN(i, ni);
        e->j[0] = MIN(j, nj);
        e->i[1] = MAX(i, ni);
        e->j[1] = MAX(j, nj);
        verti[vertcnt++] = i;
        i = ni;
        j = nj;

        min[0] = MIN(i, min[0]);
        min[1] = MIN(j, min[1]);
        max[0] = MAX(i, max[0]);
        max[1] = MAX(j, max[1]);
        //printf("%d %d %d %d\n", dir, dist, i, j);
    }
    A(0,0) = 'S';

    printf("%d %d\n", min[0], max[0]);
    printf("%d %d\n", min[1], max[1]);

    qsort(&verti, vertcnt, sizeof(verti[0]), verticmp);
    qsort(&edge, edgecnt, sizeof(edge[0]), edgecmp);

    int64_t total[2] = {0};
    int prog = 0;
    for (int i = min[0]; i <= max[0]; i++) {
        int p = (i - min[0]) * 100 / (max[0] - min[0]);
        if (p != prog) printf("%d\n", p);
        prog = p;
        bool inside = false;
        int j = min[1];
        int x = 0;
        for (int k = 0; k < edgecnt; k++) {
            edge_t* e = &edge[k];
            bool step = false;
            bool flip = false;
            if (e->j[0] == e->j[1]) {
                //v
                if (e->i[0] < i && i < e->i[1]) {
                    step = true;
                    flip = true;
                } else if (e->i[0] == i) {
                    if (x == 0) x = 1;
                    else {
                        if (x == 2) flip = true;
                        x = 0;
                    }
                } else if (i == e->i[1]) {
                    if (x == 0) x = 2;
                    else {
                        if (x == 1) flip = true;
                        x = 0;
                    }
                }
            } else {
                //h
                if (e->i[0] == i) {
                    step = true;
                }
            }
            if (step) {
                int delta = e->j[0] - j;
                total[inside] += delta;
                //for (int z = 0; z < delta; z++) printf("%c", ".#"[inside]);
                delta = e->j[1] - e->j[0] + 1;
                total[true] += delta;
                //for (int z = 0; z < delta; z++) printf("#");
                j = e->j[1] + 1;
            }
            if (flip) inside = !inside;
        }
        int delta = max[1] + 1 - j;
        total[inside] += delta;
        //for (int z = 0; z < delta; z++) printf("%c", ".#"[inside]);
        //printf("\n");
    }

    printf("total %lld %lld\n", total[0], total[1]);

#if PART == 0 && 0

    int m = max[0] - min[0] + 1;
    int n = max[1] - min[1] + 1;

#if 0
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c", A(i+min[0],j+min[1]));
        }
        printf("\n");
    }
#endif

#define PUSH(x,y) (open[opencnt++] = (coord_t){(x),(y)})
#define POP() open[--opencnt]
    PUSH(min[0]-1,min[1]-1);
    while (opencnt) {
        coord_t c = POP();
        i = c.i, j = c.j;
        A(i,j) = '.';
        if (i >= min[0]-1 && A(i-1,j) == ' ') PUSH(i-1,j);
        if (i <= max[0]+1 && A(i+1,j) == ' ') PUSH(i+1,j);
        if (j >= min[1]-1 && A(i,j-1) == ' ') PUSH(i,j-1);
        if (j <= max[1]+1 && A(i,j+1) == ' ') PUSH(i,j+1);
        assert(opencnt < 300000);
    }

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (A(i+min[0],j+min[1]) != '.') sum++;
        }
    }

#endif

    printf("\n%lld\n", sum); // 49897
    printf("\n%lld\n", sum2);
}
