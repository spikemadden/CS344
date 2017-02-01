#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int check_valid_characters (FILE* fp) {

	int character;

	while ((character = fgetc(fp)) != EOF) {

		if ((char)character >= 'A' && (char)character <= 'Z' || (char)character == ' ' || (char)character == '\n')
			continue;

		else {
			printf("invalid character found. It was %c.\n", (char)character);
			return 1;
		}

	}


	return 0;
}


int main () {
	


	FILE* fp;
	int test;

	long lSize;
	char* buffer;
	size_t result;

	struct stat file_info;

	fp = fopen("plaintext4", "rb");

	if (fp == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	test = check_valid_characters(fp);

	if (test) {
		printf("Exiting program.\n");
		exit(1);
	}

	fseek (fp, 0, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);


	buffer = (char*) malloc (sizeof(char)*lSize);
	
	if (buffer == NULL) {
		fputs ("Memory error", stderr);
		exit(2);
	}

	result = fread(buffer, 1, lSize, fp);
	
	if (result != lSize){
		fputs ("Reading error", stderr); 
		exit(3);
	}

	printf("length of result is %d.\n", strlen(buffer));

	printf("buffer has %s in it.\n", buffer);


/*

	fstat(fileno(fp), &file_info);

	int length = file_info.st_size;

	printf("The length of the file is %d.\n", length);

	//char* result = malloc(length + 1);

	char *buffer;
	buffer = (char*) malloc (sizeof(char) * length);

	if (buffer == NULL) {
		fputs ("memory error", stderr); exit (2);}

	//int fread_result = fread(result, sizeof(result), 1, fp);

	//int fread_result = fread(buffer, sizeof(buffer), 1, fp);


	fread(buffer, 1, length, fp); 

	//buffer[strlen(buffer)] = '\0';

	//printf("fread result is %d.\n", fread_result);

	printf("length of result is %d.\n", strlen(buffer));

	printf("buffer has %s in it.\n", buffer);

	

*/

	return 0;

}
