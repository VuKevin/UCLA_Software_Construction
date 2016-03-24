// Kevin Vu | CS 35L | Winter 2015 | Josh | Hwk 5 | sfrob.c
// sfrob.c: Take an input as an encoded file, sort the decoded data, 
// and encode the resulting output all WITHOUT decoding or encoding.
#include <stdlib.h> 
#include <stdio.h> // Use <stdio.h> functions to do I/O

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
    // Set Size, Index to track character count
    int stdIn;
    int index = 0;
    int size = 1234;
    // Dynamically allocate memory via malloc (instead of new on c++)
    char *block = (char*) malloc(size);
    
    // Continue to read until EOF
    while ((stdIn = getchar()) != EOF) 
    {
        block[index] = (char) stdIn;
        index++;
        if (index != size)
            continue; // Force the next iteration
        else
        {
            // Reallocate size to hold all the input
            block = (char*) realloc(block, size * 2); 
            blockcheck(block);
            size *= 2;
        }
    }
    
    if ( !(index) ) // If it equals 0, the logical ! should return 1
        exit(0);
        
    // Frob'd text lines: series of non-space bytes 
    // followed by a single space -> Append space if necessary
    if (block[index - 1] != 0x20)
    {
        // Reallocate the size if not enough
        if (index == size)
        {
            block = (char*) realloc(block, size * 2);
            blockcheck(block);
            size *= 2;
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
    
    // Print to screen
    int b = 0;
    while (b < amtOfWords)
    {
        char *p = ptrsToWords[b];
        while (*p != 0x20)
        {
            printf("%c", *p);
            p++;
        }
        printf("%c", *p);
        b++;
    }
    
    // Clean Up items that were dynamically allocated
    free(ptrsToWords);
    free(block);
    
    // Successful exit
    exit(0);
}

int frobcmp(char const *a, char const *b)
{
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