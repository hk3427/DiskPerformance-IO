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

//time calculating function
double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.;
}




//main read function 

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


//read function
int mainread(char* filename,unsigned long long blocksize,unsigned long long blockcount) 
{
double total_time;
total_time = funcread(filename,blocksize,blockcount);
unsigned long long value = (blockcount*blocksize)/(unsigned long long)1048576;
unsigned long long speed = value/total_time;
printf("It took %f seconds to READ the given input file.\n",total_time);
printf("The reading speed for the given file is %llu MiB/s.\n",speed);
return 0;
}





//write function 
int mainwrite(char* filename,unsigned long long blocksize,unsigned long long blockcount) 
{
int fp;
unsigned int *buffer = NULL;
buffer = (unsigned int*) malloc(blocksize);
double end_time,total_time;
if((fp = open(filename,O_WRONLY|O_CREAT)) < 0)
{
	printf("File cannot be opened successfully");
	exit(1);
}

double start_time = now();
for(int i = 0; i < blockcount; i++)
    { 
      write(fp, buffer,blocksize);
    }
end_time = now();
total_time = end_time - start_time;
close(fp);

unsigned long long value = (blockcount*blocksize)/(unsigned long long)1048576;
unsigned long long speed = value/total_time;
printf("It took %f seconds to WRITE the given input file.\n", total_time);
return 0;   
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


//performance measurement(Part 4)
int performance(char* filename,unsigned long long blocksize) 
{
       unsigned long long blockcount = findblockcount(filename,blocksize);
       double timetaken = funcread(filename,blocksize,blockcount);
       unsigned long long value = (blockcount*blocksize)/(unsigned long long)1048576;
       unsigned long long speed = value/timetaken;
       printf("The reading speed for the given blocksize %llu is %llu MiB/s \n",blocksize,speed);
return 0;   
}


//system call experiments
int systemCall(char* filename,unsigned long long blockcount) 
{
    if(blockcount < 1048576)
    {
        printf("Your file size to read is less then 1MiB. Double the blockcount to see some results.\n");
        exit(1);
    }
    unsigned long long blocksize = 1;
    double timetaken = funcread(filename,blocksize,blockcount);
    unsigned long long valueMB = (blockcount*blocksize)/(unsigned long long)1048576;
    unsigned long long speedMB = valueMB/timetaken;
    unsigned long long valueB = (blockcount*blocksize);
    unsigned long long speedB = valueB/timetaken;
    int fp,floc;
    char buf[6];
    double end_time,total_time;
    if((fp = open(filename,O_WRONLY)) < 0)
    {
	printf("File cannot be opened successfully");
	exit(1);
    }
    double start_time = now();
    for(int i = 0; i < blockcount; i++)
    { 
      floc = lseek(fp,i,SEEK_SET);
    }
    end_time = now();
    total_time = end_time - start_time;
    close(fp);
    unsigned long long speedlseek = valueB/total_time;
    printf("The reading speed for the given file is %llu MiB/s.\n", speedMB);
    printf("The reading speed for the given file is %llu B/s.\n",speedB);
    printf("This is how many READ system calls you can do per second.\n");
    printf("You can make %llu LSEEK system calls per second. \n",speedlseek);

return 0; 
}





//main fucntion to handle command line arguments
int main (int argc, char* argv[]) 
{ 
    //getting the file size to check if the given block count and size in not beyond the file size. 
    struct stat st;
    stat(argv[1], &st);
    size = st.st_size;
	if(argc==5 && strcmp(argv[2],"-r") == 0)
	{ 
        char *a = argv[3];
	    char *b = argv[4];
	    unsigned long long blocksize = atoi(a);
	    unsigned long long blockcount = atoi(b);
	    mainread(argv[1],blocksize,blockcount);
	exit(0);
    }
    
    else if(argc==5 && strcmp(argv[2],"-w") == 0)
    {
        char *a = argv[3];
	    char *b = argv[4];
	    unsigned long long blocksize = atoi(a);
	    unsigned long long blockcount = atoi(b);
    	mainwrite(argv[1],blocksize,blockcount);
    exit(0);
    }

    else if(argc==4 && strcmp(argv[2],"-perform") == 0)
    {
        char *a = argv[3];
	    unsigned long long blocksize = atoi(a);
    	performance(argv[1],blocksize);
    }
    else if(argc==4 && strcmp(argv[2],"-system") == 0)
    {
        char *a = argv[3];
	    unsigned long long blockcount = atoi(a);
    	systemCall(argv[1],blockcount);
    exit(0);
    }

    else
      {
    printf("Do you wish to run ./fast <filename> ? \n");
    printf("If yes, please run a separate program named fast.c? \n");
    exit(0);
    }

 exit(0);
}