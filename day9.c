#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PART 1

//#define A(x,y) a[x][y]
#define A(x,y) a[x+2][y+2]

/*
   0 1 2 3 4 n=5
0: 2 3 4 5 6
1: 1 1 1 1 [i][n-i]
2: 0 0 0
m=3
*/

int main()
{
    FILE* f = fopen("day9.txt", "r");
    char b[256];
    int a[25][25];
    int sum = 0;

    while (fgets(b, sizeof(b), f)) {
        memset(a, 0, sizeof(a));
        int n = 0, m = 0;
        char* ctx;
        for (char* t = strtok_s(b, " ", &ctx);
            t;
            t = strtok_s(NULL, " ", &ctx)) {
            sscanf(t, "%d", &A(0,n++));
            //printf("%d ", A(0,n-1));
        }
        //printf("n = %d\n", n);
        for (int i = 1; i < n; i++) {
            bool nonzero = false;
            for (int j = 0; j < n - i; j++) {
                A(i,j) = A(i-1,j+1) - A(i-1,j);
                if (A(i,j)) nonzero = true;
                //printf("%d ", A(i,j));
            }
            //printf("\n");
            m = i + 1;
            if (!nonzero) break;
        }
        //printf("\n");
#if PART == 0
        for (int i = m - 1; i >= 0; i--) {
            int x = A(i,n-i-1);
            int y = A(i+1,n-i-1);
            A(i,n-i) = x + y;
            printf("%d\n", A(i,n-i));
        }
        printf("\n");
        sum += A(0,n);
#else
        for (int i = m - 1; i >= 0; i--) {
            int x = A(i,0);
            int y = A(i+1,-1);
            A(i,-1) = x - y;
            printf("%d\n", A(i,-1));
        }
        printf("\n");
        sum += A(0,-1);
#endif
    }
    printf("%d\n", sum);
}
