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
	
## UPDATES

	The app now does 2 things.
	1. Uses the format string vulnerability to retrieve the saved frame pointer in read_name()
	2. Uses the strcpy stack overflow vulnerability to invoke a shell on the server (not a remote shell)
	
	Next Step: Write a Remote Shell will be invoked by the overflow vulnerability.

## Reverse Shell 

	I have the server2 now creating a remote shell to a listener (in this case NetCat) when the client hits it. 
	What happens is the client uses the buffer overflow to pass in the address of the reverseshell function and
	smashes the stack.
	
	1. In a new shell run Netcat to listen for the remote shell connection
		$ netcat -l -v -p 43690
	2. In a new shell run  ./server2
	3. In a new shell run  ./client2
	4. In the netcat shell run a few shell comands

	
	

	Next step is to turn the reverseshell to asm and then to shell code...
