#ifndef STDIO
#define STDIO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#endif

int save_game(char *filename, char *text)
{
	FILE *f = fopen(filename, "w");

	fputs(text, f);

	fclose(f);
}

char* new_filename(char *filename, int n)
{
	int i = 0;

	for(i = 0 ; i<3 ; i++)
	{
		*(filename+16-i) = (n%10) + '0';
		n /= 10 * (i+1);
	}

	return filename;
}

void load_game(char *filename, char *output)
{
	FILE *fp;
	char* temp = calloc(6, sizeof(char));

	fp = fopen(filename, "r");
	while ( fgets(temp, 10, fp)!=NULL)
		strcat(output, temp);

	fclose(fp);
}