#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void make_directory(char directory[]) {

	int id;
	char p_id[10];

	id = getpid();
	
	sprintf(p_id, "%d", id);

	printf("p_id is %s\n", p_id);
	
	strcpy(directory, "./maddens.rooms.");

	strncat(directory, p_id, 50);

	mkdir(directory, 0777);

}


int main () {

	
	char directory[25];

	make_directory(directory);

	//printf("directory out of function is %s\n", directory);

	FILE* file_ptr;

	char number[5];
	char temp[25];
	char* room_names[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};

	int counter = 0;

	//making files here nigga
	int i = 0;
	for (i = 0; i < 7; i++) {
		sprintf(number, "/%d", i+1);
		strncpy(temp, directory, sizeof(temp));
		strncat(temp, number, 26);
		file_ptr = fopen(temp, "w");
		
		fputs("ROOM NAME: ", file_ptr);

		

	



			while (1) {
				int random = rand() % 10;
				if (room_names[random] !=  "") {
					fputs(room_names[random], file_ptr);
					room_names[random] = "";
					break;
				}
			}
	
	}
	


	


	return 0;

}
