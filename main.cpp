#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "common.h"

using namespace std;
static int bytes_per_row = 10;
#define BASE_ADDRESS	0x9F000000
#define BASE_FILE		(BASE_ADDRESS + 1024)

void usage(char* name)
{
#ifdef M2F
	printf("\tExtract file from physical memory.\n\n");
	printf("Usage: %s [-b base_address]\n", name);
	printf("\t -b base_address  base physical address \n");
	printf("\t                  default is 0x%x\n\n", BASE_ADDRESS);
#else
	printf("\tCreate hex file to be loaded into memory\n\n");
	printf("Usage: %s [-b base_address] filename\n", name);
    printf("\t -b base_address  base physical address \n");
    printf("\t                  default is 0x%x\n", BASE_ADDRESS);
	printf("\t filename         file to be converted\n\n");
#endif	
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
	return val;
}
int main(int argc, char *argv[])
{
	FILE* fpIn = NULL;
	FILE* fpOut = NULL;
	char* szInFile = NULL;
	off_t baseaddress = 0;//BASE_FILE;
	char ch;

	while ((ch = getopt(argc, argv, "b:h?"))!= -1)
	{
		switch (ch) {
		case 'b':
			baseaddress = hex2long(optarg);
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			exit(-1);
		}

	}
#ifdef M2F
	m2f(baseaddress);
#else
    szInFile = argv[optind];
	if (!szInFile) {
		usage(argv[0]);
		exit(-1);
	}
	long len;
	unsigned char* pBuffer = f2m(szInFile, &len);
	if (!pBuffer) {
        exit(-1);
    }
	//convert to hex file
	char outfile[256];
	sprintf(outfile, "%s.bin", szInFile);
	FILE* fpout = fopen(outfile, "wb");
	fwrite(pBuffer, 1, len, fpout);
	fclose(fpout);
	printf("Convert file successfully %s\n", outfile);
	free(pBuffer);
#endif

    return 0;
}

