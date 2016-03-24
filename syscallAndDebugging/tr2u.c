// tr2u.c
// Kevin Vu CS35L Lab 7 | Hwk 7 | Winter 2016
#include <stdio.h> // Contains function prototypes of printf(), scanf(), ...
// #include <unistd.h> // To fix implicit declaration of read and write

// Retrieve the size of the 'byte string' operands
int getSize(char *arg);

int main(int argc, char *argv[])
{
  /* Initial Error Checking */

  // Error if there are NOT two operands/args
  if (argc != 3)
  {
    fprintf(stderr, "Error: Only 2 args are allowed. \n");
    return 1;
  }

  // Error if operands "from" & "to" are not the same length
  int lengthOfFrom = getSize(argv[1]);
  int lengthOfTo = getSize(argv[2]);
  if (lengthOfFrom != lengthOfTo)
  {
    fprintf(stderr, "Error: Args must equal one another in terms of size. \n");
    return 1;
  }

  // Error if there are duplicates in the first argument
  int index0 = 0;
  int index1 = 1;
  char *base = argv[1];
  for (; index0 < lengthOfFrom; index0++)
  {
      for (; index1 <lengthOfFrom; index1++)
	    {
	      char *traverse = base + index1;
	      if (*base == *traverse)
	      {
	        fprintf(stderr, "Error: There are duplicates in 'from'. \n");
	        return 1;
	      }
      }
      base = base + 1;
  }

  int tr;
  char buf;
  while ( read(0, &buf, 1) )
  {
    tr = buf;
    int index;
    char *from = argv[1];
    char *to = argv[2];
    for (index = 0; index < lengthOfFrom; index++)
	  {
	    if (tr == *from)
	      tr = *to;
	    else // if (tr != *from)
	    {
	      from++;
	      to++;
	    }
	   }
    buf = tr;
    write(1, &buf, 1);
  }
  
  return 0;
}

int getSize(char *arg)
{
  int index = 0;
  while (*arg != '\0')
  {
  	index = index + 1;
  	arg = arg + 1;
  }
  return index;
}
