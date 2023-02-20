#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>


double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.;
}


unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}


unsigned int fastread(char* filename) 
{

int fp;
unsigned long long blocksize = 1024;
unsigned int *buffer = NULL;
buffer = (unsigned int*) malloc(blocksize);
double end_time,total_time;
unsigned int result=0;
unsigned int n;
if((fp = open(filename,O_RDONLY)) < 0)
{
	printf("File cannot be opened successfully");
	exit(1);
}
double start_time = now();

while((n = read(fp, buffer, blocksize*sizeof(unsigned int))) > 0)
{
  result ^= xorbuf(buffer, ceil(n/4));
}

end_time = now();
total_time = end_time - start_time;
close(fp);
printf("It took %f seconds to read the given file.\n", total_time);
return result;
}

int main (int argc, char* argv[]) 
{
    	unsigned int xorvalue;
    	xorvalue = fastread(argv[1]);
    	printf("Xor value is = %08x.\n",xorvalue);

 exit(0);
}

