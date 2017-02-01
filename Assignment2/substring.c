#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct room{
		int num_connections;
		int room_type;
		char* room_name;
		char* connections[9];
};


int main() {

	int random, numRooms;
	int seed = time(NULL);

	FILE* file_ptr;

	
	char* room_names[10] = {"Armory", "Atrium", "Kitchen", "Study", "Garden", "Bedroom", "Shop", "Dining Room", "Throne Room", "Hall"};

		
	struct room Map[7];


	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("./test", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	int is_name_found = 0;

	int outerloop = 0;

	for (outerloop = 0; outerloop < 7; outerloop++) {

		is_name_found = 0;
		
		while ((read = getline(&line, &len, fp)) != -1) {

			//printf("Retrieved line of length %zu :\n", read);
			printf("%s", line);

			int pos = 0;

			int match = ':';

			char* found = strchr(line, match);

			if (found != NULL)
				pos = (found - line + 1) + 2; //(account for the space after : and then next letter is start of content)

			printf("the start fo the content is at %d\n", pos);
		
			printf("first characters behind : is %c\n", line[0]);

			//starting from pos, go to end of line to get the substring we want
			char* temp;

			
		   	temp = strdup(&line[pos-1]);

		   	printf("temp is %s\n", temp);




	/*

			EASIER WAY

			if it's the first line
				it's the room name
			if it's the last line
				it's the room type 
			otherwise
				its a connection

	*/
			int i = -1;
			int j = 0;

			if (is_name_found == 0) {
				//it's the room name
				//set is_name to 1
				i++;
				printf("it's the room name\n");
				Map[i].room_name = temp;
				is_name_found = 1;

			}
				
			else if (is_name_found == 1 && line[0] == 'R') {
				//has to be the room type
				printf("it's the room type\n");
				Map[i].room_type = temp;
				
			}
			else {
				//has to be a connection
				printf("it's a connection\n");
				Map[i].connections[j] = temp;
				j++;	
			}

	}



		/*
		   substring in c
		   use strncpy to copy it
			
			int i = 0;

		   for (i = pos; i < strlen(line); i++)
		   room_name[i] = line[i];


		   store room_name in struct	

*/










	}
}


/*

char* match1 = ':';
		//char* match2 = "ME:";
		//char* match3 = "PE:";

		//returns null if not found
		char* found_connection = strchr(line, match1);
		//char* found_name = strchr(line, match2);
		//char* found_type = strchr(line, match3);


		if (found_connection != NULL)
			pos = (found_connection - line + 1) + 2;
		//if (found_name != NULL || found_type != NULL)
			//pos = (found_name - line + 1) + 4;

		if (found_name != NULL)
			pos = (found_name - line + 1) + 4;
		if (found_type != NULL)
			pos = (found_connection - line + 1) + 4;
		


		printf("the start fo the room name is at %d\n", pos);
		int i = 0;
		printf("first characters behind : is %c\n", line[0]);

	   for (i = pos; i < strlen(line); i++)
	   		BUILD THE TEMP SUBSTRING IN HERE


	 
		//if (found_name != NULL)
			printf("we matched with a room name.\n");
			//store string in struct.name
		//if (found_type != NULL)
			//printf("we matched with a room type.\n");
			//store string in struct.type
		//else
			printf("we matched with a connection.\n");
			//store string in struct.connections
	

		//pos = (found-line+1) + 2; //(account for the space after : and then next letter is start of room_name)
*/