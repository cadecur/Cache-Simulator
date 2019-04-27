#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// Authors: Cade Curry (ccurry), Ishaan Gandhi (igandhi)

typedef struct{
    int valid;
    long tag;
    int LTU; //keeps track of last time used

} cacheline;


void load(int address, int b, int s, int E, int curTime, cacheline *cache, 
        int* hits, int* misses, int* evictions, int verbose)
        {
            //bit arithmetic to get set bits
            int currentSet = (address >> b) & ((1<<s) - 1);

            //bit arithmetic to get address
            int currentTag = (address >> (b+s)) & ((1<<(64-s-b)) - 1);

            //bit arithmetic to get offset
            // int offset = (address) & ((2<<b) - 1);

            // openSpot represents invalid blocks where we could
            // potentially put in our new block
            int openSpot = -1;

            // leastRecentlyUsed represents the stalest block
            // in this set
            int leastRecentlyUsed = 0;

            for (int i = 0; i < E; ++i){
                cacheline current = cache[E*currentSet + i];
                if (current.valid == 1 && current.tag == currentTag){
                    (*hits)++;
                    current.LTU = curTime;
                    if (verbose) {
                        printf("hit\n");
                    }
                    return;
                }
                if(current.valid == 0){
                    openSpot = i;
                }

                if (current.LTU < cache[E*currentSet + leastRecentlyUsed].LTU){
                    leastRecentlyUsed = i;
                } 

            }

            if (openSpot == -1){
                // need to evict
                openSpot = leastRecentlyUsed;
                cache[E*currentSet + openSpot].valid = 1;
                cache[E*currentSet + openSpot].tag = currentTag;
                cache[E*currentSet + openSpot].LTU = curTime;
                (*evictions)++;
                (*misses)++;
                if (verbose) {
                    printf("miss eviction\n");
                }
            }

            else{
                // no need to evict, we will replace the openSpot with this new value
                cache[E*currentSet + openSpot].valid = 1;
                cache[E*currentSet + openSpot].tag = currentTag;
                cache[E*currentSet + openSpot].LTU = curTime;
                (*misses)++;
                if (verbose) {
                    printf("miss\n");
                }
            }

        }



int main(int argc, char *argv[])
{
    int hits = 0;
    int misses = 0; 
    int evictions = 0;
    int opt;
    int s, E, b;
    const char *t;
    int verbose = 0;
    while ((opt = getopt(argc, argv, "v:s:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            t = optarg;
            break;
        case 'v':
            printf("Verbose mode activated.\n");
            verbose = 1;
        }
    }

    FILE *file;
    file = fopen(t, "r");

    char op;
    unsigned int address, size;

    //Initialize 2D Array
    double cacheSize = E * pow(2,s);
    cacheline *cache = malloc(sizeof(cacheline)*cacheSize);

    int curTime = 0;
    while(fscanf(file, " %c %x, %d\n", &op, &address, &size) != EOF){
        curTime++;

        if(op == 'I'){
            continue;
        }

        if(op == 'L' || op == 'S'){
            load(address, b, s, E, curTime, cache, &hits, &misses, &evictions, verbose);
        }

        if(op == 'M'){
            load(address, b, s, E, curTime, cache, &hits, &misses, &evictions, verbose);
            load(address, b, s, E, curTime, cache, &hits, &misses, &evictions, verbose);
        }

    }
    printf("\n");
    printSummary(hits, misses, evictions);
    free(cache);
    return 0;
}