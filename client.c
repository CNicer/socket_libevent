#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3300);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(res == -1)	
		perror("connect failed");
	assert(res != -1);
	
	char buff[128];
	while(1)
	{
		gets(buff);
		int n = send(sockfd,buff,128,0);
		if(n>0)
			printf("success\n");
	}

	close(sockfd);
}
