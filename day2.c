#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const char* colors[] = {
    "red", "green", "blue"
};

typedef struct {
    int x[3];
} tuple;

tuple parse(char* b) {
    char* ctx;
    char* t = strtok_r(b, ",", &ctx);
    tuple rgb = {0};
    while (t) {
        //printf("\t%s", t);
        int cnt;
        sscanf(t, "%d", &cnt);
        for (int i = 0; i < 3; i++) {
            if (strstr(t, colors[i])) {
                rgb.x[i] = cnt;
                break;
            }
        }
        t = strtok_r(NULL, ",", &ctx);
    }
    //printf("\n");
    printf("%d %d %d\n", rgb.x[0], rgb.x[1], rgb.x[2]);
    return rgb;
}

int main()
{
    int valid[3] = {12,13,14};
    FILE* f = fopen("day2.txt", "r");
    char b[256];
    int sum = 0;
    int sum2 = 0;
    char* ctx;
    while (fgets(b, sizeof(b), f)) {
        char* t = strtok_r(b, ":", &ctx);
        if (!t) continue;
        int id;
        sscanf(strchr(t, ' '), "%d", &id);
        printf("id %d\n", id);
        bool possible = true;
        int max[3] = {0};
        while (true) {
            t = strtok_r(NULL, ";", &ctx);
            if (!t) break;
            //printf("%s\n", t);
            tuple rgb = parse(t);
            for (int i = 0; i < 3; i++) {
                if (rgb.x[i] > max[i]) {
                    max[i] = rgb.x[i];
                }
                if (rgb.x[i] > valid[i]) {
                    possible = false;
                    //break;
                }
            }
        }
        if (possible) {
            sum += id;
        }
        //printf("max %d %d %d\n", max[0], max[1], max[2]);
        sum2 += max[0] * max[1] * max[2];
    }
    printf("%d\n", sum);
    printf("%d\n", sum2);
}
