// randlibsw.c | Kevin Vu
// should contain software implementation of the random number generator
// implement the interface described by the header
// this implementation should also define an initializer and a finalizer function, using
// GCC's “__attribute__ ((constructor))” and “__attribute__ ((destructor))” declaration specifiers.
#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__((constructor)) // 01: // Add __attribute__((constructor))
extern void 				 // 02: static to extern
software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long // 03: static to extern
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
__attribute__ ((destructor)) // 04: // Add __attribute__ ((destructor))
extern void // 05: static to extern
software_rand64_fini (void)
{
  fclose (urandstream);
}