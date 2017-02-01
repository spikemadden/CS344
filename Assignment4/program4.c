#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* encrypt_decrypt (const char* message, const char* key, int choice) {


	int message_length = strlen(message);

	if (message_length != strlen(key)) {
		printf("Message and key are not the same length.\n");
		exit(1);
	}

	int encrypt = 0;

	if (choice == 1)
		encrypt = 1;

	int* message_and_key = malloc(sizeof(int) * (message_length + 1));
	int* modded = malloc(sizeof(int) * (message_length + 1));
	char* ciphertext_or_message = malloc(message_length + 1);



	int i = 0;

	for (i = 0; i < message_length; i++) {

		int message_letter_converted = message[i] - 'A';
		int key_letter_converted = key[i] - 'A';

		//message has a space in position i and key does not.
		if (message_letter_converted == -33 && key_letter_converted != -33) {

			message_letter_converted = 26;

			if (encrypt) {
				message_and_key[i] = 26 + key_letter_converted;
			}

			else {
				message_and_key[i] = 26 - key_letter_converted;
			}

		}
			


		//key has a space in position i and message does not.
		else if (message_letter_converted != -33 && key_letter_converted == -33) {
			
			key_letter_converted = 26;

			if (encrypt) {
				message_and_key[i] = message_letter_converted + 26;
			}

			else {
				message_and_key[i] = message_letter_converted - 26;
			}

		}
			


		//they both have spaces at position i.
		else if (message_letter_converted == -33 && key_letter_converted == -33) {


			if (encrypt) {
				message_and_key[i] = 26 + 26;
			}

			else {
				message_and_key[i] = 26 - 26;
			}

		}
		

		//handles all cases for capital letters only.
		else {
			if (encrypt) {
				message_and_key[i] = message_letter_converted + key_letter_converted;
			}

			else {
				message_and_key[i] = message_letter_converted - key_letter_converted;
			}

		}

	}

	message_and_key[message_length] = '\0';

	for (i = 0; i < message_length; i++) {

		if (message_and_key[i] < 0)
			message_and_key[i] += 27;

		
		modded[i] = message_and_key[i] % 27;
		

		//printf("modded[%d] has %d in it.\n", i, modded[i]);

	}

	modded[message_length] = '\0';

	for (i = 0; i < message_length; i++) {

		char converted_back = modded[i] + 'A';

		if (converted_back ==  '[')
			converted_back = ' ';

		//printf("character in position %d is %c.\n", i, converted_back);

		ciphertext_or_message[i] = converted_back;

	}

	free(message_and_key);
	free(modded);

	return ciphertext_or_message;


}

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

char* make_key (const char* message) {

	int key_length = strlen(message);

	char* key = malloc(key_length + 1);
	
	int i = 0;

	for (i = 0; i < key_length; i++)
		key[i] = 'A' + random() % 26;

	key[key_length] = '\0';
	
	printf("this is the random key. %s\n", key);

	return key;



}


int main () {
	


	/*FILE* fp;
	int test;

	struct stat file_info;

	fp = fopen("test.txt", "r");

	if (fp == NULL) { 
		printf("Error opening file.\n");
		exit(1);
	}

	test = check_valid_characters(fp);

	if (test) {
		printf("Exiting program.\n");
		exit(1);
	}

	fstat(fileno(fp), &file_info);

	int length = file_info.st_size;

	printf("The length of the file is %d.\n", length);

	char* result = malloc(length + 1);

	int fread_result = fread(result, 1, length, fp);


	result[strlen(result)] = '\0';

	printf("fread result is %d.\n", fread_result);

	printf("length of result is %d.\n", strlen(result));

	*/


	FILE* fp;
	int test;

	long lSize;
	char* buffer;
	//size_t result;

	struct stat file_info;

	fp = fopen("test.txt", "rb");

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

	fread(buffer, 1, lSize, fp);
	
	int j = 0;
	for (j = 0; j < strlen(buffer); j++) {
		printf("the character in position %d is %d\n", j, buffer[j]);
	}

	printf("buffer has this in it. %s\n", buffer);

	if (buffer[strlen(buffer) - 1] == '\n') {
		printf("replace nigga\n");
		buffer[strlen(buffer) - 1] = '\0';
	}

	//make_key("HELLO");


	char* result = malloc(strlen(buffer) + 1);


	//result = encrypt_decrypt(buffer, make_key(buffer), 1);

	result = encrypt_decrypt("U WBPAHDSSF", "NWLRBBMQBHC", 0);

	printf("this is what we got back:\n");


	printf("%s\n", result);

	free (result);

	return 0;


}