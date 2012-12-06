all:

	gcc -g -mpreferred-stack-boundary=2 -o server server.c -lpthread	
	gcc -g -static -mpreferred-stack-boundary=2 -o xstack xstack.c
	./xstack server
	gcc -g -mpreferred-stack-boundary=2 -o exploit exploit.c



