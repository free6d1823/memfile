#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "md5.h"
#include "common.h"

/* return the converted memory, NULL if error */
unsigned char* f2m(const char* infile, long* memlen)
{
	unsigned char * pBuffer = NULL;

	struct stat sb;
	if (lstat(infile, &sb) == -1) {
		fprintf(stderr, "Failed to stat file %s. Error=%d\n", infile, errno);
        return NULL;
    }
    printf(" mod= %lo\n", (unsigned long) sb.st_mode);
    printf(" UID=%ld, GID=%ld\n", (long)sb.st_uid, (long) sb.st_gid);

	char md5[64] = {0};
	if(0 == md5sum(infile, md5)){
		printf("MD5=%s\n", md5);
	}
	
	FILE* fpin = fopen(infile, "rb");
	if (!fpin) {
		fprintf(stderr, "Error open file %s\n", infile);
		return pBuffer;
	}
	fseek(fpin, 0, SEEK_END);
	long length = ftell(fpin);
	if (length <= 0) {
		fprintf(stderr, "%s file length error! Code %d\n", infile, errno);
		return pBuffer;
	}
	printf("File length= %ld bytes\n", length);
	pBuffer = (unsigned char*) malloc(length + FILE_OFFSET);
	char* p = (char*) pBuffer;
	*memlen = length + FILE_OFFSET;
	memset(p,0,*memlen);
	sprintf(p,  "%s\n" /*file name */
				"%lo\n" /* mode*/
				"%ld,%ld\n" /*UID, GIF*/
				"%ld\n"  /* file length in bytes */
				"%s\n\n",    /* MD5 */
				infile, (unsigned long) sb.st_mode,
				(long)sb.st_uid, (long) sb.st_gid,
				length,
				md5);
	p = (char*) pBuffer + FILE_OFFSET;
	if( length != fread(p, 1, length, fpin)){
		fprintf(stderr, "Faile to read file %s.\n", infile);
	}	
	fclose(fpin);	
	return pBuffer;
}	
