#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PART 1

#define LABEL(s) ((s[0] & 0x1f) | ((s[1] & 0x1f) << 5))
#define TEXT(l) (text[0] = ((l) & 0x1f) + 0x60, text[1] = ((l) >> 5) + 0x60, text)
char text[3];

int64_t sum = 0, sum2 = 0;
int press;
bool trace = false;

typedef struct {
    int lbl;
    char type;
    int state;
    struct { int lbl, inp; } tgts[8];
    int tgtcnt;
    int inpcnt;
    int tgl[9];
} mod_t;

#define MOD_CAP 1<<10
mod_t mods[MOD_CAP];
int modcnt;

const int button = LABEL("button");
const int broadcaster = LABEL("broadcaster");
const int rx = LABEL("rx");

typedef struct {
    int src;
    int tgt;
    int inp;
    bool lvl;
} pulse_t;

#define PULSE_MASK 63
pulse_t pulses[PULSE_MASK + 1];
int pulserd, pulsewr;

#define PUSH(s, t, i, l) (pulses[pulsewr++ & PULSE_MASK] = (pulse_t){(s), (t), (i), (l)})
#define POP() (pulses[pulserd++ & PULSE_MASK])

void send(mod_t* m, bool lvl)
{
    for (int i = 0; i < m->tgtcnt; i++) {
        assert(pulsewr + 1 != pulserd);
        PUSH(m->lbl, m->tgts[i].lbl, m->tgts[i].inp, lvl);
    }
}

bool step()
{
    assert(pulserd != pulsewr);
    pulse_t p = POP();
    if (!p.lvl) sum++; else sum2++;
#if PART == 1
    if (p.tgt == rx && !p.lvl) return false;
#endif
    if (trace) {
        printf("%s ", TEXT(p.src));
        printf("-%s-> ", p.lvl ? "high" : "low");
        printf("%s\n", TEXT(p.tgt));
    }
    mod_t* m = &mods[p.tgt];
    switch (m->type) {
    case '*':
        send(m, p.lvl);
        break;
    case '%':
        if (!p.lvl) {
            if (!m->tgl[0]) m->tgl[0] = press + 1;
            m->state = !m->state;
            send(m, m->state);
        }
        break;
    case '&':
        m->state &= ~(1 << p.inp);
        m->state |= (p.lvl << p.inp);
        if (p.lvl && !m->tgl[p.inp]) m->tgl[p.inp] = press + 1;
        send(m, m->state != (1 << m->inpcnt) - 1);
        break;
    default:
        break;
    }
    return true;
}

int main()
{
    FILE* f = fopen("day20.txt", "r");
    char b[256];

    while (fgets(b, sizeof(b), f)) {
        char* t = b;
        if (ispunct(*t)) t++;
        int l = LABEL(t);
        mod_t* m = &mods[l];
        m->lbl = l;
        m->type = (l == broadcaster) ? '*' : b[0];
        char* ctx;
        strtok_s(t, ">", &ctx);
        printf("mod %c '%s'", m->type, TEXT(l));
        while ((t = strtok_s(NULL, ", \n", &ctx))) {
            l = LABEL(t);
            m->tgts[m->tgtcnt].lbl = l;
            m->tgts[m->tgtcnt].inp = mods[l].inpcnt++;
            assert(mods[l].inpcnt <= 9);
            m->tgtcnt++;
            assert(m->tgtcnt <= 8);
            printf(" '%s' %x", t, l);
        }
        printf("\n");
    }
    printf("\n");

    bool done = false;
    int limit = PART == 0 ? 1000 : 1000000;
    for (press = 0; !done && press < limit; press++) {
        PUSH(button, broadcaster, 0, 0);
        while (pulserd != pulsewr) {
            if (!step()) done = true;
        }
        if (trace) {
            printf("\n");
            for (int i = 0; i < MOD_CAP; i++) {
                mod_t* m = &mods[i];
                if (m->type != '%') continue;
                printf("%s %s\n", TEXT(m->lbl), m->state ? "on" : "off");
            }
            printf("\n");
        }
    }

    for (int i = 0; i < MOD_CAP; i++) {
        mod_t* m = &mods[i];
        if (!m->lbl) continue;
        printf("%c %s %x |", m->type, TEXT(m->lbl), m->state);
        if (m->type == '%') printf(" %d", m->tgl[0]);
        if (m->type == '&') {
            for (int i = 0; i < m->inpcnt; i++) {
                printf(" %d", m->tgl[i]);
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("\n%lld\n", sum);
    printf("\n%lld\n", sum2);
    printf("\n%lld\n", sum * sum2);
    printf("\n%d\n", press);
    //lcm of rk=3733 cd=3793 zf=3947 qx=4057
}
