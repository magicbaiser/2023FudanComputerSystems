#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csim.h"
#include <getopt.h>

// print usage info
void usage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(1);
}

// parse command line and get the parameters
void parseline(int argc, char **argv)
{
    int opt;
    int num = 0;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage();
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            numSet = num;
            break;
        case 'E':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            associativity = num;
            break;
        case 'b':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            blockSize = num;
            break;
        case 't':
            strcpy(filePath, optarg);
            break;
        case ':':
            printf("./csim: Missing required command line argument\n");
            usage();
            break;
        case '?':
            usage();
            break;
        default:
            printf("getopt error");
            exit(1);
            break;
        }
    }
}

void initcache() {
    cache = (struct block**)malloc(sizeof(struct block*) * numSet);
    for (int i = 0; i < numSet; i++)
    {
        cache[i] = (struct block*)malloc(sizeof(struct block) * associativity);
    }
    for (int i = 0; i < numSet; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j].flag = 0;
            cache[i][j].target = 0;
            cache[i][j].count = 0;
        }
    }
}

void freecache()
{
    for (int i = 0; i < numSet; i++)
    {
        free(cache[i]);
    }
    free(cache);
}

void newcache(unsigned int a)
{
    int set = (a / blockSize) % numSet;
    int tar = a / (blockSize * numSet);
    for (int i = 0; i < associativity; i++)
    {
        if (cache[set][i].target == tar && cache[set][i].flag == 1)
        {
            cache[set][i].count = 1;
            if (verbose) 
            {
            	printf(" hit");
			}
			hits++;
            for (int j = 0; j < associativity; j++)
    		{
        		cache[set][j].count++;
    		}
            return;
        }
    }
    if (verbose) 
	{
		printf(" miss");
	}
	misses++;
    for (int i = 0; i < associativity; i++)
    {
        if (cache[set][i].flag == 0)
        {
            cache[set][i].flag = 1;
            cache[set][i].target = tar;
            cache[set][i].count = 1;
            return;
        }
    }
    int min = -1; 
	int minposition = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (cache[set][i].count > min)
        {
            min = cache[set][i].count;
            minposition = i;
        }
    }
    if (verbose) 
	{
		printf(" eviction");
	}
    evictions++;
    cache[set][minposition].flag = 1;
    cache[set][minposition].target = tar;
    cache[set][minposition].count = 1;
    for (int i = 0; i < associativity; i++)
    {
        cache[set][i].count++;
    }
}

void input()
{
	FILE* file = fopen(filePath, "r");
    char c;
    unsigned int address;
    int size;
    while (fscanf(file, " %c %x,%d", &c, &address, &size) > 0)
    {
        if (verbose) 
		{
			printf("%c %x,%d", c, address, size);
		}
		if(c == 'I')
		{
			continue;
		}
		if(c == 'M')
		{
			newcache(address);
			newcache(address);
		}
		else
		{
			newcache(address);
		}
        printf("\n");
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    parseline(argc, argv);
    initcache();
    input();
    printSummary(hits, misses, evictions);
    freecache();
    return 0;
}
