#ifndef  _COMMON_H_
#define _COMMON_H_
#include <memory.h>

#define FILE_OFFSET 1024
/* return the converted memory, NULL if error */
unsigned char* f2m(const char* infile, long* mlen);
int m2f(unsigned long baseaddress);
#endif //_COMMON_H_
