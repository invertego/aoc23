CFLAGS := -std=c17 -Wall -Wno-unused-result
#CFLAGS += -fsanitize=address
#CFLAGS += -fsanitize=undefined
LDLIBS :=

ifeq ($(d),1)
CFLAGS += -Og -g
ifeq ($(OS),Windows_NT)
CFLAGS += -gcodeview -Wl,-pdb=
endif
else
CFLAGS += -O3
endif

ifeq ($(OS),Windows_NT)
CFLAGS += -D_CRT_SECURE_NO_WARNINGS -Dstrtok_r=strtok_s
else
CFLAGS += -D_POSIX_C_SOURCE=200809L
LDLIBS += -lm
endif

day1: day1.c
day2: day2.c
day3: day3.c
day4: day4.c
day5: day5.c
day6: day6.c
day7: day7.c
day8: day8.c
day9: day9.c
day10: day10.c
day11: day11.c
day12: day12.c
day13: day13.c
day14: day14.c
day15: day15.c
day16: day16.c
day17: day17.c
day18: day18.c
day19: day19.c
day20: day20.c
day21: day21.c
day22: day22.c
day23: day23.c
day24: day24.c
day25: day25.c

clean:
	rm -f *.ilk *.pdb day1 day2 day3 day4 day5 day6 day7 day8 day9 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22 day23 day24 day25
