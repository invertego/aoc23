#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char cards[5];
    int group;
    int bet;
} hand_t;
hand_t hands[1000];
int handcnt;

#define PART 1

#if PART == 0
const char strength[] = "23456789TJQKA";
#else
const char strength[] = "J23456789TQKA";
#endif

int handcmp(const void* va, const void* vb)
{
    const hand_t* a = va;
    const hand_t* b = vb;
    if (a->group != b->group) return a->group - b->group;
    return memcmp(a->cards, b->cards, sizeof(a->cards));
}

int calcgroup(const int* cnts)
{
    int has[5 + 1] = {0};
    for (int i = 0; i < 13; i++) {
        has[cnts[i]]++;
    }

    if (has[5]) return 6;
    else if (has[4]) return 5;
    else if (has[3] && has[2]) return 4;
    else if (has[3]) return 3;
    else if (has[2] == 2) return 2;
    else if (has[2] == 1) return 1;
    else return 0;
}

int main()
{
    FILE* f = fopen("day7.txt", "r");
    char b[256];
    int sum = 0;
    while (fgets(b, sizeof(b), f)) {
        hand_t* h = &hands[handcnt++];
        int cnts[13] = {0};
        for (int i = 0; i < 5; i++) {
            int c = strchr(strength, b[i]) - strength;
            h->cards[i] = c;
            sscanf(strchr(b, ' '), "%d", &h->bet);
            cnts[c]++;
        }
#if PART == 0
        h->group = calcgroup(cnts);
#else
        int jokers = cnts[0];
        int best = 0;
        cnts[0] = 0;
        for (int j = 0; j < 13; j++) {
            cnts[j] += jokers;
            int group = calcgroup(cnts);
            if (group > best) best = group;
            cnts[j] -= jokers;
        }
        h->group = best;
#endif
    }
    qsort(hands, handcnt, sizeof(hands[0]), handcmp);
    for (int i = 0; i < handcnt; i++) {
        sum += (i + 1) * hands[i].bet;
    }
    printf("%d\n", sum);
}
