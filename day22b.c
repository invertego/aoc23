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
#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef int coord_t[3];

typedef struct {
    coord_t x[2];
    int cons[5];
    int concnt;
    int revs[5];
    int revcnt;
} brick_t ;

brick_t bricks[1500], bricks2[1500];
int brickcnt;

int dirty[1500];
int dirtycnt;

int open[100];
int opencnt;

void print(coord_t* br)
{
    printf("%d %d %d ~ %d %d %d\n", br[0][0], br[0][1], br[0][2], br[1][0]-1, br[1][1]-1, br[1][2]-1);
}

bool intersect(coord_t* a, coord_t* b)
{
    for (int i = 0; i < 3; i++) {
        if (a[0][i] >= a[1][i] ||
            b[0][i] >= b[1][i] ||
            a[0][i] >= b[1][i] ||
            b[0][i] >= a[1][i]) {
            return false;
        }
    }
    return true;
}

int brickcmp(const void* va, const void* vb)
{
    const brick_t* a = va;
    const brick_t* b = vb;
    return a->x[0][2] - b->x[0][2];
}

int main()
{
    FILE* f = fopen("day22.txt", "r");
    char b[256];
    long long sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        coord_t* br = bricks[brickcnt++].x;
        char* ctx;
        char* t = strtok_r(b, ",", &ctx);
        sscanf(t, "%d", &br[0][0]);
        t = strtok_r(NULL, ",", &ctx);
        sscanf(t, "%d", &br[0][1]);
        t = strtok_r(NULL, "~", &ctx);
        sscanf(t, "%d", &br[0][2]);
        t = strtok_r(NULL, ",", &ctx);
        sscanf(t, "%d", &br[1][0]);
        t = strtok_r(NULL, ",", &ctx);
        sscanf(t, "%d", &br[1][1]);
        t = strtok_r(NULL, "\n", &ctx);
        sscanf(t, "%d", &br[1][2]);
        for (int i = 0; i < 3; i++) {
            int min = MIN(br[0][i], br[1][i]);
            int max = MAX(br[0][i], br[1][i]);
            br[0][i] = min;
            br[1][i] = max + 1;
        }
        print(br);
    }
    printf("\n");

    qsort(bricks, brickcnt, sizeof(bricks[0]), brickcmp);

    for (int i = 0; i < brickcnt; i++) {
        brick_t* bri = &bricks[i];
        coord_t* bi = bri->x;

        coord_t drop[2] = {
            {bi[0][0], bi[0][1], 1},
            {bi[1][0], bi[1][1], bi[0][2]}
        };
        int max = 1;

        for (int j = 0; j < brickcnt; j++) {
            if (i == j) continue;
            coord_t* bj = bricks[j].x;
            if (!intersect(drop, bj)) continue;
            if (bj[1][2] > max) {
                max = bj[1][2];
                bri->concnt = 0;
            }
            if (bj[1][2] == max) {
                assert(bri->concnt < COUNT(bri->cons));
                bri->cons[bri->concnt++] = j;
            }
        }

        for (int k = 0; k < bri->concnt; k++) {
            brick_t* brj = &bricks[bri->cons[k]];
            assert(brj->revcnt < COUNT(brj->revs));
            brj->revs[brj->revcnt++] = i;
        }

        if (max < bi[0][2]) {
            int delta = bi[0][2] - max;
            bi[0][2] -= delta;
            bi[1][2] -= delta;
        }
    }

    memcpy(bricks2, bricks, sizeof(bricks));
    for (int is = 0; is < brickcnt; is++) {
        open[opencnt++] = is;

        while (opencnt) {
            int i = open[--opencnt];

            bool safe = true;
            for (int kj = 0; kj < bricks[i].revcnt; kj++) {
                int j = bricks[i].revs[kj];
                brick_t* brj = &bricks[j];
#if PART == 0
                if (brj->concnt != 1) continue;
                if (brj->cons[0] != i) continue;
                safe = false;
                break;
#else
                for (int k = 0; k < brj->concnt; k++) {
                    if (brj->cons[k] == i) {
                        assert(dirtycnt < COUNT(dirty));
                        dirty[dirtycnt++] = j;
                        memmove(&brj->cons[k], &brj->cons[k + 1], (brj->concnt - k - 1) * sizeof(brj->cons[0]));
                        if (--brj->concnt == 0) {
                            assert(opencnt < COUNT(open));
                            open[opencnt++] = j;
                            sum2 += 1;
                            safe = false;
                        }
                        break;
                    }
                }
#endif
            }
            if (safe) {
                //printf("safe %c\n", 'a'+i);
                sum += 1;
            }
        }

        for (int di = 0; di < dirtycnt; di++) {
            int i = dirty[di];
            memcpy(&bricks[i], &bricks2[i], sizeof(bricks[0]));
        }
        dirtycnt = 0;
    }

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
