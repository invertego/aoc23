#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    FILE* f = fopen("day6ex.txt", "r");
    char b[256];
    int a[2][4] = {0};
    int cnt = 0;
    for (int i = 0; i < 2; i++) {
        fgets(b, sizeof(b), f);
        cnt = 0;
#if 1
        for (char *ctx,
            *t = strtok_r(b, ":", &ctx);
            (t = strtok_r(NULL, " ", &ctx));) {
            unsigned val;
            sscanf(t, "%u", &val);
            a[i][cnt++] = val;
            printf("'%u' ", val);
        }
        printf("\n");
#else
        for (int j = 0; j < strlen(b); j++) {
            if (!isdigit(b[j])) continue;
            a[i][0] *= 10;
            a[i][0] += b[j] - '0';
        }
        printf("%d\n", a[i][0]);
        cnt = 1;
#endif
    }
    int prod = 1;
    for (int i = 0; i < cnt; i++) {
        int time = a[0][i];
        int dist = a[1][i];
        int wins = 0;
        for (int t = 0; t <= time; t++) {
            int trav = t * (time - t);
            if (trav > dist) wins++;
            printf("%d %d %d\n", trav > dist, t, trav);
        }
        prod *= wins;
    }
    printf("%d\n", prod);
}
