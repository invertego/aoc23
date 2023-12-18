#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

char pat[21 * 5 + 1];
int patlen;
int groups[6 * 5];
int groupcnt;
int64_t memo[21 * 5 + 1 + 1][6 * 5 + 1];

bool canplace(int idx, int len)
{
    if (patlen - idx < len) return false;
    for (int i = 0; i < len; i++) {
        if (pat[idx + i] == '.') return false;
    }
    if (idx + len < patlen && pat[idx + len] == '#') return false;
    return true;
}

int64_t solve(int idx, int cnt) {
    assert((char*)&memo[idx][cnt] < (char*)memo + sizeof(memo));
    if (memo[idx][cnt] > -1) return memo[idx][cnt];
    if (cnt == groupcnt) {
        for (int i = idx; i < patlen; i++) {
            if (pat[i] == '#') return memo[idx][cnt] = 0;
        }
#if 0
        int groups2[6 * 5 * 2];
        int groupcnt2 = 0;
        for (int i = 0, j; i < patlen; i++) {
            //printf("%s %d\n", pat, i);
            //assert(pat[i] == '#' || pat[i] == '.');
            for (j = i; j < patlen && pat[j] == '#'; j++) ;
            if (i != j) {
                groups2[groupcnt2++] = j - i;
                i = j;
            }
        }
        assert(groupcnt == groupcnt2);
        assert(!memcmp(groups, groups2, groupcnt * sizeof(groups[0])));
#endif
        return memo[idx][cnt] = 1;
    }
    if (idx >= patlen) return memo[idx][cnt] = 0;
    int64_t sols = 0;
    int len = groups[cnt];
    int64_t a = 0, b = 0;
    char tmp[20];
    assert(len + 1 <= sizeof(tmp));
    if (canplace(idx, len)) {
        memcpy(tmp, pat + idx, len + 1);
        for (int i = 0; i < len; i++) {
            assert(pat[idx + i] != '.');
            pat[idx + i] = '#';
        }
        if (idx + len < patlen) {
            assert(pat[idx + len] != '#');
            pat[idx + len] = '.';
        }
        sols += a = solve(idx + len + 1, cnt + 1);
        memcpy(pat + idx, tmp, len + 1);
    }
    if (pat[idx] != '#') {
        tmp[0] = pat[idx];
        pat[idx] = '.';
        sols += b = solve(idx + 1, cnt);
        pat[idx] = tmp[0];
    }
    //printf("%d %d %d %d\n", idx, cnt, a, b);
    return memo[idx][cnt] = sols;
}

int main()
{
    FILE* f = fopen("day12.txt", "r");
    char b[256];
    int64_t sum = 0;

    while (fgets(b, sizeof(b), f)) {
        //*strchr(b, '\n') = 0;
        char* ctx;
        char* t = strtok_s(b, " ", &ctx);
        strcpy(pat, t);
#if PART == 1
        for (int i = 1; i < 5; i++) {
            strcat(pat, "?");
            strcat(pat, t);
        }
#endif
        patlen = strlen(pat);
        assert(patlen + 1 <= sizeof(pat));
        printf("%s", pat);
        groupcnt = 0;
        while ((t = strtok_s(NULL, ",", &ctx))) {
            int val;
            sscanf(t, "%d", &val);
            groups[groupcnt++] = val;
        }
#if PART == 1
        for (int i = 1; i < 5; i++) {
            memcpy(&groups[groupcnt * i], groups, groupcnt * sizeof(groups[0]));
        }
        groupcnt *= 5;
        assert(groupcnt <= sizeof(groups) / sizeof(groups[0]));
#endif
        for (int i = 0; i < groupcnt; i++) {
            printf(" %d", groups[i]);
        }
        printf("\n");
        memset(memo, -1, sizeof(memo));
        int64_t sols = solve(0, 0);
        sum += sols;
        printf("%lld\n", sols);
        //break;
    }

    printf("\n%lld\n", sum);
}
