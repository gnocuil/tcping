#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>  //struct ip

//#include <sys/types.h>

//#include <sys/ioctl.h>
//#include <net/if.h>
//#include <linux/if_packet.h>


//#include <unistd.h>
//#include <netinet/in.h>  //IPPROTO_RAW,IPPROTO_UDP


#define BUFLEN 2000
unsigned char buf[BUFLEN];

void usage()
{
	puts("Usage : tcping <ip> <port>");
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		usage();
		return 0;
	}
	struct sockaddr_in addr;
	unsigned ip;
	unsigned short port = atoi(argv[2]);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ip = inet_addr(argv[1]);
	int sock;
	int pid;
	struct timeval start, stop, diff;   
    gettimeofday(&start, 0);   
    
  
                    
	if ((pid = fork()) == 0) {//listen
		sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		while (1) {
			int result = recv(sock, buf, BUFLEN, 0);
			if (result < 0) continue;
			int i;
			for (i = 0; i < 20; ++i) printf("%02x ", buf[i + 14]);
			printf("\n");
			struct iphdr *iph = (struct iphdr *)(buf + 14);
			if (iph->saddr == ip && iph->protocol == IPPROTO_TCP) {
				gettimeofday(&stop, 0);
				int time = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
				printf("TIME=%lf ms\n", time / 1000.0);
				break;
			}
		}
		close(sock);
	} else {//tcp connect
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) return 0;
		if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		    printf("connect() failure!\n");
		    return -1;
		}
		char str[] = "Hello World!";
		send(sock, str, sizeof(str), 0);
		close(sock);
		wait(pid);
	}
}
