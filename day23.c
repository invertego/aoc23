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

#define INFINITY 10000
#define UNDEFINED (key_t){.v = ~0}

#define CAP (1 << 16)
#define KEY(i, j) ((key_t){{(j),(i)}})

typedef union {
    struct {
        unsigned j : 8;
        unsigned i : 8;
        unsigned _ : 16;
    };
    unsigned v;
} key_t;

#define PUSH(key) (open[opencnt++] = (key))
key_t open[CAP];
int opencnt;

int dist[CAP];
key_t prev[CAP];
bool visited[CAP];
char mask[CAP];

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

int solve1(bool longest)
{
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) {
        key_t key = KEY(i,j);
        dist[key.v] = INFINITY;
        prev[key.v] = UNDEFINED;
    }
    {
        key_t key = KEY(0,1);
        dist[key.v] = 0;
        prev[key.v] = UNDEFINED;
        PUSH(key);
    }

    //int count = 0;
    while (opencnt) {
        //if (!(count++ % 1024)) printf("opencnt %d %d\n", opencnt, count);
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
        visited[u.v] = true;

        char uc = A(u.i,u.j);
        for (int k = 0; k < 4; k++) {
            if (longest) {
                if (!(mask[u.v] & (1 << k))) continue;
            }
            if (uc == '<' && dj[k] != -1) continue;
            if (uc == '>' && dj[k] !=  1) continue;
            if (uc == '^' && di[k] != -1) continue;
            if (uc == 'v' && di[k] !=  1) continue;
            int vi = u.i + di[k];
            int vj = u.j + dj[k];
            char vc = A(vi,vj);
            if (vc == '#') continue;
            if (vc == '<' && dj[k] ==  1) continue;
            if (vc == '>' && dj[k] == -1) continue;
            if (vc == '^' && di[k] ==  1) continue;
            if (vc == 'v' && di[k] == -1) continue;
            key_t v = KEY(vi,vj);
            int alt = dist[u.v] < INFINITY ? dist[u.v] + (longest ? -1 : 1) : INFINITY;
            if (alt < dist[v.v]) {
                bool found = false;
                for (int idx = 0; idx < opencnt; idx++) {
                    if (open[idx].v == v.v) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    assert(opencnt < CAP);
                    PUSH(v);
                }
                dist[v.v] = alt;
                prev[v.v] = u;
            }
        }
    }

#if 1
    {
        key_t u = KEY(m-1,n-2);
        if (prev[u.v].v != UNDEFINED.v || (u.i == 0 && u.j == 1)) {
            while (u.v != UNDEFINED.v) {
                if (longest) A(u.i,u.j) = 'O';
                u = prev[u.v];
            }
        }
    }
#endif

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            key_t v = KEY(i,j);
            char vc = A(i,j);
            if (vc == '^') mask[v.v] |= 1;
            if (vc == 'v') mask[v.v] |= 2;
            if (vc == '<') mask[v.v] |= 4;
            if (vc == '>') mask[v.v] |= 8;

            key_t p = prev[KEY(i,j).v];
            if (p.v != UNDEFINED.v) {
                int m = 0;
                if (p.i > i) m |= 1;
                if (p.i < i) m |= 2;
                if (p.j > j) m |= 4;
                if (p.j < j) m |= 8;
                mask[p.v] |= m;
            }
            //printf("%c", A(i,j));
            //printf("%c", visited[v.v] ? 'V' : A(i,j));
        }
        //printf("\n");
    }
    //printf("\n");

#if 0
    if (!longest) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                char m = mask[KEY(i,j).v];
                int bits = 0;
                for (int k = 0; k < 4; k++) {
                    if (m & (1 << k)) bits++;
                }
                printf("%c", '0' + bits);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif

    int distance = dist[KEY(m-1,n-2).v];
    return longest ? -distance : distance;
}

typedef struct {
    key_t k;
    int d;
} edge_t;

typedef struct {
    edge_t edges[4];
    int edgecnt;
    bool visit;
} vert_t;

vert_t verts[CAP];
int card[CAP];

key_t vertkeys[40];
int vertcnt;

int recurse(key_t key)
{
    if (key.i == m-1 && key.j == n-2) return 0;
    vert_t* v = &verts[key.v];
    int max = -INFINITY;
    if (!v->visit) {
        v->visit = true;
        for (int k = 0; k < v->edgecnt; k++) {
            edge_t* e = &v->edges[k];
            int dist = e->d + recurse(e->k);
            if (dist > max) max = dist;
        }
        v->visit = false;
    }
    return max;
}

int solve2()
{
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
            key_t u = KEY(i,j);
            card[u.v] = adj;
            if (adj == 1 || adj > 2) {
                assert(vertcnt < sizeof(vertkeys) / sizeof(vertkeys[0]));
                vertkeys[vertcnt++] = u;
            }
        }
        //printf("\n");
    }

    for (int vidx = 0; vidx < vertcnt; vidx++) {
        key_t key = vertkeys[vidx];
        vert_t* v = &verts[key.v];

        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            dist[KEY(i,j).v] = INFINITY;
        }
        dist[key.v] = 0;
        PUSH(key);

        while (opencnt) {
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

            if ((card[u.v] == 1 || card[u.v] > 2) && min != 0) {
                assert(v->edgecnt < sizeof(v->edges) / sizeof(v->edges[0]));
                edge_t* e = &v->edges[v->edgecnt++];
                e->k = u;
                e->d = min;
                continue;
            }

            for (int k = 0; k < 4; k++) {
                int vi = u.i + di[k];
                int vj = u.j + dj[k];
                char vc = A(vi,vj);
                if (vc == '#') continue;
                key_t v = KEY(vi,vj);
                int alt = dist[u.v] < INFINITY ? dist[u.v] + 1 : INFINITY;
                if (alt < dist[v.v]) {
                    dist[v.v] = alt;
                    bool found = false;
                    for (int idx = 0; idx < opencnt; idx++) {
                        if (open[idx].v == v.v) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        assert(opencnt < CAP);
                        PUSH(v);
                    }
                }
            }
        }

#if 0
        printf("vert %d %d\n", key.i, key.j);
        for (int k = 0; k < v->edgecnt; k++) {
            printf("  %d %d %d\n", v->edges[k].k.i, v->edges[k].k.j, v->edges[k].d);
        }
#endif
    }

    return recurse(KEY(0,1));
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

    sum = solve1(false);
    printf("\nshortest %lld\n", sum);
    sum = solve1(true);

    sum2 = solve2();

    printf("\nlongest 1 %lld\n", sum);
    printf("\nlongest 2 %lld\n", sum2);
}
