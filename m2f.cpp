#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "md5.h"

char* pVert1 = NULL;
char* pVert2 = NULL;
int fd1 = -1;
int fd2 = -1;
/* return 0 if successfully */
int phy2vir(char* dest, off_t address, size_t length)
{

	fd1 = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd1 == -1) {
		fprintf(stderr, "Failed to open /dev/mem. Error=%d %s\n", errno, strerror(errno));
		return -1;
	}

	pVert1 = (char*) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd1, address);
	if (pVert1== (char*)-1) {
		fprintf(stderr, "mmap failed. Error=%d %s\n", errno, strerror(errno));
		close(fd1);
		return -1;
	}
	memcpy(dest, pVert1, length);
	munmap(pVert1, length);
	close(fd1);
	return 0;
}
int vir2phy(off_t address, char* source, size_t length)
{
    fd2 = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd2 == -1) {
        fprintf(stderr, "Failed to open /dev/mem. Error=%d %s\n", errno, strerror(errno));
        return -1;
    }

    pVert2 = (char*) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd2, address);
    if (pVert2== (char*)-1) {
        fprintf(stderr, "mmap failed. Error=%d %s\n", errno, strerror(errno));
        close(fd2);
        return -1;
    }

	memcpy(pVert2, source, length);
	munmap(pVert2, length);
	close(fd2);
	return 0;
}
#if 0
void simulation(unsigned long baseaddress)
{
	char sample[] = "sample.bin";
	FILE* fpin = fopen(sample, "rb");
    if (!fpin) {
		return;
	}
    fseek(fpin, 0, SEEK_END);
    long length = ftell(fpin);
	char* pBuffer = (char*) malloc(length);
	fseek(fpin, 0, SEEK_SET);
    long read =  fread(pBuffer, 1, length, fpin);
	fclose(fpin);
	vir2phy(baseaddress, pBuffer, length);
	free(pBuffer);

}
#endif

int m2f(unsigned long baseaddress, long foffset)
{
	printf("B=%lX, O=%ld\n", baseaddress, foffset);
#if 0
	simulation(baseaddress);
#endif	
	FILE* fpOut = NULL;
	char* pHeader = (char*) malloc(foffset);
	char* pData = NULL;
	phy2vir(pHeader, baseaddress, foffset);
	do {
		char* p = strchr(pHeader, '\n');
		char* pFileName = NULL;
		if (!p) break;
		pFileName = pHeader;
		*p=0;p++;
		char* p1 = strchr(p, '\n');
		unsigned long mode = 0;
		if(!p1) break;
		*p1++ = 0;
		mode = atoi(p);
		char* guid;
		p = strchr(p1, '\n');
		if(!p) break;
		*p++ = 0;
		guid = p1;
		long fsize = 0;
		p1 = strchr(p, '\n');
		if(!p1) break;
		*p1++ = 0;
		fsize= atoi(p);
		char* md5;
		p = strchr(p1, '\n');
		if(!p) break;
		*p++ = 0;
		md5 = p1;
    	printf("%s\n" /*file name */
                "%lo\n" /* mode*/
                "%s\n" /*UID, GIF*/
                "%ld bytes\n"  /* file length in bytes */
                "%s\n\n",    /* MD5 */
                pFileName, (unsigned long) mode,guid,
                fsize,       md5);
		pData = (char*) malloc(fsize);
		phy2vir(pData, baseaddress+foffset, fsize);
		//
		fpOut = fopen(pFileName, "wb");
		if (!fpOut) {
			fprintf(stderr, "Faile to create file %s\n", pFileName);
			break;
		}
		if(fsize != fwrite(pData, 1, fsize, fpOut)) {
			fprintf(stderr, "Failed to duplicate file %s\n", pFileName);
			break;
		}
		fclose(fpOut);
		fpOut = NULL;
		//check md5
		char checksum[33];
		if ( 0 == md5sum(pFileName, checksum))		
		{
			printf("cmp %s vs %s\n", md5, checksum);
			if (memcmp(md5, checksum, strlen(md5))==0) {
				printf("MD5 check pass: %s.\n", checksum);
			}
		}

	}while (0);
	if(fpOut)
		fclose(fpOut);
	if (pHeader) {
		free(pHeader);
	}
	if (pData) {
		free(pData);
	}

	return 0;
}

