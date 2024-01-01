#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

char input[23000];

uint8_t hash(const char* t)
{
    uint8_t h = 0;
    for (int i = 0; t[i]; i++) {
        h += t[i];
        h *= 17;
    }
    return h;
}

typedef struct {
    char label[6 + 1];
    char power;
} lens_t;

typedef struct {
    lens_t lenses[1000];
    int lenscnt;
} box_t;

box_t boxes[256];

int main()
{
    FILE* f = fopen("day15.txt", "r");
    char b[256];
    long long sum = 0;

    while (fgets(b, sizeof(b), f)) {
        char* newline = strchr(b, '\n');
        if (newline) *newline = 0;
        strcat(input, b);
    }

    char* ctx;
    char* t = strtok_r(input, ",", &ctx);
    do {
#if PART == 0
        sum += hash(t);
#else
        char* label = t;
        while (isalpha(*t)) t++;
        char op = *t;
        *t++ = 0;
        uint8_t h = hash(label);
        box_t* box = &boxes[h];
        if (op == '=') {
            int power = *t++ - '0';
            bool match = false;
            for (int i = 0; i < box->lenscnt; i++) {
                lens_t* lens = &box->lenses[i];
                if (!strcmp(lens->label, label)) {
                    lens->power = power;
                    match = true;
                    break;
                }
            }
            if (!match) {
                lens_t* lens = &box->lenses[box->lenscnt++];
                strcpy(lens->label, label);
                lens->power = power;
            }
        } else if (op == '-') {
            for (int i = 0; i < box->lenscnt; i++) {
                lens_t* lens = &box->lenses[i];
                if (!strcmp(lens->label, label)) {
                    memmove(lens, lens + 1, sizeof(lens[0]) * (box->lenscnt - i - 1));
                    box->lenscnt--;
                    break;
                }
            }
        }
        assert(*t == 0);
#endif
    } while ((t = strtok_r(NULL, ",", &ctx)));

    long long sum2 = 0;
    for (int i = 0; i < 256; i++) {
        box_t* box = &boxes[i];
        for (int j = 0; j < box->lenscnt; j++) {
            sum2 += (i + 1) * (j + 1) * box->lenses[j].power;
        }
    }

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
