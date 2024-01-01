#include <ctype.h>
#include <stdio.h>
#include <string.h>

const char* digits[] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

int decode(const char* b) {
    if (isdigit(b[0])) return b[0] - '0';
    for (int i = 0; i < 9; i++) {
        if (!strncmp(b, digits[i], strlen(digits[i]))) {
            return i + 1;
        }
    }
    return -1;
}

int main()
{
    FILE* f = fopen("day1.txt", "r");
    char b[256];
    int sum = 0;
    while (fgets(b, sizeof(b), f)) {
        //printf("%s\n", b);
        int first = -1, last = -1;
        for (int i = 0; i < strlen(b); i++) {
            int v = decode(b + i);
            if (v < 0) continue;
            if (first < 0) first = v;
            last = v;
        }
        printf("%d %d\n", first, last);
        sum += first * 10 + last;
    }
    printf("%d\n", sum);
}
