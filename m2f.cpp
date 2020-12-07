#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

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

int m2f(unsigned long baseaddress)
{
	char source[16] = "hello world!\n";
	vir2phy(baseaddress, source, 16);
	char des[32] = {0};
	phy2vir(des, baseaddress, 16);
	printf(":%s\n", des);
/*
	if(pVert1) munmap(pVert1, 16);
	if(fd1 != -1) close(fd1);
    if(pVert2) munmap(pVert2, 16);
    if(fd2 != -1) close(fd1);

*/
}

