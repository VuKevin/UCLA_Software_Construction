// randmain.c | Kevin Vu
// main program glues everything tgt
// dynamically link implementation of randlib either
// hardware or software depending on what randcpuid reports
// if any function called by the main program fails,
// the main program should report an error and exit with nonzero status.
#include "randcpuid.h" // but NOT randlib.h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <dlfcn.h>

/* Main program, which outputs N bytes of random data.  */
int main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
    perror (argv[1]);
      else
    valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  /* void (*initialize) (void); */ // 01: DELETE
  unsigned long long (*rand64) (void);
  /* void (*finalize) (void); */ // 02: DELETE
  void *ptr2lib; // 03: ADD -> void *ptr2lib
  char *errorstring; // 04: ADD -> char *errorstring

  // If hardware at hand is supported
  if (rdrand_supported ())
    {
      /* // 05: DELETE
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini; 
      */

      // 06: Then, open the library
      ptr2lib = dlopen("randlibhw.so", RTLD_NOW);
      // 07: Check if !ptr2lib, if so then error handle
      if (ptr2lib == NULL)
      {
        // fprintf(stderr, "%s\n", dlerror());
        printf("Error, unable to open randlibhw.so:%s\n", dlerror());
        exit(1);
      }

      // 08: Locate hardware_rand64()
      rand64 = dlsym(ptr2lib, "hardware_rand64");
      errorstring = dlerror();
      // 09: If there was truly an error when doing dlsym, error handle it
      if (errorstring)
      {
        // fprintf(stderr, "%s\n", errorstring);
        printf("Error, unable to retrieve hardware_rand64():%s\n", errorstring);
        exit(1);
      }
    }
  else // (!rdrand_supported ())
    {
      /* // 10: DELETE
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini; */

      // 11: Then, open the library
      ptr2lib = dlopen("randlibsw.so", RTLD_NOW);
      // 12: Check if !ptr2lib, if so then error handle
      if (ptr2lib == NULL)
      {
        // fprintf(stderr, "%s\n", dlerror());
        printf("Error, unable to open randlibsw.so :%s\n", dlerror());
        exit(1);
      }

      // 13: Locate software_rand()
      rand64 = dlsym(ptr2lib, "software_rand64");
      errorstring = dlerror();
      // 14: If there was truly an error when doing dlsym, error handle it
      if (errorstring)
      {
      // fprintf(stderr, "%s\n", errorstring);
        printf("Error, unable to retrieve software_rand64():%s\n", errorstring);
        exit(1);
      }
    }

  /* initialize (); */ // 15: DELETE
  int wordsize = sizeof rand64 ();
  int output_errno = 0;
  do
  {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (fwrite (&x, 1, outbytes, stdout) != outbytes)
    {
      output_errno = errno;
      break;
    }
      nbytes -= outbytes;
  }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      /* finalize (); */ // 16: DELETE
      return 1;
    }

  /* finalize (); */ // 17: DELETE
  dlclose(ptr2lib); // 18: Close the library "Also, the main program should call dlclose to clean up before exiting. "
  return 0;
}
