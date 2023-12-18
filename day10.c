#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PART 0

enum {
    N = 1 << 0,
    S = 1 << 1,
    E = 1 << 2,
    W = 1 << 3,
};

const char key[] = "|-LJ7F.S";
unsigned char values[] = {
    N | S | (N | S) << 4,
    E | W | (E | W) << 4,
    N | E | (S | W) << 4,
    N | W | (S | E) << 4,
    S | W | (N | E) << 4,
    S | E | (N | W) << 4,
    0,
    N | S | E | W | (N | S | E | W) << 4,
};

unsigned char map[142][142];
int mapn;
unsigned char visit[142*2][142*2];

#define M(x,y) map[x+1][y+1]
#define V(x,y) visit[x+1][y+1]
#define V_(x,y,u,v) V((x)*2+(u),(y)*2+(v))
#define VL(x,y,u,v) (V_(x,y,0,0) |= 1, V_(x,y,u,v) |= 1)

typedef struct {
    int i, j;
} coord_t;

coord_t open[100000];
int opencnt;

int main()
{
    FILE* f = fopen("day10.txt", "r");
    char b[256];

    int i = 0, j = 0;
    int si = 0, sj = 0;
    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        mapn = strlen(b);
        for (j = 0; j < mapn; j++) {
            if (b[j] == 'S') {
                si = i;
                sj = j;
            }
            int idx = strchr(key, b[j]) - key;
            M(i,j) = values[idx];
        }
        i++;
    }

    i = si;
    j = sj;
    unsigned char mask = ~0;
    int steps = 0;
    while (true) {
        //printf("%d %d\n", i, j);
        if      (mask & N & M(i,j) & (M(i-1,j) >> 4)) { VL(i,j,-1, 0); i--; mask = ~S; }
        else if (mask & S & M(i,j) & (M(i+1,j) >> 4)) { VL(i,j, 1, 0); i++; mask = ~N; }
        else if (mask & W & M(i,j) & (M(i,j-1) >> 4)) { VL(i,j, 0,-1); j--; mask = ~E; }
        else if (mask & E & M(i,j) & (M(i,j+1) >> 4)) { VL(i,j, 0, 1); j++; mask = ~W; }
        else assert(false);
        steps++;
        if (i == si && j == sj) break;
    }

#define PUSH(x,y) (open[opencnt++] = (coord_t){(x),(y)})
#define POP() open[--opencnt]
    PUSH(-1,-1);
    while (opencnt) {
        coord_t c = POP();
        i = c.i, j = c.j;
        V(i,j) |= 1;
        if (i > -1       && !V(i-1,j)) PUSH(i-1,j);
        if (i < mapn*2+1 && !V(i+1,j)) PUSH(i+1,j);
        if (j > -1       && !V(i,j-1)) PUSH(i,j-1);
        if (j < mapn*2+1 && !V(i,j+1)) PUSH(i,j+1);
        assert(opencnt < 100000);
    }

    int inside = 0;
    for (i = 0; i < mapn; i++) {
        for (j = 0; j < mapn; j++) {
            if (!V(i*2,j*2)) inside++;
        }
    }

    printf("%d\n", steps / 2);
    printf("%d\n", inside);
}
