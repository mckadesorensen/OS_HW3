#include <stdio.h>
#include <time.h>
#define CACHE_MIN (1024)       /* smallest array to test */
#define CACHE_MAX (4096*4096)  /* largest array to test */
int x[CACHE_MAX];              /* array to stride through */
#define SAMPLE 25

int main(void)
{
    int register i, index, stride, limit, temp=0;
    int steps, tsteps, csize;
    double sec;
    clock_t start, end;
    for ( csize=CACHE_MIN ; csize<=CACHE_MAX ; csize*=2 ) {
        for ( stride=1 ; stride<=csize/2 ; stride*=2 ) {
            sec=0;
            limit = csize-stride+1;
            steps=0;
            do {
                start = clock();
                for ( i=SAMPLE*stride ; i!=0 ; i-- )
                    for ( index=0 ; index<limit ; index+=stride )
                        x[index] = x[index] + 1;
                steps ++;
                end = clock();
                sec += ((double)(end-start)/CLOCKS_PER_SEC);
            } while (sec < 1.0);
            tsteps = 0;
            do {
                start = clock();
                for ( i=SAMPLE*stride ; i!=0 ; i-- )
                    for ( index=0 ; index<limit ; index+=stride )
                        temp = temp + index;
                tsteps ++;
                end = clock();
                sec -= ((double)(end-start)/CLOCKS_PER_SEC);
            } while (tsteps<steps);
            printf("Size:%8d Stride:%8d read+write:%14.2f ns\n",
                   csize*sizeof(int), stride*sizeof(int),
                   (double) sec*1e9/(steps*SAMPLE*stride*((limit-1)/stride+1)));
        }
    }
    printf("Done\n");
    return 0;
}
