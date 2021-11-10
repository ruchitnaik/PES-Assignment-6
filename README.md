# PES-Assignment-6  

Code for Assignment 6 for PES, ECEN-5813, Fall 2021. The project contains only the DEBUG build target whose purpose is to set a comand
line interface over the serial communication on KL25Z.  

## Instructions to use  
### UART Configuration
```
Baud rate: 38400  
Data size: 8  
Parity   : None  
Stop Bits: 2  
```  
The commands should be writtern after "?" is printed on the command line. The mark denotes the command line is ready to accept new commands. 
To know the format of valid commands for CLI, the user must first type in the "help" command.

### List of Commands  
- ``` Command: Author ``` | Arg: <> | Brief: Prints a string with your name.  
- ``` Command: Dump ``` | Arg: <Start>, <Len> | Brief: Prints a hexdump of the memory requested; <Start> in hex; <Len> any format.  
- ``` Command: Info ``` | Arg: <> | Brief: Prints Build Information.  
- ``` Command: Help ``` | Arg: <> | Brief: Prints the user help info for all the commands in the lookup table.  

### Editing the Lookup Table  
User can add or edit any CLI commnds by editing the lookup table i.e. ``` command[]``` array. If a new command is to be added, it has to be 
added in the lookup table and the user must define its handler in the same format as other handlers are defined.  

## Development Environment   
- Developed using MCUXpresso on Windows.  
- DEBUG Targer - Prints debug messages to UART terminal.  

## Project Description  
The code for the project implements the following routine:  
We use two different buffers to implement queue for UART transmit and receive. Before starting the main routine we test the queue to check if we 
have a solid circular buffer queue before using it. Once all test cases are passed, the terminal would print "Welcome to BreakfastSerial!" and 
then a "?" which signifies that the terminal is ready to accept new commands. The terminal does not handle backspaces although it could handle 
insuffieint number of spaces between the arguments and also the commands are case insensitive.  

- A circular buffer implementation. You will need two circular buffer objects, one each for the 
transmit and receive directions. These may be allocated statically (the preferred approach), or 
they may be allocated dynamically at initialization time1. Each circular buffer should have a 
capacity of 256 bytes.  
- Test code to exercise your circular buffer. You may wish to adapt the automated tests you 
created for Assignment 2. This code should run at startup if the DEBUG define is set in your 
code, in order to give you confidence that your circular buffers are solid.  
- Code to configure UART0 and send and receive data over it. Parameters for this assignment 
are specified above. Your implementation should be fully interrupt based2. The UART solution 
should be built atop your circular buffer implementation.  
- Glue code that ties your UART communication code into the standard C library functions. 
After this glue code is working, a call to printf() or putchar() on the device should result in 
characters being sent over UART0 to the PC, and a call to getchar() should result in reading a 
character that the user typed on the PC.  
- A command processor that can accept some very simple interactive commands (specified
above) and take action on the device.  

## Output Screenshots  
- COM Port Configuration  
![COM_PORT_SETUP][COM_PORT_SETUP]  

- Terminal Output  
![TERMINAL_OUTPUT][TERMINAL_OUTPUT]

[COM_PORT_SETUP]: https://github.com/ruchitnaik/PES-Assignment-6/blob/master/Screen%20Captures/UART_Config.PNG
[TERMINAL_OUTPUT]: https://github.com/ruchitnaik/PES-Assignment-6/blob/master/Screen%20Captures/Console_Output.PNG






