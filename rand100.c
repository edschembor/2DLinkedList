#include <stdio.h>
#include "rand100.h"

/* Pseudo random generator which returns 1..100. This version depends on the
 * second version of the random number generator. It is independent of the
 * generator used by Get_next_op. */

static unsigned long int next100 =137;

int rand100(void)
{
    int random_number;

    do {
        next100 = next100 *1103515245 + 12345;
    	random_number = (unsigned int)(next100/65536)%32768;
    } while (random_number >= 32700);

    return random_number % 100 + 1;
}

/* Seed the rand100 random number generator. */
void srand100( unsigned int seed )
{
    next100 = seed + 137;
}

