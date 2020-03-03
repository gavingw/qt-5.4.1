#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	if(argc >1){		
		//printf("--argc is %d-----argv[1] = %s \r\n",argc, argv[1]);
	}
	else{
		printf("input err,pleas inter debug level num 0~6\n");
		return -1;
	}
	int send_data = atoi(argv[1]);
	if((send_data < 0)||(send_data > 6))
	{
		printf("input err,pleas inter debug level num 0~6\n");
		return -1;
	}
	
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "Loglevel_server");
	len = sizeof(address);
	
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1){
		perror("Connect failed,please run the  Loglevel_server server first!\n");
		exit(1);
	}
	
	write(sockfd, &send_data, sizeof(send_data));
	//printf("write socket %s over!len = %d ,data = %d\n", argv[1],sizeof(argv[1]),atoi(argv[1]));
	printf("set debug level = %d\n", send_data);
	close(sockfd);
	exit(0);
	
}