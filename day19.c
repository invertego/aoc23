#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

//#define LABEL(s) (s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24))
#define LABEL(s) ((s[0] & 0x1f) | ((s[1] & 0x1f) << 5) | ((s[2] & 0x1f) << 10))

typedef struct {
    unsigned char var;
    unsigned char cnd;
    int cmp;
    int tgt;
} rule_t;

rule_t parserule(char* str)
{
    rule_t r = {0};
    if (strchr(str, ':')) {
        char* ctx;
        r.cnd = strchr(str, '<') != NULL ? 1 : 2;
        
        char var[4] = {0};
        strcpy(var, strtok_r(str, "<>", &ctx));
        const char xmas[] = "xmas";
        r.var = strchr(xmas, var[0]) - xmas;
        //printf(" 0'%s'", var);
        
        sscanf(strtok_r(NULL, ":", &ctx), "%d", &r.cmp);
        //printf(" 1'%d'", r.cmp);
        
        str = strtok_r(NULL, "", &ctx);
    }
    char tgt[4] = {0};
    strcpy(tgt, str);
    r.tgt = LABEL(tgt);
    //printf(" 2'%s'", tgt);
    return r;
}

typedef struct {
    rule_t rules[4];
    int rulecnt;
} flow_t;

flow_t flows[1 << 15];

typedef struct {
    int rate[4];
} part_t;

const int in = LABEL("in");
const int accept = LABEL("A\0");
const int reject = LABEL("R\0");

bool eval(const int* rate)
{
    flow_t* f = &flows[in];
    while (true) {
        for (int i = 0; i < f->rulecnt; i++) {
            rule_t* r = &f->rules[i];
            if (r->cnd != 0) {
                bool cnd =
                    ((r->cnd == 1) && rate[r->var] < r->cmp) ||
                    ((r->cnd == 2) && rate[r->var] > r->cmp);
                if (!cnd) continue;
            }
            if (r->tgt == accept) return true;
            if (r->tgt == reject) return false;
            f = &flows[r->tgt];
            break;
        }
    }
}

typedef int range_t[2];
long long pass;
long long fail;

long long count(range_t* rate)
{
    long long prod = 1;
    for (int i = 0; i < 4; i++) {
        prod *= rate[i][1] - rate[i][0] + 1;
    }
    return prod;
}

long long eval2(range_t* rate, int flow)
{
    if (flow == accept) return pass += count(rate);
    if (flow == reject) return fail += count(rate);
    flow_t* f = &flows[flow];
    while (true) {
        for (int i = 0; i < f->rulecnt; i++) {
            rule_t* r = &f->rules[i];
            if (r->cnd != 0) {
                if (r->cnd == 1) {
                    if (rate[r->var][0] >= r->cmp) continue; // none
                    if (rate[r->var][1] <  r->cmp) {} // all
                    else {
                        range_t rate2[4];
                        memcpy(rate2, rate, sizeof(rate2));
                        rate2[r->var][1] = r->cmp - 1;
                        eval2(rate2, r->tgt);
                        rate[r->var][0] = r->cmp;
                        continue;
                    }
                } else {
                    if (rate[r->var][1] <= r->cmp) continue; // none
                    if (rate[r->var][0] >  r->cmp) {} // all
                    else {
                        range_t rate2[4];
                        memcpy(rate2, rate, sizeof(rate2));
                        rate2[r->var][0] = r->cmp + 1;
                        eval2(rate2, r->tgt);
                        rate[r->var][1] = r->cmp;
                        continue;
                    }
                }
            }
            if (r->tgt == accept) return pass += count(rate);
            if (r->tgt == reject) return fail += count(rate);
            f = &flows[r->tgt];
            break;
        }
    }
}

int main()
{
    FILE* f = fopen("day19.txt", "r");
    char b[256];
    long long sum = 0, sum2 = 0;
    int flowcnt = 0;
    int partcnt = 0;

    while (fgets(b, sizeof(b), f)) {
        if (b[0] == '\n') break;
        *strchr(b, '}') = 0;
        //^[a-z]{2,3}\{([msxa][<>]\d{2,4}:(A|R|[a-z]{2,3}),)+(A|R|[a-z]{2,3})\}$
        char* ctx;
        char* t = strtok_r(b, "{", &ctx);
        char flow[4] = {0};
        strcpy(flow, t);
        //printf("'%s'", flow);
        flow_t* f = &flows[LABEL(flow)];
        while ((t = strtok_r(NULL, ",", &ctx))) {
            f->rules[f->rulecnt++] = parserule(t);
        }
        //printf("\n");
        flowcnt++;
    }

    while (fgets(b, sizeof(b), f)) {
        //{x=1140,m=650,a=75,s=1458}
        int rate[4];
        for (int i = 0; i < 4; i++) {
            sscanf(strchr(b, "xmas"[i]) + 2, "%d", &rate[i]);
            //printf("%d ", rate[i]);
        }
        //printf("\n");
        partcnt++;

        if (eval(rate)) {
            sum += rate[0] + rate[1] + rate[2] + rate[3];
        }
    }

    range_t rs[4] = {{1,4000},{1,4000},{1,4000},{1,4000}};
    eval2(rs, in);

    printf("%d %d\n", flowcnt, partcnt);

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);

    printf("\npass %lld\n", pass);
    printf("\nfail %lld\n", fail);
}
