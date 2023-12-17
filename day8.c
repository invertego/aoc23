#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PART 1

int edge[26 * 26 * 26][2];
char path[512];
int pathidx;
int pathlen;

int toint(const char* a)
{
    return
        (a[0] - 'A') * 26 * 26 +
        (a[1] - 'A') * 26 +
        (a[2] - 'A');
}

int main()
{
    FILE* f = fopen("day8.txt", "r");
    char b[256];
    int steps = 0;
    int nodes[6];
    int nodecnt = 0;
    
    fgets(path, sizeof(path), f);
    *strchr(path, '\n') = 0;
    pathlen = strlen(path);

    fgets(b, sizeof(b), f); // skip
    while (fgets(b, sizeof(b), f)) {
        //AAA = (BBB, CCC)
        int x = toint(b);
        int y = toint(strchr(b, '(') + 1);
        int z = toint(strchr(b, ',') + 2);
        printf("%d %d %d\n", x, y, z);
        edge[x][0] = y;
        edge[x][1] = z;
        if (b[2] == 'A') {
            nodes[nodecnt++] = x;
        }
    }

#if PART == 0
    const int dest = toint("ZZZ");
    int node = toint("AAA");
    while (node != dest) {
        //int prev = node;
        node = edge[node][path[pathidx] == 'R'];
        steps++;
        pathidx = (pathidx + 1) % pathlen;
        //printf("%d -> %d\n", prev, node);
    }
#elif 0
    for (bool done = false; !done; ) {
        done = true;
        for (int i = 0; i < nodecnt; i++) {
            //int prev = node;
            nodes[i] = edge[nodes[i]][path[pathidx] == 'R'];
            //printf("%d -> %d\n", prev, node);
            if (nodes[i] % 26 != 25) done = false;
        }
        steps++;
        pathidx = (pathidx + 1) % pathlen;
    }
#else
    int* visit = malloc(sizeof(int) * 26 * 26 * 26);
    for (int i = 0; i < nodecnt; i++) {
        int node = nodes[i];
        steps = 0;
        pathidx = 0;
        memset(visit, 0, sizeof(int) * 26 * 26 * 26);
        bool cycle = false;
        int solvecnt = 0, prevsolve = 0;
        for (bool done = false; !done; ) {
            visit[node] = steps + 1;
            //int prev = node;
            node = edge[node][path[pathidx] == 'R'];
            //printf("%d -> %d\n", prev, node);
            steps++;
            pathidx = (pathidx + 1) % pathlen;
            if (node % 26 == 25) {
                printf("solve %d\n", steps - prevsolve);
                if (++solvecnt == 10) done = true;
                prevsolve = steps;
            }
            //if (node == nodes[i]) done = true;
            if (visit[node] && !cycle) {
                printf("cycle %d %d\n", visit[node] - 1, steps);
                cycle = true;
            }
        }
    }
    //23147 19631 12599 21389 17873 20803
    //lcm 21083806112641
#endif

    printf("%d\n", steps);
}
