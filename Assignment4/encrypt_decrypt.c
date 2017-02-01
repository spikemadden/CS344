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