#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    unsigned b, e;
} range;

#define N 10000
range rprev[N];
range rcurr[N];
int rprevcnt, rcurrcnt;

//#define ISEMPTY(p) ((p)->b == (p)->e)
//#define SETEMPTY(p) ((p)->e = (p)->b)
#define ISEMPTY(p) ((p)->b > (p)->e)
#define SETEMPTY(p) ((p)->e = 0, (p)->b = 1)

int main()
{
    FILE* f = fopen("day5.txt", "r");
    char b[256];
    unsigned seeds[20], prev[20], curr[20];
    int seedcnt = 0;
    char* ctx;
    fgets(b, sizeof(b), f);
    char* t = strtok_s(b, ":", &ctx);
    while (true) {
        t = strtok_s(NULL, " ", &ctx);
        if (!t) break;
        unsigned val;
        sscanf(t, "%u", &val);
        if (!(seedcnt & 1))
            rcurr[seedcnt >> 1].b = val;
        else
            rcurr[seedcnt >> 1].e = rcurr[seedcnt >> 1].b + val - 1;
        seeds[seedcnt++] = val;
        //printf("'%u' ", val);
    }
    rcurrcnt = seedcnt >> 1;
    memcpy(curr, seeds, sizeof(seeds));
    memcpy(prev, curr, sizeof(curr));
    bool done = false;
    while (!done) {
        done = !fgets(b, sizeof(b), f);
        if (done || b[0] == '\n') {
            printf("end\n");
            memcpy(prev, curr, sizeof(curr));

            for (int i = 0; i < rprevcnt; i++) {
                range* p = &rprev[i];
                range* c = &rcurr[rcurrcnt];
                if (ISEMPTY(p)) continue;
                c->b = p->b;
                c->e = p->e;
                rcurrcnt++;
            }
            memcpy(rprev, rcurr, rcurrcnt * sizeof(rcurr[0]));
            rprevcnt = rcurrcnt;
            rcurrcnt = 0;
            continue;
        }
        if (strchr(b, ':')) continue;
        unsigned dst, src, cnt;
        sscanf(b, "%u %u %u", &dst, &src, &cnt);
        printf("%u %u %u | %u %u\n", dst, src, cnt, rprevcnt, rcurrcnt);
        for (int i = 0; i < seedcnt; i++) {
            unsigned x = prev[i];
            if (x >= src && x < src + cnt) {
                curr[i] = x - src + dst;
            }
        }
        unsigned sb = src, se = src + cnt - 1;
        for (int i = 0; i < rprevcnt; i++) {
            range* p = &rprev[i];
            range* q = &rprev[rprevcnt];
            range* c = &rcurr[rcurrcnt];
            assert(rprevcnt < N);
            assert(rcurrcnt < N);
            if (ISEMPTY(p)) continue;
            if (i < 50) printf("%u %u | %u %u\n", p->b, p->e, sb, se);
            if (sb <= p->b && p->e <= se) {
                //map whole range
                c->b = p->b - src + dst;
                c->e = p->e - src + dst;
                SETEMPTY(p);
                rcurrcnt++;
            } else if (p->b < sb && se < p->e) {
                //split in 3
                c->b = sb - src + dst;
                c->e = se - src + dst;
                q->b = se + 1;
                q->e = p->e;
                p->e = sb - 1;
                rcurrcnt++;
                rprevcnt++;
            } else if (p->b < sb && sb < p->e + 1 && p->e <= se) {
                //split in 2
                c->b = sb - src + dst;
                c->e = p->e - src + dst;
                p->e = sb - 1;
                rcurrcnt++;
            } else if (p->e > se && sb <= p->b && p->b < se + 1) {
                //split in 2
                c->b = p->b - src + dst;
                c->e = se - src + dst;
                p->b = se + 1;
                rcurrcnt++;
            } else {
                assert(p->e + 1 <= sb || se + 1 <= p->b);
            }
        }
    }
    unsigned min = curr[0];
    for (int i = 0; i < seedcnt; i++) {
        if (curr[i] < min) min = curr[i];
        printf("%u %u\n", seeds[i], curr[i]);
    }
    unsigned min2 = rprev[0].b;
    for (int i = 0; i < rprevcnt; i++) {
        range* p = &rprev[i];
        if (ISEMPTY(p)) continue;
        if (p->b < min2) min2 = p->b;
    }
    printf("%u\n", min);
    printf("%u %u\n", min2, rprevcnt);
}
