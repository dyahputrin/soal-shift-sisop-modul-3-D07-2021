#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <dirent.h> 


#define DATA_BUFFER 300
#define SIZE 300
#define CURR_DIR "/home/gian/"

int curr_fd = -1;
char auth_user[2][DATA_BUFFER]; // [0] => id, [1] => pass
const int SIZE_BUFFER = sizeof(char) * DATA_BUFFER;

// Socket setup
int create_tcp_server_socket();

void *routes(void *argv);
int getinput(int fd, char *sendmessage, char *accmessage);
int login(int fd);
void regis(int fd);
void add(int fd);
int writefile(int fd,char *dirname,char *targetFileName);
bool alreadyDownloaded(FILE *fp, char *filename);
char *getFileName(char *filePath);
int sendfile(int fd, char *filename);
void download (char *filename, int fd);
void see (int fd);
void find(int fd);



int main()
{
    socklen_t addrlen;
    struct sockaddr_in new_addr;
    pthread_t tid;
    char buf[DATA_BUFFER];
    int server_fd = create_tcp_server_socket();
    int new_fd;

    while (1) {
        new_fd = accept(server_fd, (struct sockaddr *)&new_addr, &addrlen);
        if (new_fd >= 0) {
            printf("Accepted a new connection with fd: %d\n", new_fd);
            pthread_create(&tid, NULL, &routes, (void *) &new_fd);
        } else {
            fprintf(stderr, "Accept failed [%s]\n", strerror(errno));
        }
    }
    return 0;
}


void *routes(void *argv){

    int fd = *(int *) argv;
    char cmd[DATA_BUFFER];
    chdir(CURR_DIR);

    while (recv(fd, cmd, DATA_BUFFER, MSG_PEEK | MSG_DONTWAIT) != 0) {
        // public route
        if (fd != curr_fd) {
            if (getinput(fd, "\nSelect command:\n1. Login\n2. Register\n", cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);

            if (strcmp(cmd, "1") == 0) {
                if(login(fd) == 1) curr_fd = fd;
            } 
            else if (strcmp(cmd, "2") == 0) {
                regis(fd);
            } 
            else {
                send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
            }
        } else { 
            char menu[DATA_BUFFER] = "\nChoose\n1. Add\n2.Download\n3.Delet\n4.See\n5.Find\n";
		
            if (getinput(fd, menu, cmd) == 0) break;
            	write(fd, "\n", SIZE_BUFFER);

            if (strcmp(cmd, "1") == 0) {
                add(fd);
            }
		if(strcmp(cmd, "4") == 0 ){
			see(fd);
		}
		if(strcmp(cmd, "5") == 0 ){
			find(fd);
		}
            else {
                char *tmp = strtok(cmd, " ");
                char *tmp2 = strtok(NULL, " ");
                if (!tmp2) {
                    send(fd, "Error: Second argument not specified\n", SIZE_BUFFER, 0);
                }else if(strcmp(cmd, "2") == 0) download(tmp2, fd);
			
                else {
                    send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
                }
            }
        }
        sleep(0.001);
    }
    if (fd == curr_fd) {
        curr_fd = -1;
    }
    printf("Close connection with fd: %d\n", fd);
    close(fd);

}

void see(int fd){
	char publisher[DATA_BUFFER], year[DATA_BUFFER], client_path[DATA_BUFFER],temp[SIZE];
	int c,i=0;
	FILE *file;
	printf("see\n");
	file = fopen("files.tsv", "r");
	if (file) {
	    while ((c = fscanf(file,"%s %s %s\n",publisher, year, client_path)) != EOF){
		printf("data : \n%s %s %s %d\n",publisher,year,client_path,i++);
		sprintf(temp,"Publisher : %s\nTahun Publikasi : %s\nFilepath : %s\n\n\n",publisher,year,client_path);
		send(fd, temp, SIZE_BUFFER, 0);
		}
	fclose(file);
	    
	}
	
}

void find(int fd){
	char namefile[SIZE], buffer[SIZE], new[SIZE];

	getinput(fd, "Filename : \n",buffer);

	DIR *d;
	struct dirent *dir;

	d = opendir("./FILES");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if(strstr(dir->d_name , buffer)) {
				printf("%s\n", dir->d_name);
				sprintf(new,"%s\n",dir->d_name);
				send(fd, new, SIZE, 0);
			}
		}
	closedir(d);
	}
}


void download (char *filename, int fd){
	FILE *fp = fopen("files.tsv", "a+");
	if (alreadyDownloaded(fp, filename)) {
		sendfile(fd, filename);
	} else {
		send(fd, "Error: File hasn't been downloaded\n", SIZE_BUFFER, 0);
	}
    fclose(fp);
}

