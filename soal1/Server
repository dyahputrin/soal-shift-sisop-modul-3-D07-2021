// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char hasil[100];
	char cek[10] = {2};
	char *hello = "Hello from server";
	FILE*akun;
	int h=0;
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer);
	
	if(strcmp(buffer,"1")==0){
		valread = read( new_socket , buffer, 1024);
		printf("%s\n",buffer);
		char fn[10]={"akun.txt"},pat[10],temp[200];
		FILE *fp;
		fp=fopen(fn,"r");
		while(!feof(fp))
		{
			fgets(temp,1000,fp);
			if(strstr(temp,buffer)) h=h+1;
		}
		fclose(fp);

		if(h==1) {
			sprintf(buffer,"Login Success");
			send(new_socket , buffer , strlen(buffer) , 0 );	
		}
		else {
			sprintf(buffer,"Login Failed");
			send(new_socket , buffer , strlen(buffer) , 0 );
		}
		return 0;
	}

	if(strcmp(buffer,"2")==0){
		valread = read( new_socket , buffer, 1024);
		printf("%s\n",buffer);
		akun=fopen("akun.txt","a");
		fprintf(akun,"%s\n",buffer);
		fclose(akun);
	}
	else return 0;
	
	return 0;
}

