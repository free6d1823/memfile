#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* return the converted memory, NULL if error */
unsigned char* f2m(const char* infile)
{
	unsigned char * pBuffer = NULL;

	struct stat sb;
	if (lstat(infile, &sb) == -1) {
		fprintf(stderr, "Failed to stat file %s. Error=%d\n", infile, errno);
        return NULL;
    }
    printf(" mod= %lo\n", (unsigned long) sb.st_mode);
    printf(" size=%lldx%ld\n", (long long)sb.st_blocks, (long) sb.st_blksize);
    printf(" UID=%ld, GID=%ld\n", (long)sb.st_uid, (long) sb.st_gid);

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
	fclose(fpin);
	return pBuffer;
}	
