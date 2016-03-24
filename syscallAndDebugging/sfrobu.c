// Kevin Vu | CS 35L | Winter 2015 | Josh | Hwk 7 | sfrobu.c
// sfrobu.c: Rewrite of sfrob.c so that it uses system calls rather than <stdio.h>
// To read standard input and write standard output
#include <stdlib.h> 
#include <stdio.h> // Use <stdio.h> functions to do I/O
#include <unistd.h> // For read & write
#include <sys/stat.h> // fstat() use
#define false 0
#define true !(false)

int numComparisons = 0;

// Write a C function frobcmp that: 
//  takes two arguments a and b of type char const * as input
//  Returns -- | -1: a < b |  0: a == b | 1: a > b
int frobcmp(char const *a, char const *b);

// frobcmpaux:
//  Helper to allow us to use qsort to sort data
//  by being one of the arguments for qsort
int frobcmpaux(const void *a, const void *b);

// blockcheck:
//  Report any allocation of memory errors to stderr
void blockcheck(char *b);

// Exiting with status 0 indicates success
// Exiting with status 1 indicates error
int main()
{
    int buf = 2048;
    int index = 0;
    int size;
    
    // Memory
    char *block; 
    char readIn;
    
    struct stat fileContents;
    fstat(0, &fileContents);
    int sizeOfFile = fileContents.st_size;
    
    // If stdin is a regular file, your program should initially allocate enough 
    // memory to hold all the data in that file all at once, not
    // reallocating memory as you go.
    if (S_ISREG(fileContents.st_mode))
    {
        if (read(0, &readIn, 1) == 0)
        {
            fprintf(stderr, "Comparisons: %d\n", 0);
            return 0;
        }
        
        block = (char*)malloc(sizeOfFile);
        blockcheck(block);
        size = sizeOfFile;

        int i = 0;
        while (i < sizeOfFile)
        {
            read(0, &readIn, 1);
            block[index++] = readIn;  

            // If the regular file grows while you are reading it, 
            // then my program should still be able to allocate more memory 
            // after the initial file size has been read.
            if (index == size)
            {
                block = (char*)realloc(block, buf+size);
                blockcheck(block);
                size = size + buf;
            }
            
            fstat(0, &fileContents);
            sizeOfFile = fileContents.st_size;
            i++;
        }
    }
    
    // For irregular files
    else // (!S_ISREG(fileContents.st_mode))
    {
        if (read(0, &readIn, 1) == 0)
        {
            fprintf(stderr, "Comparisons: %d\n", 0);
            return 0;
        }
        block = (char*)malloc(buf);
        blockcheck(block);
        block[index++] = readIn;
        while ( (read(0, &readIn, 1)) > 0)
        {
            block[index++] = readIn;
            if (index == buf)
            {
                block = (char*)realloc(block, buf*2);
                blockcheck(block);
                buf = buf * 2;
            }
        }
    }

    // Frob'd text lines: series of non-space bytes 
    // followed by a single space -> Append space if necessary
    if (block[index - 1] != 0x20)
    {
        // Reallocate the size if not enough
        if (index == buf)
        {
            block = (char*) realloc(block, buf * 2);
            blockcheck(block);
            buf = buf * 2;
        }
        // Append the SP
        index++;
        block[index - 1] = 0x20;
    }
    
    // Determine number of words to use for following conditional loops
    int amtOfWords = 0;
    int i = 0;
    while (i < index)
    {
        if (block[i] == 0x20)
        {
            amtOfWords++;
        }
        i++;
    }
    
    char **ptrsToWords = (char **) malloc(sizeof(char*)*amtOfWords);
    if (ptrsToWords == NULL)
    {
        fprintf(stderr, "Error allocating memory \n");
        exit(1);
    }
    
    // ptrToBlock points at the start of the block
    char *ptrToBlock = block;
    int counter = 0;
    int a = 0;
    while (a < amtOfWords)
    {
        ptrsToWords[counter] = ptrToBlock;
        counter++;
        while (*ptrToBlock != 0x20)
            ptrToBlock++;
        // Advance
        ptrToBlock++; 
        a++;
    }
    
    // "Use qsort to sort data"
    qsort(ptrsToWords, counter, sizeof(char*), frobcmpaux);
    
    int *mem = (int*)malloc(sizeof(int)*amtOfWords);
    int x = 0;
    int y = 1;
    // Print to screen
    int b = 0;
    while (b < amtOfWords)
    {
        char *p1 = ptrsToWords[b];
        while (*p1 != 0x20)
        {
            p1++;
            y++;
        }
        mem[x++] = y;
        y = 1;
        b++;
    }
    
    int w = 0;
    while (w < amtOfWords)
    {
        char *p2 = ptrsToWords[w];
        int z = 0;
        while (z < mem[w])
        {
            // System Call
            write(1, &(*p2), 1);
            p2++;
            z++;
        }
        w++;
    }
    
    // Clean Up items that were dynamically allocated
    free(ptrsToWords);
    free(block);
    free(mem);
    
    // Print
    // Exactly 1 space !! 
    // And use base 10 without leading zeros. Terminated with a new line.
    fprintf(stderr, "Comparisons: %d\n", numComparisons);

    // Successful exit
    exit(0);
}

int frobcmp(char const *a, char const *b)
{
    numComparisons++;
    // Each argument points to an array of non-space 
    // bytes followed by a space byte
    const char *arr_one = *(const char **)a; 
    const char *arr_two = *(const char **)b;
    
    // While it remains true that neither a nor b 
    // are met by space characters
    for ( ; ; arr_one++, arr_two++)
    {
        // Cast 
        int sequence_one = (int)*arr_one;
        int sequence_two = (int)*arr_two;
        
        // Case 0: a == b
        if (sequence_one == 0x20 && sequence_two == 0x20)
            return 0;
        // Case 1: a > b
        else if (sequence_two == 0x20)
            return 1;
        // Case -1: a < b
        else if (sequence_one == 0x20)
            return -1;
        
        // Otherwise
        // XOR to retrieve decoded chars
        if ((sequence_one ^ 0x2A) > (sequence_two ^ 0x2A))
            return 1; 
        else if ((sequence_one ^ 0x2A) < (sequence_two ^ 0x2A))
            return -1;
        else // ((sequence_one ^ 0x2A) == (sequence_two ^ 0x2A))
            continue;
    }
}

int frobcmpaux(const void *a, const void *b)
{
    return frobcmp(a, b);
}


void blockcheck(char *b)
{
    if (b == NULL) 
    {
        fprintf(stderr, "Error allocating memory \n");
        exit(1);
    }    
}