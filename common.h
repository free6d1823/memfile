#ifndef  _COMMON_H_
#define _COMMON_H_

#define FILE_OFFSET 1024
/* return the converted memory, NULL if error */
char* f2m(const char* infile, long* mlen);
int m2f(unsigned long baseaddress);
int bin2hex(char* buffer, size_t length, size_t start, char* fileroot);
/* bin to interleaved hex files */
int bin2hex2(char* buffer, size_t length, size_t start, char* fileroot);
#endif //_COMMON_H_
