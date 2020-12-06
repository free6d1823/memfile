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

int main(int argc, char *argv[])
{
	FILE* fpIn = NULL;
	FILE* fpOut = NULL;
	char* szInFile = NULL;
	char* baseaddress = NULL;
	char ch;

	while ((ch = getopt(argc, argv, "b:h?"))!= -1)
	{
		switch (ch) {
		case 'b':
			baseaddress = optarg;
			break;
		case 'h':
		case '?':
			usage(argv[0]);
			exit(-1);
		}

	}
#ifdef M2F
printf("Memory to file\n");	

#else
    szInFile = argv[optind];
	if (!szInFile) {
		usage(argv[0]);
		exit(-1);
	}
	unsigned char* pBuffer = f2m(szInFile);
	if (!pBuffer) {
        exit(-1);
    }
	//convert to hex file
	free(pBuffer);
#endif

    return 0;
}

