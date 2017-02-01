
//command [arg1 arg2 ...] [< input_file] [> output_file] [&]

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define token_delim " \t\r\n\a"


int token_buff = 2048;
int last_foreground_exit_status = 0;


char* read_line() {

	//returns getline
	ssize_t buffer = 0;
	char* line = NULL;

	getline(&line, &buffer, stdin);

	return line;


}

char** parse_line(char* line) {

	int index = 0;

	char** all_tokens = malloc(token_buff * sizeof(char*));
	char* token;

	token = strtok(line, token_delim);

	while (token != NULL) {

		all_tokens[index++] = token;

		//If we need to reallocate the pointers.
		if (index >= token_buff) {

			token_buff *= 2;
			all_tokens = realloc(all_tokens, token_buff * sizeof(char*));

		}

		token = strtok(NULL, token_delim);

	}

	//Done finding tokens, NULL terminate the array.
	all_tokens[index] = NULL;


	return all_tokens;


}

int startup(char** argv) {


	pid_t pid, wait_pid;

	int background = 0;
	char* stdin = NULL;
	char* stdout = NULL;

	int out_found = 0;
	int in_found = 0;


	int argc = 0;

	//count up the arguments
	for (argc = 0; argv[argc] != NULL; ++argc);

	//no arguments
	if (argc == 0)
		return -1;

	//set background flags
	if (strcmp(argv[argc - 1], "&") == 0) {
	
		background = 1;
		argv[argc - 1] = NULL;
		argc -= 1;
	
	}

	//while there's still a < or > to take care of
	while (argc > 2 && ((!strcmp(argv[argc - 2], "<")) || (!strcmp(argv[argc - 2], ">")))) {

		if (!strcmp(argv[argc - 2], ">")) {
			stdout = argv[argc - 1];
			out_found = 1;
		}
		
		else if (!strcmp(argv[argc - 2], "<")) {
			stdin = argv[argc - 1];
			in_found = 1;
		}

		argv[argc - 2] = NULL;
		argv[argc - 1] = NULL;

		argc -= 2;

	}



	pid = fork();



	if (pid < 0)
		printf("We had an error forking.\n");

	//child process
	else if (pid == 0) {

		//reroute input and output here
		if (background && !in_found) {
			stdin = "/dev/null";
			in_found = 1;
		}
		
		if (in_found) {
		
			int fd_in;
			fd_in = open(stdin, O_RDONLY, 0);
			
			if (fd_in == -1) {
				printf("cannot open badfile for %s for input.\n", stdin);
				exit(EXIT_FAILURE);
			}

			dup2(fd_in, STDIN_FILENO);
			close(fd_in);

		}

		if (out_found) {

			int fd_out;
			fd_out = creat(stdout, 0644);

			if (fd_out == -1) {
				printf("cannot open %s for output.\n", stdout);
				exit(EXIT_FAILURE);
			}

			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		
		}

		if (execvp(argv[0], argv) == -1)
			printf("We had an error in the child process.\n");

		exit(EXIT_FAILURE);
	}

	//parent process
	else {

		if (!background) {
			do {
				wait_pid = waitpid(pid, &last_foreground_exit_status, WUNTRACED);
			} while (!WIFEXITED(last_foreground_exit_status) && !WIFSIGNALED(last_foreground_exit_status));
		}

		else
			printf("background pid is %d\n", pid);
	}

	return 1;
}




//Exits the shell.
//When run, it must kill any other proceses or jobs that your shell has started before it terminates your shell itself.
int exit_program() {

	return 0;
}

int change_directory(char** arguments) {

	//if no path is provided, we're just going to the home path.
	if (arguments[1] == NULL)
		chdir(getenv("HOME"));



	//else they gave us a path so we'll change directories to that
	else {
		if (chdir(arguments[1]) != 0)
			printf("Error in changing directory.\n");
	}

	return 1;

}


//Status command prints out the exit status or terminating signal of the last foreground process.
void print_status() {

	printf("exit value %d\n", WEXITSTATUS(last_foreground_exit_status));
	last_foreground_exit_status = -1;
}

void inter_handler(int sig_num) {

	if (sig_num == SIGINT) {
		printf(" terminated by signal %d\n", sig_num);
	}
}

void child_handler(int sig_num) {

	pid_t pid;
	pid = wait(NULL);

	if (pid !=  -1)
		printf("background pid %d is done.\n", pid);

}



int main () {

	char* line;
	char** commands;
	
	int keep_going = 0;

	while(1) {

		printf(": ");
		line = read_line();
		commands = parse_line(line);

		signal(SIGINT, inter_handler);
		signal(SIGCHLD, child_handler);

		if (commands[0] ==  NULL || *commands[0] == 35)
			//keep going if theres any empty command or comment.
			;

		else if (strcmp (commands[0], "exit") == 0) {
			exit_program();
			break;
		}
		
		else if (strcmp (commands[0], "cd") == 0)
			change_directory(commands);


		else if (strcmp (commands[0], "status") == 0)
			print_status();


		else
			startup(commands);

	
		free(line);
		free(commands);

	}


	return 0;

}
