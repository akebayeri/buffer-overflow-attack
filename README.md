#assignsecprog

Assignment for Secure Programming

##Contents
	
	Original Folder
		This repository contains all files for the assignement as we were given them
	
	All other files are created as part of the solution

##Installation

	To compile all files use the make command
	$ make

## Run it
	
	Open a shell
	$ ./server

	Open a new shell
	$ ./client

##Whats Going On
		
	I've created copies of the client and server called
	*server2
	*client2
	
	In server2.c
	There seem to be 2 possible vextors of attack 
	1. Line 41 - Format String Vulnerability
	2. Line 64 - Buffer Overflow Vulnerability

	In client.c
	The call to the vulnerable buffer overflow is at line 56

	Here's the plan
	1. Create a reverse shell function and edit the server code to call it directly
		This will verify that we can actually achjieve the attack 
	2. Add a new function dummy() that will just print a String to standard out and try to call it via a stack overflow and verify that the server prints the string 
		-This will verify that we can actually overflow the buffer and make it execute some code
	3. Create the payload from the remote shellcode we created from 1. 
	4. Alter the client2 app at line 56 to send the remote shell code as the payload.