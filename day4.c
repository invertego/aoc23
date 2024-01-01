#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int x[25];
    int cnt;
} tuple;

tuple parse(char* b) {
    char* ctx;
    char* t = strtok_r(b, " ", &ctx);
    tuple rgb = {0};
    while (t) {
        int cnt;
        sscanf(t, "%d", &cnt);
        //printf("'%d' ", cnt);
        rgb.x[rgb.cnt++] = cnt;
        t = strtok_r(NULL, " ", &ctx);
    }
    //printf("\n%d\n", rgb.cnt);
    return rgb;
}

int main()
{
    FILE* f = fopen("day4.txt", "r");
    char b[256];
    int counts[202] = {};
    int sum = 0;
    int sum2 = 0;
    char* ctx;
    while (fgets(b, sizeof(b), f)) {
        char* t = strtok_r(b, ":", &ctx);
        if (!t) continue;
        int id;
        sscanf(strchr(t, ' '), "%d", &id);
        printf("id %d\n", id);
        t = strtok_r(NULL, "|", &ctx);
        tuple valid = parse(t);
        t = strtok_r(NULL, "|", &ctx);
        tuple have = parse(t);
        int points = 0;
        int wins = 0;
        for (int i = 0; i < have.cnt; i++) {
            for (int j = 0; j < valid.cnt; j++) {
                if (have.x[i] == valid.x[j]) {
                    points = points ? points * 2 : 1;
                    wins++;
                    break;
                }
            }
        }
        sum += points;
        int cards = counts[id - 1] + 1;
        sum2 += cards;
        for (int i = 0; i < wins; i++) {
            counts[id + i] += cards;
        }
    }
    printf("%d\n", sum);
    printf("%d\n", sum2);
}
