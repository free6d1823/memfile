#include <stdlib.h>
#include <stdio.h>
#include "common.h"

/* convert bin to hex file */
int bin2hex(char* buffer, size_t length, size_t start, char* filename)
{
	FILE* fp = fopen(filename, "wt");
	if(fp == NULL) {
		fprintf(stderr, "Faile to create file %s\n", filename);
		return -1;
	}
	fprintf(fp, "@%X\n", start/4);
	char* p = buffer;
	char data[10];
	for (int i=0; i<length/4; i++) {
		sprintf(data, "%2X%2X%2X%2X\n",p[0], p[1], p[2], p[3]);
		fputs(fp, data);
		p += 4;
	}
	size_t rem = length%4;
	if (rem > 0 ) {
		int i;
		for ( i=0; i<rem; i++)
			sprintf(data+i*2, "%2X",*(p+i));
		for (;i<4;i++){
			data[i*2]='0';
			data[i*2+1] = '0';
		}
		data[8]='\n';
		data[9] = 0;

	}	
	fclose(fp);
}
