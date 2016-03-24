// randlibhw.c | Kevin Vu
// Should contain the hardware implementation of the random number generator.
// Implement the interface described by the header
#include "randlib.h"
#include <immintrin.h>

/* Hardware implementation.  */

/* Initialize the hardware rand64 implementation.  */
extern void // 01: Replace static with extern.
hardware_rand64_init (void)
{}

/* Return a random value, using hardware operations.  */
extern unsigned long long // 02: Replace static with extern.
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
extern void // 03: Replace static with extern.
hardware_rand64_fini (void)
{}