/*
 * @File		cli.c
 * @Brief		The file contains functions related to handling the UART on the KL25Z board.
 * 				It also includes the IRQ Hanlder for the UART Tx and Rx and function to bind
 * 				stdio function with the UART using REDLIB library.
 * @Author		Ruchit Naik
 * @Date		08-Nov-2021
 *
 * @Institute	University of Colorado, Boulder
 * @Course		ECEN 8513: Principles of Embedded Software
 *
 * @Attribute
 */

#include "cli.h"

typedef void (*command_handler_t)(int, char *argv[]);

//Look-up table data structure
typedef struct{
	const char *name;
	command_handler_t handler;
	const char *help_string;
}command_table_t;


static void handle_author(int argc, char *argv[]){
	printf("Ruchit Naik \r\n");
}


static void handle_dump(int argc, char *argv[]){
//	uint32_t origin = *argv[1];
	uint32_t origin = 0;
	sscanf(*(&argv[1]), "%x", &origin);
	size_t len = 0;// = *argv[2];
	sscanf(*(&argv[2]), "%i", &len);
	hexdump((void*)origin, len);
}


static void handle_unknown(int argc, char *argv[]){
	printf("Invalid Command\r\n");
}

static void handle_help(int argc, char *argv[]){
	printf("Command: Author ; Arguments: <> ; Description: Prints a string with your name.\r\n");
	printf("Command: Dump ; Arguments: <Start>, <Len> ; Description: Prints a hexdump of the memory requested \r\n");
	printf("Command: Info ; Arguments: <> ; Description: Prints Build Information.\r\n");
}

static void handle_info(int argc, char *argv[]){
//	printf("Version %s built on %s at %s \r\n", VERSION_TAG, BUILD_MACHINE, BUILD_DATE);
//	printf("Commit: %s \r\n", GIT_LOG);
	printf("info coming up\r\n");
}



//Cmd Look-up table
static const command_table_t command[] = {
		{"author", handle_author},
		{"dump", handle_dump},
		{"help", handle_help},
		{"info", handle_info},
		{"", handle_unknown}
};

static const int num_commands = sizeof(command)/sizeof(command_table_t);


void Process_Message(char *input){
	char *ptr = &input[0];
	char *end;

	//To find the end pointer
	for(end = input; *end != '\0'; end++);

	//Tokenize input in place
	bool in_token = false;
	char *argv[10];
	int argc = 0;
	memset(argv, 0, sizeof(argv));		//initializing argv with 0

	for(ptr = input; ptr < end; ptr++){
			if(*ptr == ' '){				//Check on spaces
				if(!in_token){
					ptr++;					//Ignore spaces if not a token
				}
				else{
					*ptr = '\0';			//Fill up the space after token with \0
					in_token = false;
				}
			}
			else{							//Managing the token from the input string token
				if(!in_token){
					argv[argc] = ptr;			//pointing to first argument on the accumulated buffer
					argc++;						//Increment argc for next argument
				}
				in_token = true;
			}
	}

	argv[argc] = NULL;
	if (argc == 0){						//No command entered
		return;
	}

	//TODO: Dispatch argc/argv to handler
	for(int i=0; i<num_commands; i++){
		if(strcasecmp(argv[0], command[i].name) == 0){
			command[i].handler(argc, argv);					//calls corresponding function handler
			ptr = &input[0];
			return;
		}
	}

	handle_unknown(argc, argv);
	ptr = &input[0];
	return;
}
