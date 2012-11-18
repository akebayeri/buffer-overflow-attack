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

  /* Receive a prompt from the server */
  recv(s, (void *)buffer, BLENGTH, MSG_WAITALL);

  /* Display the prompt to the user */
  fputs(buffer, stdout);

  /* Read a response from the user */
  fgets(buffer, BLENGTH, stdin);

  /* Send the response to the server */
  send(s, (void *)buffer, BLENGTH, 0);

  /* Receive a reply from the server */
  recv(s, (void *)buffer, BLENGTH, MSG_WAITALL);

  /* Display the reply to the user */
  fputs(buffer, stdout);
}

/* Send our message to the server */
static void
send_message(int s)
{
  char buffer[BLENGTH];

  /* Receive a prompt from the server */
  recv(s, (void *)buffer, BLENGTH, MSG_WAITALL);

  /* Display the prompt to the user */
  fputs(buffer, stdout);

  /* Read a response from the user */
  fgets(buffer, BLENGTH, stdin);

  /* Send the response to the server */
  send(s, (void *)buffer, BLENGTH, 0);
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
