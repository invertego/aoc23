#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 0

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef int64_t coord_t[3];

typedef union {
    struct { coord_t p, v; };
    coord_t x[2];
} stone_t ;

stone_t stones[1500];
int stonecnt;

void print(coord_t* st)
{
    printf("%lld %lld %lld @ %lld %lld %lld\n",
        st[0][0], st[0][1], st[0][2], st[1][0], st[1][1], st[1][2]);
}

int part1()
{
    int sum = 0;

    for (int i = 0; i < stonecnt; i++)
    for (int j = i + 1; j < stonecnt; j++) {
        coord_t* si = stones[i].x;
        coord_t* sj = stones[j].x;
        double
            x1 = si[0][0],
            y1 = si[0][1],
            x2 = si[0][0] + si[1][0],
            y2 = si[0][1] + si[1][1],
            x3 = sj[0][0],
            y3 = sj[0][1],
            x4 = sj[0][0] + sj[1][0],
            y4 = sj[0][1] + sj[1][1];

        double a1 = (x1-x3)*(y3-y4)-(y1-y3)*(x3-x4);
        double b1 = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
        double a2 = (x1-x3)*(y1-y2)-(y1-y3)*(x1-x2);
        double b2 = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
        if (b1 == 0) continue;
        if (a1 < 0 != b1 < 0) continue;
        if (a2 < 0 != b2 < 0) continue;
        double x = x1 + (x2-x1)*(a1/b1);
        double y = y1 + (y2-y1)*(a1/b1);
        //double x = x3 + (x4-x3)*(a2/b2);
        //double y = y3 + (y4-y3)*(a2/b2);

        //const double min = 7, max = 27;
        const double min = 200000000000000, max = 400000000000000;
        if (x >= min && x <= max && y >= min && y <= max) {
            //print(si);
            //print(sj);
            //printf("\n");
            sum += 1;
        }
    }

    return sum;
}

typedef double vec2[2];
typedef double vec3[3];

