#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct room {
	int num_connections;
	int room_type;
	char* room_name;
	char* connections[6];
};

void make_directory(char directory[]) {

	int id;
	char p_id[10];

	id = getpid();
	
	sprintf(p_id, "%d", id);
	
	strcpy(directory, "./maddens.rooms.");

	strncat(directory, p_id, 50);

	mkdir(directory, 0777);

}

int roomCheck(struct room a, struct room b) {
	
	int i;
	
	if (a.room_name == b.room_name)
		return 0;
	
	for (i = 0; i < a.num_connections; i++) {
		if(a.connections[i] == b.room_name)
			return 0;
	}
	
	return 1;
}


void print_room_info (struct room current) {

	printf("CURRENT LOCATION: ");
	printf(current.room_name);

	printf("\nPOSSIBLE CONNECTIONS: ");
	int i = 0;
	for (i = 0; i < current.num_connections; i++) {
		if (i == (current.num_connections - 1))
			printf("%s\n", current.connections[i]);
		else
			printf("%s, ", current.connections[i]);
	}

}

void play (struct room Map[]) {

	size_t len = 0;
	ssize_t read;

	int steps = 0;
	char* path[20];
	

	struct room* user_location;

	//Find start room
	int i;
	for (i = 0; i < 7; i++) {
		if (Map[i].room_type == 1) {
			user_location = &Map[i];
		}
	}

	//Until we hit the end room, keep going.
	while ((*user_location).room_type != 2) {
		
		//Print out current room and connections.
		print_room_info(*user_location);

		//Ask for user input on where to go.
		char* location = NULL;
		printf("WHERE TO? >");
		getline(&location, &len, stdin);
		printf("\n");	
		if (location[strlen(location) - 1] == '\n') {
			location[strlen(location) - 1] = '\0';
		}
		


		//Check if inputted location matches something in connection array.
		
		int j;
		for (j = 0; j < (*user_location).num_connections; j++) {
			if (strcmp (location, (*user_location).connections[j]) == 0) {

				
				
				int k;
				for (k = 0; k < 7; k++) {
					if (strcmp(location, Map[k].room_name) == 0) {
						path[steps++] = location;
						user_location = &Map[k];
						j = (*user_location).num_connections + 1;
						break;
					}


				}
			}
			
		}
		
		//If connection wasn't found in connection list.
		if (j == ((*user_location).num_connections)) {
				printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		}
		
		

	} 

	//Out of the while loop so we hit the end room.
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
	//print_room_info(*user_location);
	
	for (i = 0; i < steps; i++) {
		printf("%s\n", path[i]);
	}


}



int main () {
	
	int random, num_rooms, i, j;
	int seed = time(NULL);
	srand(seed);

	FILE* file_pointer;

	char directory[25];
	char number[5];
	char temp[30];
	char* room_names[10] = {"Armory", "Atrium", "Kitchen", "Study", "Garden", "Bedroom", "Shop", "Dining Room", "Throne Room", "Hall"};
		
	struct room setup[7];
	struct room Map[7];	

	make_directory(directory);
	
	
	//Create the room files.
	for (i = 0; i < 7; i++) {
		sprintf(number, "/room%d", i+1);
		strncpy(temp, directory, sizeof(temp));
		strncat(temp, number, 26);
		file_pointer = fopen(temp, "w");
		
		while (1) {
			random = rand() % 10;
			
			if (room_names[random] !=  "") {
				setup[i].room_name = room_names[random];
				setup[i].num_connections = 0;
				setup[i].room_type = 0;
				
				room_names[random] = "";
				break;
			}
		}
		
		fclose(file_pointer);
	}
	
	//Fill the setup structs.
	for (i = 0; i < 7; i++) {
		num_rooms = (rand() % 4) + 3;

		if (num_rooms > setup[i].num_connections) {
			j = setup[i].num_connections; 
			while (j < num_rooms) {
				random = rand() % 7;

				if (roomCheck(setup[i], setup[random])) {
					setup[i].connections[j] = setup[random].room_name;
					setup[i].num_connections += 1;
					
					setup[random].connections[setup[random].num_connections] = setup[i].room_name;
					setup[random].num_connections += 1;
					
					j++;
				}
			}
		}
	}
	
	
	//Set up START_ROOM and END_ROOM
	int flag = 1;
	
	do {
		random = rand() % 7;
		if (setup[random].room_type == 0) {
			setup[random].room_type = flag;
			flag++;
		}
	} while (setup[random].room_type != 2);
	
	//Write struct information to files.
	for (i = 0; i < 7; i++) {
		sprintf(number, "/room%d", i+1);
		strncpy(temp, directory, sizeof(temp));
		strncat(temp, number, 26);
		file_pointer = fopen(temp, "w");
		
		fprintf(file_pointer, "ROOM NAME: %s\n", setup[i].room_name);
		
		for(j = 0; j < setup[i].num_connections; j++) {
			fprintf(file_pointer, "CONNECTION %d: %s\n", j+1, setup[i].connections[j]);
		}
		
		//Check struct for room type id.
		char* room_description;

		if (setup[i].room_type == 0)
			room_description = "MID_ROOM";
		else if (setup[i].room_type == 1)
			room_description = "START_ROOM";
		else if(setup[i].room_type == 2)
			room_description = "END_ROOM";

		fprintf(file_pointer, "ROOM TYPE: %s\n", room_description);
		
		fclose(file_pointer);
	}


	//Read the file and fill in the structs.

	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	int is_name_found;
	int room_number;

	//+15 for "/room%d"
	char* roomfile_name = malloc(strlen(directory) + 15);

	//Going through each room in the array of structs.
	for (room_number = 0; room_number < 7; room_number++) {

		is_name_found = 0;
	

		//Initalize everything in struct.
		Map[room_number].num_connections = 0;
		Map[room_number].room_type = 0;

		int loop = 0;
		for (loop = 0; loop < 6; loop++) {
			Map[room_number].connections[loop] = NULL;
		}

		sprintf(roomfile_name, "%s/room%d", directory, room_number+1);
		
		file_pointer = fopen(roomfile_name, "r");
		

		//Read all the lines in the file.
		while ((read = getline(&line, &len, file_pointer)) != -1) {

			int pos = 0;

			//Look for ':' in the line.
			char* found = strchr(line, ':');

			if (found != NULL)
				pos = (found - line + 1) + 2; //(account for the space after : and then next letter is start of content)

			char* temp;
			
		   	temp = strdup(&line[pos-1]);
		
		   	temp[strlen(temp) - 1] = '\0';

		   	//If  it's the first line, it's the room name.
			if (is_name_found == 0) {
				//it's the room name
				Map[room_number].room_name = temp;
				is_name_found = 1;

			}
			
			//Only other line in the file that starts with 'R' is the room id.
			else if (is_name_found == 1 && line[0] == 'R') {
				//has to be the room type
				if (strcmp(temp, "MID_ROOM") == 0)
					Map[room_number].room_type = 0;
				else if (strcmp(temp, "START_ROOM") == 0)
					Map[room_number].room_type = 1;
				else if(strcmp(temp, "END_ROOM") == 0)
					Map[room_number].room_type = 2;
						
			}
			
			else {
				//has to be a connection
				Map[room_number].connections[Map[room_number].num_connections++] = temp;
				
	
			}

			free(line);
			//we want getline to allocate buffer
			line = NULL;


		}

	}

	fclose(file_pointer);

	//WE CAN PLAY THE GAME NOW
	
	play(Map);


	exit(0);
	return 0;
}
