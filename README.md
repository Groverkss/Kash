# Kash

## Pre-requisites

You need to install **ncurses.h** library to make this work.
You can install it by using

- `sudo apt-get install libncurses5-dev libncursesw5-dev`

## Usage

1. make
2. ./shell

To delete all .o files, run `make clean`.

## Commands:

* cd [DIRECTORY] : Changed current directory to given directory
* pwd : Prints current directory contents  
* echo : Printf the given arguements 
* ls [al] [DIRECTORY]... : Prints contents of arguements given as directory
* pinfo [pid] : Gives information about a pid
* history <num> : Displays last <num> commands
* nightswatch [OPTION] [COMMAND] : Executes COMMAND (newborn or nightwatch) until 'q' is pressed
* All other executables in PATH variable can be executed
* Background proccesing can be done with '&' at end of command
* The shell support commands seperated by semi-colons

## Assumptions

* Hostname size is at max 256 characters
* Path sizes are at max 512 character
* Max character limit of a executable is 256 characters
* History file should not be deleted during execution of the shell
