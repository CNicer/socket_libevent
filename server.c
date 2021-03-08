#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <event.h>
#include <stdlib.h>

int socketinit()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3300);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr)) == -1)
		return -1;
	if(listen(sockfd,10 == -1))
		return -1;
	return sockfd;
}

void recvfun(int fd,short event,void *arg)
{
	char buff[128]= {0};
	struct event *e = (struct event *)arg;
	int n = recv(fd,buff,128,0);
	if(n>0)
		printf("%s\n",buff);
	else if(n == 0)
	{
		printf("%d dsiconnected\n",fd);
		close(fd);
	}
	else
		perror("recv error");
}

void acceptfun(int fd,short event,void *arg)
{
	struct event_base *eb = (struct event_base*)arg;
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	int c = accept(fd,(struct sockaddr*)&caddr,&len);
	printf("%d connetc\n",c);
	struct event *e = event_new(eb,c,EV_READ|EV_PERSIST,recvfun,e);
	event_add(e,NULL);
	
}

int main()
{
	int sockfd = socketinit();
	if(sockfd == -1)
		perror("sockfd error");
	assert(sockfd != -1);
	
	struct event_base *eb= event_base_new();
	struct event *e= event_new(eb,sockfd,EV_READ|EV_PERSIST,acceptfun,(void *)eb);
	event_add(e,NULL);
	event_base_dispatch(eb);
	event_free(e);
	event_base_free(eb);
	return 1;
}

