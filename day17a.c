#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 0

#define A(i,j) a[(i)+1][(j)+1]
#define M(i,j) memo[(i)+1][(j)+1]
char a[141+2][141+2];
int memo[141+2][141+2];
int m, n;

char input[141+2][141+2];

const int big = 10000;

int solve(int i, int j)
{
    if (M(i, j) >= 0) return M(i, j);
    if (i < 0 || j < 0 || i >= m || j >= n) return M(i, j) = big;
    if (i == m - 1 && j == n - 1) return M(i, j) = 0;
    int x = solve(i + 1, j) + A(i + 1, j);
    int y = solve(i - 1, j) + A(i - 1, j);
    int z = solve(i, j + 1) + A(i, j + 1);
    int w = solve(i, j - 1) + A(i, j - 1);
    if (y < x) x = y;
    if (z < x) x = z;
    if (w < x) x = w;
    return M(i, j) = x;
}


#define INFINITY 10000
#define UNDEFINED (coord_t){-1,-1}

typedef struct {
    int i, j;
} coord_t;

#define PUSH(x,y) (open[opencnt++] = (coord_t){(x),(y)})
coord_t open[100000];
int opencnt;

int dist[141+2][141+2];
coord_t prev[141+2][141+2];

const int di[] = {-1, 1, 0, 0};
const int dj[] = { 0, 0,-1, 1};

int solve2()
{
    int mindist = INFINITY;
    int maxdist = 0;

    for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) {
        dist[i][j] = INFINITY;
        prev[i][j] = UNDEFINED;
        PUSH(i,j);
    }
    dist[0][0] = 0;

    while (opencnt) {
        int min = INFINITY;
        int uidx = -1;
        coord_t u = {0};
        for (int k = 0; k < opencnt; k++) {
            coord_t tmp = open[k];
            int d = dist[tmp.i][tmp.j];
            if (min > d) {
                min = d;
                u = tmp;
                uidx = k;
            }
        }
        memmove(&open[uidx], &open[uidx + 1], sizeof(open[0]) * (opencnt - uidx - 1));
        opencnt--;
        
        for (int k = 0; k < 4; k++) {
            coord_t v = u;
            v.i += di[k];
            v.j += dj[k];
            //test v in Q
            if (v.i < 0 || v.i >= m ||
                v.j < 0 || v.j >= n) {
                continue;
            }
            int alt = dist[u.i][u.j] + A(v.i,v.j);//Edges(u, v)
            if (alt < mindist) mindist = alt;
            if (alt > maxdist) maxdist = alt;
            if (alt < dist[v.i][v.j]) {
                dist[v.i][v.j] = alt;
                prev[v.i][v.j] = u;
            }
        }
    }

    //coord_t S[1000];
    //int Sidx = 0;
    coord_t u = {m-1,n-1};
    if (prev[u.i][u.j].i >= 0 || (u.i == 0 && u.j == 0)) {
        while (u.i >= 0) {
            input[u.i][u.j] = '*';
            //S[Sidx++] = u;
            u = prev[u.i][u.j];
        }
    }

    printf("min %d max %d\n", mindist, maxdist);

    return dist[m-1][n-1];
}

int main()
{
    FILE* f = fopen("day17.txt", "r");
    char b[256];
    long long sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        *strchr(b, '\n') = 0;
        n = strlen(b);
        memcpy(&input[m], b, n);
        for (int i = 0; i < n; i++) {
            A(m,i) = b[i] - '0';
        }
        m++;
    }

    //memset(memo, -1, sizeof(memo));
    //sum = solve(0, 0);
    sum = solve2();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c", input[i][j]);
        }
        printf("\n");
    }

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
