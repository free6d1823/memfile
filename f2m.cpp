#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory.h>
#include "md5.h"
#include "common.h"

/* return the converted memory, NULL if error */
char* f2m(const char* infile, long* memlen)
{
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
		return NULL;
	}
	fseek(fpin, 0, SEEK_END);
	long length = ftell(fpin);
	if (length <= 0) {
		fprintf(stderr, "%s file length error! Code %d\n", infile, errno);
		return NULL;
	}
	printf("File length= %ld bytes\n", length);
	char* pBuffer = (char*) malloc(length + FILE_OFFSET);
	char* p = pBuffer;
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
	p = pBuffer + FILE_OFFSET;
	fseek(fpin, 0, SEEK_SET);
	long read =  fread(p, 1, length, fpin);
	if( length != read ){
		fprintf(stderr, "Failed to read file %s, length=%ld.\n", infile, read);
	}	
	fclose(fpin);	
	return pBuffer;
}	
