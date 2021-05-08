// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	char ans[10];
	char id[100],pass[100],gabungan[200];
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	printf("Hello message sent\n");
	printf("User Authentication\n1.Login\n2.Register\ntype your choice : ");
	scanf("%s",ans);
	send(sock, ans, strlen(ans), 0);

	if(strcmp(ans,"1")==0){
		printf("Enter ID : ");
		scanf("%s",id);
		printf("Enter Password: ");
		scanf("%s",pass);
		sprintf(gabungan, "%s:%s", id,pass);
		send(sock, gabungan, strlen(gabungan), 0 );
		valread = read(sock , buffer, 1024);
		printf("%s\n",buffer);
		}

	if(strcmp(ans,"2")==0){
		printf("Enter ID : ");
		scanf("%s",id);
		printf("Enter Password: ");
		scanf("%s",pass);
		sprintf(gabungan, "%s:%s", id,pass);
		send(sock, gabungan, strlen(gabungan), 0 );
		}
	else
		return 0;
	return 0;
}

