#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 0

#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

#define LABEL(s) ((s[0] & 0x1f) | ((s[1] & 0x1f) << 5) | ((s[2] & 0x1f) << 10))

typedef struct {
    int edges[10];
    int edgecnt;
} vert_t;

vert_t verts[1 << 15];
bool visit[1 << 15];

int open[10000];
int opencnt;

int bfs(int start)
{
    int sum = 0;
    memset(visit, 0, sizeof(visit));
    open[opencnt++] = start;

    while (opencnt) {
        int i = open[--opencnt];
        vert_t* vi = &verts[i];
        if (visit[i]) continue;
        visit[i] = true;
        sum += 1;

        for (int k = 0; k < vi->edgecnt; k++) {
            int j = vi->edges[k];
            assert(opencnt < COUNT(open));
            open[opencnt++] = j;
        }
    }

    return sum;
}

int pairs[3][2] = {
    {LABEL("nmv"), LABEL("thl")},
    {LABEL("vgk"), LABEL("mbq")},
    {LABEL("fxr"), LABEL("fzb")},
};

int main()
{
    FILE* f = fopen("day25.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        char* ctx;
        char* t = strtok_s(b, ":", &ctx);
        int i = LABEL(t);
        vert_t* vi = &verts[i];
        while ((t = strtok_s(NULL, " \n", &ctx))) {
            int j = LABEL(t);
            bool skip = false;
            for (int k = 0; k < COUNT(pairs); k++) {
                if ((i == pairs[k][0] && j == pairs[k][1]) ||
                    (i == pairs[k][1] && j == pairs[k][0])) {
                    skip = true;
                }
            }
            if (skip) continue;
            vert_t* vj = &verts[j];
            assert(vi->edgecnt < COUNT(vi->edges));
            vi->edges[vi->edgecnt++] = j;
            assert(vj->edgecnt < COUNT(vj->edges));
            vj->edges[vj->edgecnt++] = i;
        }
    }

    sum = bfs(LABEL("nmv"));
    sum2 = bfs(LABEL("thl"));

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
    printf("\n%lld\n", sum * sum2);
}
