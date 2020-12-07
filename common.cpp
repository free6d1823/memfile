#include <stdlib.h>
#include <stdio.h>
#include "common.h"

/* convert bin to hex file */
int bin2hex(char* buffer, size_t length, size_t start, char* fileroot)
{
	char filename[256];
	sprintf(filename, "%s.hex", fileroot);
	FILE* fp = fopen(filename, "wt");
	char data[10];
	if(fp == NULL) {
		fprintf(stderr, "Faile to create file %s\n", filename);
		return -1;
	}
	sprintf(data, "@%X\n", (unsigned int)start/4);
	fputs(data, fp); 
	char* p = buffer;
	for (int i=0; i<length/4; i++) {
		sprintf(data, "%02X%02X%02X%02X\n",p[0], p[1], p[2], p[3]);
		fputs(data, fp);
		p += 4;
	}
	size_t rem = length%4;
	if (rem > 0 ) {
                switch(rem) {
                case 1:
                        sprintf(data, "%02X000000\n", p[0]);
                        fputs(data, fp);
                        break;
                case 2:
                        sprintf(data, "%02X%02X0000\n", p[0], p[1]);
                        fputs(data, fp);
                        break;
                case 3:
                        sprintf(data, "%02X%02X%02X00\n", p[0], p[1], p[2]);
                        fputs(data, fp);
                        break;
                }
	}	
	fclose(fp);
	return 0;
}

/* bin to interleaved hex files */
int bin2hex2(char* buffer, size_t length, size_t start, char* fileroot)
{
        char filename[256];
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	char data[10];

        sprintf(filename, "cha_%s.hex", fileroot);
        fp1 = fopen(filename, "wt");
        if(fp1 == NULL) {
                fprintf(stderr, "Faile to create file %s\n", filename);
                return -1;
        }
        sprintf(filename, "chb_%s.hex", fileroot);
        fp2 = fopen(filename, "wt");
        if(fp2 == NULL) {
                fprintf(stderr, "Faile to create file %s\n", filename);
		fclose(fp1);
                return -1;
        }

        sprintf(data, "@%X\n", (unsigned int)start/4);
        fputs(data, fp1);
        fputs(data, fp2);


        char* p = buffer;
        for (int i=0; i<length/4; i++) {
                sprintf(data, "%02X%02X\n", p[0], p[1]);
		fputs(data, fp1);
		sprintf(data, "%02X%02X\n", p[2], p[3]);
                fputs(data, fp2);
                p += 4;
        }
        size_t rem = length%4;
        if (rem > 0 ) {
		
		switch(rem) {
		case 1:
			sprintf(data, "%02X00\n", p[0]);
			fputs(data, fp1);
			fputs("0000\n", fp2);
			break;
		case 2:
                        sprintf(data, "%02X%02X\n", p[0], p[1]);
                        fputs(data, fp1);
                        fputs("0000\n", fp2);
                        break;
		case 3:
                        sprintf(data, "%02X%02X\n", p[0], p[1]);
                        fputs(data, fp1);
			sprintf(data, "%02X00\n", p[2]);
                        fputs(data, fp2);
                        break;

                }
        }
        fclose(fp1);
	fclose(fp2);
        return 0;

}

