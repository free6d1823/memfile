#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "common.h"

using namespace std;
#define SE1000
#ifdef SE1000
#define MEMORY_BASE	    0x000060000000L
#define BASE_ADDRESS	0x00003F000000L
#else
//PC simulation
#define MEMORY_BASE 0x0000000
#define BASE_ADDRESS    0x000000
#endif

#define FILE_OFFSET     0x1000
#define BASE_FILE		(BASE_ADDRESS + FILE_OFFSET)

void usage(char* name)
{
#ifdef M2F
	printf("\tExtract file from physical memory.\n\n");
	printf("Usage: %s [-b base_address] [-f offset] \n", name);
	printf("\t -b base_address  physical address \n");
	printf("\t                  default is 0x%lx\n\n", (MEMORY_BASE+ BASE_ADDRESS));
	printf("\t -f offset        offset of the file content.\n");
	printf("\t                  default is 0x%x\n\n", FILE_OFFSET);
#else
	printf("\tCreate hex file to be loaded into memory\n\n");
	printf("Usage: %s [-b base_address] [-x<n>] filename\n", name);
    printf("  -b base_address  base address relative to RTL node\n");
    printf("                   default is 0x%0lx\n", BASE_ADDRESS);
	printf("  -f offset        offset of the start of file content.\n");
	printf("                   default is 0x%x\n", FILE_OFFSET);
	printf("  -x<n>            convert to hex format\n");
	printf("                   n = 0(bin), 1(32-bit-hex), 2(16-16-hex)\n");
	printf("  filename         file to be converted\n\n");
#endif	
	printf("Layout in memory:\n");
	printf(" [base_address]    file description\n");
	printf("                   <file name>\n");
	printf("                   <file mode>\n");
	printf("                   <user ID>, <group ID>\n");
	printf("                   <bytes of file size in decimal>\n");
	printf("                   <MD5 checksum>\n");
	printf(" [+offset]         file content\n\n");
}
#define char2i(d) (d>= '0' && d <= '9')?(d-'0'): \
					 ( (d >= 'a' && d<= 'f')?(d-'a'+10): \
                       (d >= 'A' && d<= 'F')?(d-'A'+10):0 )
unsigned long long hex2long(char* hex)
{
	unsigned long long val = 0;
	char* p;
	bool start = false;
	for(p=hex; *p != 0; p++) {
		if (!start) {
			if (*p == 'x' || *p == 'X')
				start=true;
			continue;
		}
		val <<= 4;
		val += char2i(*p); 
	}
	if(!start) { //not hex, try decimal
		val = atoi(hex);
	}
	return val;
}
int main(int argc, char *argv[])
{
	FILE* fpIn = NULL;
	FILE* fpOut = NULL;
	char* szInFile = NULL;
#ifdef M2F
	long long baseaddress = (MEMORY_BASE+BASE_ADDRESS);//physical start
#else	
	off_t baseaddress = BASE_ADDRESS; //RTL start
#endif	
	off_t offset = FILE_OFFSET;

	char ch;
	int hexMode = 0;
	while ((ch = getopt(argc, argv, "f:x:b:h?"))!= -1)
	{
		switch (ch) {
		case 'f':
			offset = hex2long(optarg);
			break;
		case 'b':
			baseaddress = hex2long(optarg);
			break;
		case 'x':
			hexMode = atoi(optarg);
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			exit(-1);
		}

	}
#ifdef M2F
	m2f(baseaddress, offset);
#else
	szInFile = argv[optind];
	if (!szInFile) {
		usage(argv[0]);
		exit(-1);
	}
	long len;
	char* pBuffer = f2m(szInFile, &len, (long) offset);
	if (!pBuffer) {
        	exit(-1);
	}
	//convert to hex file
	if (hexMode == 0) {
		char outfile[256];
		sprintf(outfile, "%s.bin", szInFile);
		FILE* fpout = fopen(outfile, "wb");
		fwrite(pBuffer, 1, len, fpout);
		fclose(fpout);
	} else if (hexMode == 1) {
		bin2hex(pBuffer, len, baseaddress, szInFile);
	} else if (hexMode == 2) {
		bin2hex2(pBuffer, len, baseaddress, szInFile);
	}
	printf("Converting file successfully.\n\n");
	free(pBuffer);
#endif

	return 0;
}

