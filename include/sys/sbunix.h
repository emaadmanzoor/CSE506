#ifndef __SBUNIX_H
#define __SBUNIX_H

#include <sys/defs.h>

// location of the time since boot in seconds
#define CLOCK_X 75  // leave some space for digits
#define CLOCK_Y 24

void printf(const char *fmt, ...);
void printat( int x, int y, int val );
#endif
