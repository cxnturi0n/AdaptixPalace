#ifndef _MASK_H_
#define _MASK_H_

#include "loader.h"

char * unmask_rc4(RESOURCE * src, RESOURCE * key);
char * unmask_xor(RESOURCE * src, RESOURCE * key);

#endif