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

unsigned long long size;

double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.;
}

double funcread(char* filename,unsigned long long blocksize,unsigned long long blockcount) 
{
int fp;
double end_time,total_time;
unsigned int result=0;
double start_time;
if((fp = open(filename,O_RDONLY)) < 0)
{
  printf("File cannot be opened successfully");
  exit(1);
}
if((blocksize*blockcount) > size)
{   
    printf("Please enter a new block size or count to stay within the file size limit\n");
    exit(1);
}

else
{

start_time = now();
for(int i = 0; i < blockcount; i++)
    { 
        unsigned int *buffer = NULL;
        buffer = (unsigned int*) malloc(blocksize);
        read(fp,buffer,blocksize);
        for(int x = 0; x < blocksize; x++)
        {
          result ^= buffer[x];
        }
        free(buffer);
    }
end_time = now();

}

total_time = end_time - start_time;
close(fp);
return total_time;
}





unsigned long long findblockcount(char* filename,unsigned long long blocksize) 
{
    printf("Please wait...finding the right block count for you\n");
    unsigned long long blockcount = 1;
    double timetaken;
    timetaken = funcread(filename,blocksize,blockcount);
 while(1)
  {

    if(5.0 < timetaken && timetaken < 15.0){
    printf("The reading time for the given block count %lld is %f seconds.\n",blockcount,timetaken);
    printf("It can be READ within a reasonable time.\n");
    printf("You can use given block count as %llu to produce results for next experiments.\n",blockcount);
    break;
   }

   else
   {
        blockcount = blockcount*2;
        if((blocksize*blockcount) > size)
        {
        blockcount = blockcount/2;
        printf("Doubling the block count exceeds the file size. Use the last block count as %llu to produce results for next experiments.\n",blockcount);
        break; 
        }

        else
        {
        timetaken = funcread(filename,blocksize,blockcount);
        }

   }

 }
  
  unsigned long long filesize = blocksize*blockcount;
  printf("The output file size is %llu bytes\n",filesize);
return blockcount;   
}


int main (int argc, char* argv[]) 
{ 
    //getting the file size to check if the given block count and size in not beyond the file size. 
    struct stat st;
    stat(argv[1], &st);
    size = st.st_size;
    char *a = argv[2];
    unsigned long long blocksize = atoi(a);
    findblockcount(argv[1],blocksize);
    exit(0);
}