int sendfile(int fd, char *filename)
{
    char buf[DATA_BUFFER] = {0};
    int ret_val;
    printf("Sending [%s] file to client!\n", filename);
    strcpy(buf, filename);
    sprintf(filename, "FILES/%s", buf);
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        printf("File not found\n");
        send(fd, "File not found\n", SIZE_BUFFER, 0);
        return -1;
    }
    send(fd, "Start receiving file\n", SIZE_BUFFER, 0);
    send(fd, buf, SIZE_BUFFER, 0);

    // Transfer size
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    sprintf(buf, "%d", size);
    send(fd, buf, SIZE_BUFFER, 0);

    while ((ret_val = fread(buf, 1, DATA_BUFFER, fp)) > 0) {
        send(fd, buf, ret_val, 0);
    }
    recv(fd, buf, DATA_BUFFER, 0);
    printf("Send file finished\n");
    fclose(fp);
    return 0;
}

int getinput(int fd, char *sendmessage, char *accmessage){
	send(fd, sendmessage, DATA_BUFFER, 0);

	int ret_val = recv(fd, accmessage, DATA_BUFFER, 0);
	printf("client {%s}\n",accmessage);
	return ret_val;
}

int login(int fd){
	char id[SIZE], pass[SIZE], tmp[SIZE], tmp1[SIZE];
	int flag = 0, login = 0;

	FILE *akun = fopen("akun.txt", "a+");

	printf("login\n");
	printf("%s", tmp);

	getinput(fd, "Id : ", id);
	getinput(fd, "Pass : ", pass);
	sprintf(tmp ,"%s:%s", id,pass);

	printf("%s", tmp);

	while (fscanf(akun, "%s", tmp1) != EOF) {
		login = strcmp(tmp,tmp1);
		printf("%s = %s %d\n",tmp,tmp1,login);
        	if (login == 0){
			flag = 1;
			printf("client succes login\n");
			send(fd, "Login Succes\n", DATA_BUFFER, 0);
			return 1;
		}
   	}
	if (flag == 0 ) send(fd, "Login Failed\n", DATA_BUFFER, 0);

	fclose(akun);
	return -1;
}

void regis(int fd){
	char id[SIZE], pass[SIZE], tmp[SIZE];

	FILE *akun = fopen("akun.txt", "a+");

	printf("regis\n");

	getinput(fd, "Id : ", id);
	getinput(fd, "Pass : ", pass);
	fprintf(akun ,"%s:%s\n", id,pass);
	printf("%s:%s\n", id,pass);
	
	fclose(akun);
}

char *getFileName(char *filePath)
{
    char *ret = strrchr(filePath, '/');
    if (ret) return ret + 1;
    else return filePath;
}

void add(int fd)
{
    char *dirName = "FILES";
    char publisher[DATA_BUFFER], year[DATA_BUFFER], client_path[DATA_BUFFER];
    sleep(0.001);
    if (getinput(fd, "Publisher: ", publisher) == 0) return;
    if (getinput(fd, "Tahun Publikasi: ", year) == 0) return;
    if (getinput(fd, "Filepath: ", client_path) == 0) return;

    FILE *fp = fopen("files.tsv", "a+");
    char *fileName = getFileName(client_path);

    if (alreadyDownloaded(fp, fileName)) {
        send(fd, "Error: File is already uploaded\n", SIZE_BUFFER, 0);
    } else {
        send(fd, "Start sending file\n", SIZE_BUFFER, 0);
        mkdir(dirName, 0777);
        if (writefile(fd, dirName, fileName) == 0) {
            fprintf(fp, "%s\t%s\t%s\n", client_path, publisher, year);
            printf("Store file finished\n");
        } else {
            printf("Error occured when receiving file\n");
        }
    }
    fclose(fp);
}

bool alreadyDownloaded(FILE *fp, char *filename)
{
    char db[DATA_BUFFER], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = getFileName(strtok(db, "\t"));
        if (strcmp(tmp, filename) == 0) return true;
    }
    return false;
}


int writefile(int fd,char *dirname,char *targetFileName)
{
    int ret_val, size;
    char buf[DATA_BUFFER] = {0};
    char in[1];

    // Make sure that client has the file
    ret_val = recv(fd, buf, DATA_BUFFER, 0);
    if (ret_val == 0 || strcmp(buf, "File found") != 0) {
        if (ret_val == 0) printf("Connection to client lost\n");
        else puts(buf);
        return -1;
    }
    recv(fd, buf, SIZE_BUFFER, 0);
    size = atoi(buf);

    printf("Store [%s] file from client\n", targetFileName);
    sprintf(buf, "%s/%s",dirname,targetFileName);
    FILE *fp = fopen(buf, "w+");

    while (size-- > 0) {
        if ((ret_val = recv(fd, in, 1, 0)) < 0)
            return ret_val;
        fwrite(in, 1, 1, fp);
    }
    ret_val = 0;
    printf("Storing file finished\n");
    fclose(fp);
    return ret_val;
}

/****   SOCKET SETUP    *****/
int create_tcp_server_socket()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    int opt = 1;

    /* Step1: create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Initialize the socket address structure */
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    saddr.sin_addr.s_addr = INADDR_ANY;

    /* Step2: bind the socket to port 7000 on the local host */
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Step3: listen for incoming connections */
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }
    return fd;
}
