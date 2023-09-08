/*
 * csim.c - Cache simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include "cachelab.h"

/* 64-bit address */
#define ADDRBIT 64

/* cache line structure */
typedef struct CacheLine {
    int valid;
    int tag;
    int lru;
} CacheLine;

/*
 * count # of hits, misses, evictions
 */
int main(int argc, char *argv[]) {
    int hit = 0, miss = 0, evict = 0;
    int opt, v = 0, s = 0, E = 0, b = 0, S, size, sel, maxlru;
    long addr, tag, index;
    char op;
    FILE *trace;

    /* arguments
     * -s <s>: # of set index bits
     * -E <E>: associativity
     * -b <b>: # of block bits
     * -t <tracefile>: name of trace file
     */
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                S = pow(2, s);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace = fopen(optarg, "r");
                break;
        }
    }
    if (trace == NULL) {
        printf("wrong filename\n");
        exit(1);
    }
    if (!s || !E || !b) {
        printf("missing argument\n");
        exit(1);
    }

    // allocate cache sets
    CacheLine **cache = (CacheLine **)malloc(S * sizeof(CacheLine *));
    for (int i = 0; i < S; i++) {
        cache[i] = (CacheLine *)malloc(E * sizeof(CacheLine));
    }

    // execute operations
    while (!feof(trace)) {
        // ignore 'I' operations
        char c = fgetc(trace);
        if (feof(trace)) { break; }
        if (c != ' ') {
            while (fgetc(trace) != '\n');
            continue;
        }

        // check hit/miss/eviction
        op = fgetc(trace);
        if (op == 'L' || op == 'S' || op == 'M') {
            fscanf(trace, "%lx,", &addr);
            fscanf(trace, "%d", &size);
            while (fgetc(trace) != '\n');

            if (v) { printf("%c %lx,%d", op, addr, size); }

            tag = addr / (long)pow(2, s + b);
            index = (addr / (long)pow(2, b)) % (long)pow(2, s);

            sel = -1;
            for (int i = 0; i < E; i++) {
                // hit
                if (cache[index][i].valid && tag == cache[index][i].tag) {
                    sel = i;
                    cache[index][sel].lru = 0;
                    break;
                }
            }
            // lru count (hit)
            if (sel >= 0) {
                if (v) { printf(" hit"); }
                for (int i = 0; i < E; i++) {
                    if (i != sel) {
                        cache[index][i].lru++;
                    }
                }
                hit++;
            } else {
                // miss
                if (v) { printf(" miss"); }
                sel = -1;
                for (int i = 0; i < E; i++) {
                    if (!cache[index][i].valid) {   // empty entry i exists
                        sel = i;
                        cache[index][sel].valid = 1;
                        cache[index][sel].tag = tag;
                        cache[index][sel].lru = 0;
                        break;
                    }
                }
                if (sel < 0) {     // eviction
                    if (v) { printf(" eviction"); }
                    maxlru = 0;
                    for (int i = 0; i < E; i++) {
                        if (cache[index][i].lru >= maxlru) {
                            sel = i;
                            maxlru = cache[index][i].lru;
                        }
                    }
                    cache[index][sel].valid = 1;
                    cache[index][sel].tag = tag;
                    cache[index][sel].lru = 0;
                    evict++;
                }

                // lru count (miss)
                for (int i = 0; i < E; i++) {
                    if (i != sel) {
                        cache[index][i].lru++;
                    }
                }
                miss++;
            }
            if (v) { printf("\n"); }
        } else {
            printf("wrong operation\n");
            exit(1);
        }

        // 'M' operation: execute twice
        if (op == 'M') {
            sel = -1;
            for (int i = 0; i < E; i++) {
                // hit
                if (cache[index][i].valid && tag == cache[index][i].tag) {
                    sel = i;
                    cache[index][sel].lru = 0;
                    break;
                }
            }
            // lru count (hit)
            if (sel >= 0) {
                for (int i = 0; i < E; i++) {
                    if (i != sel) {
                        cache[index][i].lru++;
                    }
                }
                hit++;
            } else {
                // miss
                sel = -1;
                for (int i = 0; i < E; i++) {
                    if (!cache[index][i].valid) {   // empty entry i exists
                        sel = i;
                        cache[index][sel].valid = 1;
                        cache[index][sel].tag = tag;
                        cache[index][sel].lru = 0;
                        break;
                    }
                }
                if (sel < 0) {     // eviction
                    maxlru = 0;
                    for (int i = 0; i < E; i++) {
                        if (cache[index][i].lru >= maxlru) {
                            sel = i;
                            maxlru = cache[index][i].lru;
                        }
                    }
                    cache[index][sel].valid = 1;
                    cache[index][sel].tag = tag;
                    cache[index][sel].lru = 0;
                }

                // lru count (miss)
                for (int i = 0; i < E; i++) {
                    if (i != sel) {
                        cache[index][i].lru++;
                    }
                }
                miss++;
            }
        }
    }

    // print result
    printSummary(hit, miss, evict);

    // free & file close
    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }
    free(cache);
    fclose(trace);

    return 0;
}
