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

unsigned int fastread(char* filename) 
{

int fp;
unsigned long long blocksize = 8000;
double end_time,total_time;
unsigned int result=0;
unsigned int n;
if((fp = open(filename,O_RDONLY)) < 0)
{
	printf("File cannot be opened successfully");
	exit(1);
}
double start_time = now();

while(1) 
{
        unsigned int *buffer = NULL;
        buffer = (unsigned int*) malloc(blocksize);
        size_t byteA = read(fp,buffer,blocksize);
        if(byteA<1)
        {
            break;
        }
        for(int x = 0; x < blocksize; x++)
        {
          result ^= buffer[x];
        }
        free(buffer);

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

