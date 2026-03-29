#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>


typedef char bool;
#define true    1
#define false   0


/* Helper random functions */
static inline int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}


#endif