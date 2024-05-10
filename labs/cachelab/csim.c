#include "cachelab.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

uint32_t global_time = 0;

int v = 0, s, S, b, B, E;

int hit = 0;
int miss = 0;
int eviction = 0;

typedef struct {
    int valid;
    unsigned tag;
    uint32_t timestamp;
}cache_line;

cache_line** cache;

void print_help() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "  -h         Print this help message.\n"
            "  -v         Optional verbose flag.\n"
            "  -s <num>   Number of set index bits.\n"
            "  -E <num>   Number of lines per set.\n"
            "  -b <num>   Number of block offset bits.\n"
            "  -t <file>  Trace file.\n"
            "\n"
            "Examples:\n"
            "  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

void free_cache() {
    if (cache == NULL) {
        return;
    }
    for (int i = 0; i < S; i++) {
        if (cache[i] == NULL) {
            continue;
        }
        free(cache[i]);
    }
    free(cache);
}

void init_cache() {
    if (cache != NULL) {
        free_cache();
    }
    cache = (cache_line**)malloc(S * sizeof(cache_line*));
    for (int i = 0; i < S; i++) {
        cache[i] = (cache_line*)malloc(E * sizeof(cache_line));
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = -1;
            cache[i][j].timestamp = 0;
        }
    }
}

void update_cache(unsigned tag, unsigned index) {
    global_time++;
    for (int i = 0; i < E; i++) {
        if (cache[index][i].tag == tag) {
            hit++;
            cache[index][i].timestamp = global_time;
            if (v) {
                printf("hit ");
            }
            return;
        }
    }
    miss++;

    int last_time = global_time;
    int line = 0;
    for (int i = 0; i < E; i++) {
        if (cache[index][i].valid == 0) {
            cache[index][i].valid = 1;
            cache[index][i].tag = tag;
            cache[index][i].timestamp = global_time;
            printf("miss ");
            return;
        }
        if (cache[index][i].timestamp < last_time) {
            last_time = cache[index][i].timestamp;
            line = i;
        }
    }
    eviction++;
    cache[index][line].tag = tag;
    cache[index][line].timestamp = global_time;
    if (v) {
        printf("miss eviction ");
    }
}

void update_many_caches(unsigned tag, unsigned index, unsigned offset, int size) {
    int lines = (offset + size - 1) / B;
    for (int i = 0; i <= lines; i++) {
        update_cache(tag, index + i);
    }
}

void execute_trace(char identifier, unsigned address, int size) {
    unsigned tag = address >> (s + b);
    unsigned index = (address >> b) & ((1 << s) - 1);
    unsigned offset = address & ((1 << b) - 1);
    switch (identifier) {
        case 'L':
            update_many_caches(tag, index, offset, size);
            if (v) {
                printf("\n");
            }
            break;
        case 'M':
            update_many_caches(tag, index, offset, size);
        case 'S':
            update_many_caches(tag, index, offset, size);
            if (v) {
                printf("\n");
            }
            break;
        default:
            break;
    }
}

void read_trace_file(char* filename) {
    FILE *pFile;
    pFile = fopen(filename, "r");

    char identifier;
    unsigned address;
    int size;

    while (fscanf(pFile, " %c %x,%d", &identifier, &address, &size) > 0) {
        if (v) {
            printf("%c %x,%d ", identifier, address, size);
        }
        execute_trace(identifier, address, size);
    }

    fclose(pFile);
}

int main(int argc, char** argv) {
    int opt;
    char* trace_name;
    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                exit(0);
            case 'v':
                v = 1;
                break;
            case 's':
                s = atoi(optarg);
                S = 1 << s;
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                B = 1 << b;
                break;
            case 't':
                trace_name = (char *)malloc(strlen(optarg) + 1);
                strcpy(trace_name, optarg);
                break;            
            default:
                print_help();
                exit(-1);
        }
    }

    init_cache();
    read_trace_file(trace_name);
    printSummary(hit, miss, eviction);
    
    return 0;
}
