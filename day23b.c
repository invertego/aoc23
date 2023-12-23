#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

#define A(i,j) a[(i)+1][(j)+1]
char a[141+2][141+2];
int m, n;

#define INFINITY 10000

typedef struct {
    int i, j;
} coord_t;

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

typedef struct {
    coord_t c;
    int d;
} edge_t;

typedef struct {
    edge_t edges[4];
    int edgecnt;
    bool node;
    bool visit;
} vert_t;

vert_t verts[141][141];

coord_t vertkeys[40];
int vertcnt;

void dfs1(coord_t c, int d, vert_t* start, bool slope)
{
    vert_t* v = &verts[c.i][c.j];
    if (v->visit) return;

    if (v->node && d != 0) {
        assert(start->edgecnt < COUNT(start->edges));
        edge_t* e = &start->edges[start->edgecnt++];
        e->c = c;
        e->d = d;
        return;
    }

    char uc = A(c.i,c.j);
    v->visit = true;
    for (int k = 0; k < 4; k++) {
        if (slope) {
            if (uc == '<' && dj[k] != -1) continue;
            if (uc == '>' && dj[k] !=  1) continue;
            if (uc == '^' && di[k] != -1) continue;
            if (uc == 'v' && di[k] !=  1) continue;
        }
        coord_t v = c;
        v.i += di[k];
        v.j += dj[k];
        char vc = A(v.i,v.j);
        if (vc == '#') continue;
        dfs1(v, d + 1, start, slope);
    }
    v->visit = false;
}

int dfs2(coord_t c)
{
    if (c.i == m-1 && c.j == n-2) return 0;
    vert_t* v = &verts[c.i][c.j];
    int max = -INFINITY;
    if (!v->visit) {
        v->visit = true;
        for (int k = 0; k < v->edgecnt; k++) {
            edge_t* e = &v->edges[k];
            int dist = e->d + dfs2(e->c);
            if (dist > max) max = dist;
        }
        v->visit = false;
    }
    return max;
}

int solve(bool slope)
{
    memset(verts, 0, sizeof(verts));
    vertcnt = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int adj = 0;
            if (A(i,j) != '#') {
                for (int k = 0; k < 4; k++) {
                    int vi = i + di[k];
                    int vj = j + dj[k];
                    if (A(vi,vj) != '#') adj++;
                }
            }
            //printf("%c", '0' + adj);
            if (adj == 1 || adj > 2) {
                verts[i][j].node = true;
                assert(vertcnt < COUNT(vertkeys));
                vertkeys[vertcnt++] = (coord_t){i,j};
            }
        }
        //printf("\n");
    }

    for (int vidx = 0; vidx < vertcnt; vidx++) {
        coord_t c = vertkeys[vidx];
        vert_t* v = &verts[c.i][c.j];

        dfs1(c, 0, v, slope);

#if 0
        printf("vert %d %d\n", c.i, c.j);
        for (int k = 0; k < v->edgecnt; k++) {
            printf("  %d %d %d\n", v->edges[k].c.i, v->edges[k].c.j, v->edges[k].d);
        }
#endif
    }

    return dfs2((coord_t){0,1});
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

    sum = solve(true);
    sum2 = solve(false);

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