double dot3(const double* a, const double* b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void cross3(const double* a, const double* b, double* c)
{
    for (int i = 0; i < 3; i++) {
        int j = (i + 1) % 3, k = (i + 2) % 3;
        c[i] = a[j]*b[k] - a[k]*b[j];
    }
}

void norm3(const double* a, double* b)
{
    double l = sqrt(dot3(a,a));
    b[0] = a[0] / l;
    b[1] = a[1] / l;
    b[2] = a[2] / l;
}

bool intersect2(const double* p1, const double* v1, const double* p2, const double* v2, double* out, double* t)
{
    out[0] = DBL_MAX;
    out[1] = DBL_MAX;
    double
        x1 = p1[0],
        y1 = p1[1],
        x2 = p1[0] + v1[0],
        y2 = p1[1] + v1[1],
        x3 = p2[0],
        y3 = p2[1],
        x4 = p2[0] + v2[0],
        y4 = p2[1] + v2[1];

    double a1 = (x1-x3)*(y3-y4)-(y1-y3)*(x3-x4);
    double b1 = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    //double a2 = (x1-x3)*(y1-y2)-(y1-y3)*(x1-x2);
    //double b2 = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    if (b1 == 0) return false;
    //if (a1 < 0 != b1 < 0) continue;
    //if (a2 < 0 != b2 < 0) continue;
    double x = x1 + (x2-x1)*(a1/b1);
    double y = y1 + (y2-y1)*(a1/b1);
    //double x = x3 + (x4-x3)*(a2/b2);
    //double y = y3 + (y4-y3)*(a2/b2);
    out[0] = x;
    out[1] = y;
    if (t) *t = a1/b1;
    return true;
}

void basis(int nx, int ny, int nz, double* ax, double* ay)
{
    const vec3 ux = {1,0,0};
    const vec3 uy = {0,1,0};
    const vec3 uz = {0,0,1};
    vec3 r = {nx,ny,nz};
    vec3 n;
    norm3(r, n);
    int nmin = nx;
    if (abs(ny) < abs(nmin)) nmin = ny;
    if (abs(nz) < abs(nmin)) nmin = nz;
    if (nmin == nx) {
        cross3(n, ux, ay);
        cross3(ay, n, ax);
    } else if (nmin == ny) {
        cross3(n, uy, ay);
        cross3(ay, n, ax);
    } else {
        cross3(n, uz, ay);
        cross3(ay, n, ax);
    }
}

bool proj_intersect(int i, int j, double scale, const double* ax, const double* ay, double* is_out, double* t)
{
    coord_t* cs[2] = {stones[i].x,stones[j].x};
    vec2 p2[2], v2[2];
    for (int cx = 0; cx < 2; cx++) {
        coord_t* c = cs[cx];
        vec3 p = {c[0][0]/scale,c[0][1]/scale,c[0][2]/scale};
        vec3 v = {c[1][0],c[1][1],c[1][2]};
        vec2 p2_ = {dot3(p, ax),dot3(p, ay)};
        vec2 v2_ = {dot3(v, ax),dot3(v, ay)};
        memcpy(&p2[cx], &p2_, sizeof(p2_));
        memcpy(&v2[cx], &v2_, sizeof(v2_));
    }
    vec2 is;
    bool result = intersect2(p2[0], v2[0], p2[1], v2[1], is, t);
    if (is_out) memcpy(is_out, is, sizeof(is));
    return result;
}

void minimize(int nx, int ny, int nz, double scale, double* smallest, int* small)
{
    vec3 ax, ay;
    basis(nx, ny, nz, ax, ay);
    //printf("%f %f %f | %f %f %f\n", ax[0], ax[1], ax[2], ay[0], ay[1], ay[2]);
    vec2 min = {DBL_MAX,DBL_MAX};
    vec2 max = {-DBL_MAX,-DBL_MAX};
    for (int i = 0; i < stonecnt; i++)
    for (int j = i + 1; j < stonecnt; j++) {
        vec2 is;
        if (!proj_intersect(i, j, scale, ax, ay, is, NULL)) continue;
        //printf("%f %f\n", is[0], is[1]);
        min[0] = MIN(min[0], is[0]);
        min[1] = MIN(min[1], is[1]);
        max[0] = MAX(max[0], is[0]);
        max[1] = MAX(max[1], is[1]);
    }
    //printf("\n");
    double area = (max[0] - min[0]) * (max[1] - min[1]);
    //printf("%2d %2d %2d | %f\n", nx, ny, nz, area);
    if (area < *smallest) {
        *smallest = area;
        small[0] = nx;
        small[1] = ny;
        small[2] = nz;
        printf("smallest %f | %d %d %d | %f %f | %f %f\n", *smallest, small[0], small[1], small[2], min[0], min[1], max[0], max[1]);
    }
}

int64_t part2()
{
    const double scale = 1e12;
    double smallest = DBL_MAX;
    int small[3] = {0};
    for (int nx = -64; nx <= 64; nx++)
    for (int ny = -64; ny <= 64; ny++)
    for (int nz = 1; nz <= 64; nz++) {
        minimize(nx, ny, nz, scale, &smallest, small);
    }
    int tgt[3];// = {1, 55, 17};
    memcpy(tgt, small, sizeof(small));
    for (int fact = 2; fact <= 8; fact++) {
        for (int nx = (tgt[0]-1)*fact; nx <= (tgt[0]+1)*fact; nx++)
        for (int ny = (tgt[1]-1)*fact; ny <= (tgt[1]+1)*fact; ny++)
        for (int nz = (tgt[2]-1)*fact; nz <= (tgt[2]+1)*fact; nz++) {
            minimize(nx, ny, nz, scale, &smallest, small);
        }
    }

    int nx = small[0], ny = small[1], nz = small[2];
    vec3 ax, ay;
    basis(nx, ny, nz, ax, ay);
    vec3 r = {nx,ny,nz};
    int64_t sum = 0;
    for (int i = 0; i < stonecnt; i++) {
        double t;
        for (int j = 0; j < stonecnt; j++) {
            if (i == j) continue;
            if (proj_intersect(i, j, scale, ax, ay, NULL, &t)) break;
        }
        //printf("%f\n", t * scale);
        coord_t* c = stones[i].x;
        int64_t ti = llround(t * scale);
        int64_t oi[3];
        for (int j = 0; j < 3; j++) {
            oi[j] = c[0][j] + (c[1][j] - r[j]) * ti;
        }
        sum = oi[0] + oi[1] + oi[2];
        printf("%lld | %lld %lld %lld\n", sum, oi[0], oi[1], oi[2]);
    }

    return sum;
}

int main()
{
    FILE* f = fopen("day24.txt", "r");
    char b[256];
    int64_t sum = 0, sum2 = 0;

    while (fgets(b, sizeof(b), f)) {
        coord_t* st = stones[stonecnt++].x;
        char* ctx;
        char* t = strtok_s(b, ",", &ctx);
        for (int i = 0; i < 6; i++) {
            sscanf(t, "%lld", &st[i/3][i%3]);
            t = strtok_s(NULL, ", @\n", &ctx);
        }
        //print(st);
    }
    //printf("\n");

    sum = part1();
    stonecnt = MIN(stonecnt, 10);
    sum2 = part2();

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
}
