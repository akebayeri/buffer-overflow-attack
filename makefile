
all:

	gcc -g -mpreferred-stack-boundary=2 -o ./original/server ./original/server.c -lpthread	
	gcc -g -static -mpreferred-stack-boundary=2 -o ./original/xstack ./original/xstack.c
	gcc -g -mpreferred-stack-boundary=2 -o ./original/client ./original/client.c
	./original/xstack ./original/server

	gcc -g -mpreferred-stack-boundary=2 -o server2 server2.c -lpthread	
	gcc -g -static -mpreferred-stack-boundary=2 -o xstack xstack.c
	gcc -g -mpreferred-stack-boundary=2 -o client2 client2.c
	gcc -S -static -mpreferred-stack-boundary=2 -o shellcode.s shellcode.c
	gcc -g -static -o shellcode shellcode.c
	gcc -g -static -o testremoteshell testshellcode.c
	#gcc -g -static -o connectbackshell connectbackshell.c
	gcc -g -static -mpreferred-stack-boundary=2 -o exploit exploit.c
	./xstack server2

clean:
	rm -f xstack
	rm -f server2
	rm -f client2
