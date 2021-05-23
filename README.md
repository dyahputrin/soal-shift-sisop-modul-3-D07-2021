### Laporan Resmi Soal Shift Modul 3 (D07)

----------------------
### Soal 1
### A. Program menu untuk login dan register
program login memiliki fungsi ```int login (int fd)``` fungsi ini digunakan untuk mengambil input dari client dan mencocokan dengan database yang ada pada ```akun.txt``` dengan menggunakan iterasi while hingga ```EOF``` lalu cocokan baris yang ada dengan inputan dari client dengan ```strcmp``` jika dari semua baris yang ada tersebu terdapa line yang cocok maka user berhasil login.
```
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
```

program register memiliki fungsi ```register( int fd) ``` fungsi ini digunakan untuk mengambil input dari client dan hasil inputan tersebut kedalam database ```akun.txt``` yang mana pada database tersebut memiliki format ```id:pass```.
```
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
```
### B. Database dan Folder FILES
untuk pembuatan Databse dan folder ```FILES``` dilakukan pada fungsi ```void regis(int fd).
```
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
```

### C. Add files
pada problem tersebut kita diminta untuk membuat program yang berfungsi untuk menambahkan file yang telah dikirimkan oleh client kepada server untuk dimasukan kedalam folder penyimpanan yang ada yaitu ```FILES```, dengan menginputkan data buku dikirmkan oleh client dan client menunjukan lokasi dari file yang akan dikirim yang akan dikirimkan oleh client

```
void add(int fd)
{
    char *dirName = "FILES";
    char publisher[DATA_BUFFER], year[DATA_BUFFER], client_path[DATA_BUFFER];
    FILE *fp = fopen("files.tsv", "a+");
    char *fileName = getFileName(client_path);
    
    if (getinput(fd, "Publisher: ", publisher) == 0) return;
    if (getinput(fd, "Tahun Publikasi: ", year) == 0) return;
    if (getinput(fd, "Filepath: ", client_path) == 0) return;
    
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
```
ketika penambahan dari file atau dengan dipilihnya menu add maka akan dibuat file baru ```files.tsv``` yang menjadi perangkum buku - buku yang ada pada folder ```FILES```, dengan inputan dari client maka server akan mengecek apakah file yang hendak dikirimkan sudah pernah ditambahkan atau belum jika belum maka proses bisa dilanjutkan dengan fungsi ```int writefile(int fd,char *dirname,char *targetFileName)``` yang mana pada fungsi tersebut file akan di salin untuk dikirimkan kedalam folder ```FILES```.
```
int writefile(int fd,char *dirname,char *targetFileName)
{
    int ret_val, size;
    char buf[DATA_BUFFER] = {0};
    char in[1];


    // mengecek apakah file tersebut ada
    ret_val = recv(fd, buf, DATA_BUFFER, 0);
    if (ret_val == 0 || strcmp(buf, "File found") != 0) {
        if (ret_val == 0) printf("Connection to client lost\n");
        else puts(buf);
        return -1;
    }
    recv(fd, buf, SIZE_BUFFER, 0);
    size = atoi(buf);
    // proses pengiriman file
    printf("%s from client\n", targetFileName);
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
```

### D. client mendownload file yang ada pada server
dengan menggunakan fungsi ```void download (char *filename, int fd)``` maka client akan dikirimkan file yang sesuai dengan inputan yang ada pada folder ```FILES```
dan apabila file tidak valid maka akan dikirmika pesan error.
```
void download (char *filename, int fd){
	FILE *fp = fopen("files.tsv", "a+");
	if (alreadyDownloaded(fp, filename)) {
		sendfile(fd, filename);
	} else {
		send(fd, "Error: File hasn't been downloaded\n", SIZE_BUFFER, 0);
	}
    fclose(fp);
}

```
pada code diatas memanggil sebuah fungsi lagi yaitu ```bool alreadyDownloaded(FILE *fp, char *filename)``` pada fungsi tersebut dilakukan pengecekan pada file yang ditunjuk yaitu file ```files.tsv```. dengan menggunakan iterasi while hingga ```EOF``` yang apabila berhasil menemukan file yang dicari maka akan mereturn true, yang nanti akan dilnjutkan pada ```fungsi download``` dan file akan dikirim menggunakan fungsi ```int sendfile(int fd, char *filename)```.
```
bool alreadyDownloaded(FILE *fp, char *filename)
{
    char db[DATA_BUFFER], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = getFileName(strtok(db, "\t"));
        if (strcmp(tmp, filename) == 0) return true;
    }
    return false;
}
```
```
int sendfile(int fd, char *filename)
{
    char buf[DATA_BUFFER] = {0};
    int ret_val;
    printf("Sending %s\n", filename);
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
```
### F. Melihat seluruh file yang ada pada folder ```FILES```
diminta untuk menampilkan seluruh data yang ada pada file tsv, meliputi nama, publisher, tahun, filepath, ekstensifile.
dengan menggunakan fungsi ```void see(int fd)``` dilakukan iterasi while hingga ```EOF``` data - data yang hendak dikirim digabungkan terlebih dahulu menjadi satu gabungan string yang mana string tersebut akan dikirimkan kepada client dan akan ditampilkan oleh client, sehingga client hanya perlu mencetak dari kiriman server.

```
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
```
### G. fungsi untuk mencari file dengan nama tertentu pada folder ```FILES```
mencari file yang ada dengan melakukan iterasi dan membandingkan nama file yang telah diinputkan dengan nama file yang ada menggunakan perintah ```strstr``` yang apabila perintah bernilai benar maka akan mengeluarkan nama yang mirip dari input dan akan mengirimkan nama file tersebut ke client untuk dicetak pada client.
```
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
```

### Client.c 
```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define DATA_BUFFER 300
#define CURR_DIR "/home/gian/client"

const int SIZE_BUFFER = sizeof(char) * DATA_BUFFER;
char inputPath[DATA_BUFFER];
bool _inputPath = false;

int create_tcp_client_socket();
void *handleInput(void *client_fd);
void *handleOutput(void *client_fd);
void getServerInput(int fd, char *input);
void sendFile(int fd);
void writeFile(int fd);

int main()
{
    pthread_t tid[2];
    int client_fd = create_tcp_client_socket();

    pthread_create(&(tid[0]), NULL, &handleOutput, (void *) &client_fd);
    pthread_create(&(tid[1]), NULL, &handleInput, (void *) &client_fd);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    close(client_fd);
    return 0;
}
// fungsi untuk menghandle inputan yang masuk dari server
void *handleInput(void *client_fd)
{
    chdir(CURR_DIR);
    int fd = *(int *) client_fd;
    char message[DATA_BUFFER] = {0};

    while (1) {
        gets(message);
        send(fd, message, SIZE_BUFFER, 0);
        if (_inputPath) {
            strcpy(inputPath, message);
        }
    }
}

// fungsi untuk menghandle output dari client
void *handleOutput(void *client_fd) 
{
    chdir(CURR_DIR);
    int fd = *(int *) client_fd;
    char message[DATA_BUFFER] = {0};

    while (1) {
        memset(message, 0, SIZE_BUFFER);
        getServerInput(fd, message);
        printf("%s", message);
        fflush(stdout);
    }
}

//fungsi untuk menfirim file ke server
void sendFile(int fd)
{
    printf("send %s\n", inputPath);
    int ret_val;
    FILE *fp = fopen(inputPath, "r");
    char buf[DATA_BUFFER] = {0};

    if (fp) {
        send(fd, "File found", SIZE_BUFFER, 0);

        fseek(fp, 0L, SEEK_END);
        int size = ftell(fp);
        rewind(fp);
        sprintf(buf, "%d", size);
        send(fd, buf, SIZE_BUFFER, 0);

        while ((ret_val = fread(buf, 1, 1, fp)) > 0) {
            send(fd, buf, 1, 0);
        } 
        printf("Sending file finished!\n");
        fclose(fp);
    } else {
        printf("File not found\n");
        send(fd, "File not found", SIZE_BUFFER, 0);
    }
}

// fungsi untuk menyalin file 
void writeFile(int fd)
{
    char buf[DATA_BUFFER] = {0};
    int ret_val = recv(fd, buf, DATA_BUFFER, 0);
    FILE *fp = fopen(buf, "w+");

    recv(fd, buf, DATA_BUFFER, 0);
    int size = atoi(buf);
    
    while (size > 0) {
        ret_val = recv(fd, buf, DATA_BUFFER, 0);
        fwrite(buf, 1, ret_val, fp);
        memset(buf, 0, SIZE_BUFFER);
        size -= ret_val;
    }
    puts("Send file finished");
    send(fd, "Send file finished", SIZE_BUFFER, 0);
    fclose(fp);
}

//fungsi untuk mendapatkan inputan dari server
void getServerInput(int fd, char *input)
{
    if (recv(fd, input, DATA_BUFFER, 0) == 0) {
        printf("Server shutdown\n");
        exit(EXIT_SUCCESS);
    }
}

int create_tcp_client_socket()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    int opt = 1;
    struct hostent *local_host; /* need netdb.h for this */

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

    /* Let us initialize the server address structure */
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    local_host = gethostbyname("127.0.0.1");
    saddr.sin_addr = *((struct in_addr *)local_host->h_addr);

    /* Step2: connect to the TCP server socket */
    ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}
```

### Server.c
```
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

// menjadi route utama pengganti
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

// fungsi untuk menampilkan seluruh isi files.tsv
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

//fungsi untuk mencari nama file yang mirip dengan inputan
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

// fungsi untuk mendownload file
void download (char *filename, int fd){
	FILE *fp = fopen("files.tsv", "a+");
	if (alreadyDownloaded(fp, filename)) {
		sendfile(fd, filename);
	} else {
		send(fd, "Error: File hasn't been downloaded\n", SIZE_BUFFER, 0);
	}
    fclose(fp);
}

//fungsi untuk mengirimkan file
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

//fungsi untuk mendapatkan input serta mengirimkan pesan sebelum input
int getinput(int fd, char *sendmessage, char *accmessage){
	send(fd, sendmessage, DATA_BUFFER, 0);

	int ret_val = recv(fd, accmessage, DATA_BUFFER, 0);
	printf("client {%s}\n",accmessage);
	return ret_val;
}

//fungsi login user
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

//fungsi register akun
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

//fungsi untuk mendapatkan nama file yang sesuai dengan kriteria
char *getFileName(char *filePath)
{
    char *ret = strrchr(filePath, '/');
    if (ret) return ret + 1;
    else return filePath;
}

//fungsi untuk menambahkan file 
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

//fungsi boolean untuk mereturn apakh sudah ada atau belum
bool alreadyDownloaded(FILE *fp, char *filename)
{
    char db[DATA_BUFFER], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = getFileName(strtok(db, "\t"));
        if (strcmp(tmp, filename) == 0) return true;
    }
    return false;
}

// fungsi untuk menyalin file
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
```

### Kendala
penempatan folder dan file yang berbeda beda 

### Otput
https://github.com/uio90/gambar1a

### Soal 2
### A. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya
Untuk membuat program perkalian matrix, yang pertama yaitu saya membuat satu fungsi yang bernama ```void scanmatrix```, fungsi ini digunakan untuk mengambil nila/ input yang ingin dimasukkan ke dalam matrix, cara yang dilakukan yaitu dengan membuat 2 buah perulangan yang mana yang pertama menggunakan batasnya sebagai baris dan yang kedua sebagai kolom. Dan tinggal melakukan ```scanf("%d", &matrix[i][j])```, scanf terhadap matrix dengan variable i(Baris) dan j(Kolom).<br/>
```
void scanmatrix(int matrix[][21], int row, int column) 
{
   // Perulangan untuk Baris
   for (int i = 0; i < row; ++i)
   {
      // Perulangan untuk kolom
      for (int j = 0; j < column; ++j)
      {
         // Scan element atau nilai dalam matrix 2 dimennsi
         scanf("%d", &matrix[i][j]);
      }
   }
}
```
Selanjutnya setelah bisa menginputkan nilai ke dalam matrix, lalu membuat fungsi kedua yaitu bernama ```void perkalianmatrix```, seperti namanya fungsi ini digunakan untuk mengkalikan 2 buah matrix, cara yang digunakan yaitu <br/>
    1. Mensetkan isi awal dari matrix dengan 0. <br/>
    2. Mengkalikan 2 buah matrix yaitu dengan cara membuat 3 buah perulangan yang mana pertama sebagai batas baris matriks pertama, <br/>
    3. Perulangan selanjutnya menggunakan batas kolom matriks kedua, dan <br/>
    4. Satu buah perulangan lagi dengan batas kolom matriks pertama yang digunakan sebagai bantuan untuk mengkalikan matriks a dengan matriks b
.
```
void perkalianmatrix(int a[][21], int b[][21], int ans[][21], int r1, int c1, int r2, int c2) 
{
   // Perulangan baris
   for (int i = 0; i < r1; ++i) 
   {
      // Perulangan Kolom
      for (int j = 0; j < c2; ++j) 
      {
         ans[i][j] = 0;
      }
   }
   for (int i = 0; i < r1; ++i) 
   {
      for (int j = 0; j < c2; ++j) 
      {
         for (int k = 0; k < c1; ++k) 
         {
            ans[i][j] += a[i][k] * b[k][j];
         }
      }
   }
}
```
Dan setelah selesai membuat perulangannnya tinggal mengkalikan matriks a dan b dan disimpan ke dalam variable/matrix ans.<br/>
Selanjutnya pada fungsi main, yaitu agar output pada soal2a bisa terbaca di soal2b, maka dilakukan shared memory
```
key_t key = 1234;
int *value;
// Disimpan ke variable value
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
value = shmat(shmid, NULL, 0);
```
Selanjutnya masuk ke dalam proses mengkalikan 2 buah matriks, prosesnya sebagai berikut:  <br/>
    1. Mensetkan bahwa baris=4 dan kolom=3 pada matriks pertama dan baris=3 dan kolom=6 pada matriks kedua  <br/>
    2. Menginputkan nilai Matriks pertama dengan memanggil fungsi ```scanmatrix(a, r1, c1);```  <br/>
    3. Menginputkan nilai Matriks kedua dengan memanggil fungsi ```scanmatrix(b, r2, c2);```  <br/>
    4. Mengkalikan kedua matriks, a dan b dengan memanggil fungsi ```perkalianmatrix(a, b, ans, r1, c1, r2, c2);```  <br/>
    5. Selanjutnya untuk menampilkan hasil matriks yang sudah dikalikan maka tinggal membuat 2 buah perulangan dengan batas pertama baris pada matriks 1 dan kolom pada matriks 2  <br/>
    6. Dan tinggal ```printf("%d  ", ans[i][j]);```  <br/>
    7. Selanjutnya untuk mengirimkan outputnya menggunakan shared memory yaitu dengan memasukkan outputnya ke dalam variable ```value```
.<br/>
Berikut SC pada fungsi main: <br/>
```
int main() 
{
   // Shared Memory
   key_t key = 1234;
   int *value;
   // Disimpan ke variable value
   int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
   value = shmat(shmid, NULL, 0);
   
   int a[21][21], b[21][21], ans[21][21], r1, c1, r2, c2;
   // Mengisi ukuran dari matrixnya
   r1 = 4;
   c1 = 3;
   r2 = 3;
   c2 = 6;
   // Scan isian/element matrix
   printf ("Masukkan Matriks Pertama : \n");
   scanmatrix(a, r1, c1);
   printf ("Masukkan Matriks Kedua : \n");
   scanmatrix(b, r2, c2);
   // Kalikam kedua matrix dengan memanggil fungsi yg sudah kita buat
   perkalianmatrix(a, b, ans, r1, c1, r2, c2);
   printf ("\n");

   // Hasil
   for (int i = 0; i < r1; ++i) 
   {
      for (int j = 0; j < c2; ++j) 
      {
         printf("%d  ", ans[i][j]);
         if (j == c2 - 1)
            printf("\n");
      }
   }
   // Kirim menggunakan Shared Memory
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) 
      {
         value[i*6+j] = ans[i][j];
         //printf ("Terkirim : %d\n", *value);
      }
   }
   sleep(5);
   
   shmdt(value);
   shmctl(shmid, IPC_RMID, NULL);

   return 0;
}
```
### B. Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c)
Yang pertama yaitu membuat struct yang menyimpan variable matriks_pertama dan matriks_kedua<br/>
```
typedef struct matrix {
    int matriks_pertama;
    int matriks_kedua;
}matrix;
```
Selanjutnya membuat fungsi bernama ketentuan, yang mana fungsi ini mengatur faktorial dengan ketentuan seperti di dalam soal, yaitu ketika a>=b maka a!/(a-b)!, jika b>a maka a! dan jika 0 keluarkan 0
```
long long ketentuan(long long b, int testcase)
{
	long long ans=1;
	for (long long a=0;a<testcase;a++)
	{
        // ketika b > a
		if(b-a <= 0)
			ans *= 1;
        // ketika a > b
		else
			ans *= (b-a);
	}
    // Retutn nilai ans
	return ans;
}
```
Selanjutnya membuat fungsi faktorial yang mana berupa pointer, yang mana fungsi ini memiliki bentuk ketika terdapat angka 0 pada matriks pertama maupun matriks kedua, maka akan menampilkan output 2. begitu juga jika tidak,maka akan dipanggil fungsi ketentuan diatas
```
void *faktorial (void *input)
{
	matrix *temp = (matrix *)input;
	if((*temp).matriks_kedua == 0 || (*temp).matriks_pertama == 0)
    {
        printf("0\t");  
    }	
	else
    {
        printf("%lld\t", ketentuan((*temp).matriks_pertama, (*temp).matriks_kedua));
    }	
}
```
Selanjutnya masuk ke dalam proses mengkalikan 2 buah matriksnya, yaitu pada fungsi main:  <br/>
    1. Mendeklarasikan variable untuk menerima hasil output dari soal 2a(Shared Memory) <br/>
    2. Menampilkan output dari soal2a yang mana juga merupakan matriks pertama, yaitu dengan membuat 2 buah perulangan dan menampilkannya menggunakan ```printf("%d ",value[i*6+j]);``` <br/>
    3. Memasukkan input ke matriks kedua yaitu dengan cara membuat 1 perulangan dengan batas 24(banyak data) dab scan menggunakan ```scanf("%d", &matriks_kedua[i]);``` <br/>
    4. Setelah nilai dari matriks pertama dan matriks kedua terisi, maka selanjutnya tinggal mengkaliklan matriksnya dengan menggunakan thread <br/>
    5. Mengkalikan menggunakan thread, yang pertama yaitu membuat perulangan dengan batas 24(sebanyak 24kali). <br/>
    6. Selanjutnya untuk membuat bentuknya seperti matrik yaitu setiap 6 angka, maka akan pindah ke line baru ```if(j%6 == 0)``` <br/>
    7. Setelah itu membuat ```temp1``` yang mana dari struct matrix. <br/>
    8. Menyimpan nilai dari matriks pertama dan kedua kedalam struct matriks temp1 yang sudah dibuat tadi <br/>
    9. Menyimpan nilai matriks_pertama dengan nilai dari value(output dari 2a) ```(*temp1).matriks_pertama= value[j];``` <br/>
    10. Menyimpan nilai matriks_kedua dengan nilai dari matriks_kedua(yang sudah diinputkan tadi) ```(*temp1).matriks_kedua = matriks_kedua[j];``` <br/>
    11. Setelah itu tinggal create dan join threadnya menggunakan ```pthread_create(&thread, NULL, faktorial, (void *)temp1); ``` dan ```pthread_join(thread,NULL); ```
.<br/>
Berikut SC pada fungsi main: <br/>
```
void main()
{
    // Shared Memory
	pthread_t thread;
	key_t key = 1234;

    //int *temp_matriks_pertama;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);

    value = shmat(shmid, NULL, 0);

    sleep(5);

    // Menampilkan Matriks Pertama
    printf ("Matriks Pertama: \n");
    for(int i=0;i<4;i++)
    {
    	for(int j=0;j<6;j++)
        {
            printf("%d ",value[i*6+j]);
    	}
        printf ("\n");
    }
    printf ("\n");	
	
    // Memasukkan input ke matriks kedua
	printf("Masukkan Matriks Kedua: \n");
    for(int i=0;i<24;i++)
    {
        //printf("Masukkan data input[%d]:", i+1);
        scanf("%d", &matriks_kedua[i]);
    }
    printf ("\n");	
	
    // Mengkalikan matriks menggunakan thread
	printf("Matriks yang dihasilkan: \n");
    for(int j=0;j<24;j++)
	{
        if(j%6 == 0)
            printf("\n");
            
        matrix *temp1 = (matrix  *)malloc(sizeof(*temp1)); 

        (*temp1).matriks_pertama= value[j];
        (*temp1).matriks_kedua = matriks_kedua[j];
        
        pthread_create(&thread, NULL, faktorial, (void *)temp1); 
        pthread_join(thread,NULL); 
    }
    printf("\n");

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
```
### C. Membuat program untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5”
Terdapat 3 Fungsi yaitu f1, f2, f3 <br/>
Fungsi f1: <br/>
```
void f1() 
{
  dup2(pipe1[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  execlp("ps", "ps", "aux", NULL);
  perror("bad exec ps");
  _exit(1);
}
```
pada fungsi f1, yaitu menampilkan command ps aux, disini menampilkannya menggunakan execlp ```execlp("ps", "ps", "aux", NULL);``` <br/> disini hanya menggunakan 1 pipe yang bernama ```pipe1``` </br>
Fungsi f2: <br/>
```
void f2() 
{
  dup2(pipe1[0], 0);
  dup2(pipe2[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("sort", "sort", "-nrk", "3,3", NULL);
  perror("bad exec grep root");
  printf("\n");
  printf("\n");
  _exit(1);
}
```
pada fungsi f2, yaitu menampilkan command sort -nrk 3,3, disini menampilkannya menggunakan execlp ```execlp("sort", "sort", "-nrk", "3,3", NULL);``` <br/> disini menggunakan 2 pipe yang bernama ```pipe1``` dan ```pipe2```. </br>
Fungsi f3: <br/>
```
void f3() 
{
  dup2(pipe2[0], 0);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("head", "head", "-5", NULL);
  perror("bad exec grep sbin");
  printf("\n");
  printf("\n");
  _exit(1);
}
```
pada fungsi f3, yaitu menampilkan command head -5, disini menampilkannya menggunakan execlp ```execlp("head", "head", "-5", NULL);``` <br/> disini hanya menggunakan 1 pipe bernama ```pipe2```. </br>

Selanjutnya pada fungsi main, yaitu dengan memanggil pipe dan fork <br/>
```
if (pipe(pipe1)==-1) 
{
  perror("bad pipe1");
  exit(1);
}
if ((pid=fork())==-1) 
{
  perror("bad fork1");
  exit(1);
} 
```
Setelah itu tinggal memanggil fungsi f1 nya, ketika ```pid==0``` <br/>
```
else if (pid == 0) 
{
  f1();
  printf("\n");
  printf("\n");
}
```
Setelah itu memanggil juga fungsi f2 nya, ketika ```pid==0``` <br/>
```
else if (pid == 0) 
{
  f2();
  printf("\n");
  printf("\n");
}
```
Setelah itu memanggil juga fungsi f3 nya, ketika ```pid==0``` <br/>
```
else if (pid == 0) 
{
  f3();
  printf("\n");
  printf("\n");
}
```
Namun letak perbedaaan memanggil nya menggunakan IPC PIPES, yaitu pada fungsi 1, itu memanggil pipe1 dan fork, namun untuk fungsi 2, yaitu memanggil pipe2 dan fork. dan untuk fungsi 3 tidak perlu memannggil pipe1/pipe2 hanya menggunakan fork. <br/>

### Kendala
kendala yang saya kerjakan pada soal ini, terletak pada soal2b yaitu kendala dalam membandingkan 2 buah matriks dan sesuai dengan ketentuan faktorialnya. <br/>

### Output
Output Soal 2a <br/>
![alt text](https://github.com/migellamp/ss_soal1/blob/main/2a.png) <br />
Output Soal 2b <br/>
![alt text](https://github.com/migellamp/ss_soal1/blob/main/2b.png) <br />
Output Soal 2c <br/>
![alt text](https://github.com/migellamp/ss_soal1/blob/main/2c.png) <br />

----------------------

### Soal 3
Pada soal 3, kita diminta untuk membuat sebuah program C yang dapat mengkategorikan file-file sesuai ekstensinya, dan memindahkan file-file tersebut ke dalam suatu folder yang hasilnya terdapat di working directory ketika program dijalankan.

## Soal 3a
Program menerima opsi “-f” dan menambahkan argument file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. Apabila file berhasil dikategorikan, outputnya “Berhasil Dikategorikan”. Apabila gagal dikategorikan, outputnya “Sad, gagal :(”.
```
if(strcmp(argv[1], "-f") == 0) {

    if(argc <= 2) exit(1);

    pthread_t ptid[argc-2];
    for(int i=2; i<argc; i++) pthread_create(&ptid[i-2], NULL, &methodF, (void *)argv[i]);

    for(int i=2; i<argc; i++) pthread_join(ptid[i-2], NULL);
    exit(0);
  }
  
```
Kode program diatas memperlihatkan untuk setiap proses kita menggunakan ```pthread_create``` untuk memulai thread baru pada proses dan ```pthread_join``` untuk menunggu thread tertentu terminate.
```
int fileNum = 1;
void* methodF(void* arg) {
  char buffExt[100], buffNamaFile[1337], buffSource[1337], buffDest[5000], cwd[1337];

  getcwd(cwd, sizeof(cwd));
  strcpy(buffSource, (char*)arg);

  if(access(buffSource, F_OK) == -1) {
    printf("File %d : Sad, gagal :(\n", fileNum);
    fileNum++;
    pthread_exit(0);
  }

  else if(access(buffSource, F_OK) != -1) {
    printf("File %d : Berhasil Dikategorikan!\n", fileNum);
    fileNum++;
  }

  DIR* dir = opendir(buffSource);
  if(dir) {
    pthread_exit(0);
  }
  closedir(dir);

  fetchNamaFile(buffSource, buffNamaFile);

  strcpy(buffSource, (char*) arg);

  ext(buffSource, buffExt);

  for(int i=0; i<sizeof(buffExt); i++) {
    if(buffExt == "Unknown") {
      buffExt[0] = toupper(buffExt[0]);
    }

    else if(buffExt == "Hidden") {
      buffExt[0] = toupper(buffExt[0]);
    }

    else buffExt[i] = tolower(buffExt[i]);
  }

  strcpy(buffSource, (char*)arg);

  check(buffExt);

  sprintf(buffDest, "%s/%s/%s", cwd, buffExt, buffNamaFile);

  rename(buffSource, buffDest);

  pthread_exit(0);
}

```
Potongan kode program diatas ini merupakan function untuk perintah "-f". Pertama kita perlu mengambil current working directory menggunakan ```getcwd()```. Setelah itu kita cek akses pada file tersebut menggunakan ```access()```. Apabila file dapat diakses maka file dapat dikategorikan, sebaliknya file gagal dikategorikan. Dialnjutkan, ke fungsi fetchNamaFile untuk mengambil nama dari suatu file tanpa path-nya. Setelah nama file ditemukan, kita perlu mengambil nama ekstensi dari file tersebut menggunakan fungsi ext. Setelah itu, masuk pada fungsi check untuk mengecek permission menggunakan chmod 0775. Langkah terakhir, file kita rename kembali seperti nama semula dan terminate thread.

## Soal 3b
Program menerima opsi “-d” dan menambhkan 1 argument directory saja. Perintah ini akan mengkategorikan file-file yang berada di directory tersebut. Jika berhasil dikategorikan, outputnya “Direktori Berhasil Diaktegorikan”. Apabila gagal dikategorikan, outputnya “Yah, gagal disimpan☹”.
```
 if(strcmp(argv[1],"-d") == 0) {
    if(argc != 3) exit(1);

    DIR* dir = opendir(argv[2]);
    if(dir) {
    	folder = argv[2];
    	printf("Direktori sukses disimpan!\n"); 
    }
  	
    else if(ENOENT == errno) {
    	printf("Yah, gagal disimpan :(\n");
      	exit(1);
    }
    closedir(dir);
  }
```
Potongan kode program diatas digunakan untuk mengecek argument yang diinputkan pengguna adalah "-d". Setelah itu, apabila directory yang diinputkan pengguna ada maka directory itu akan sukses disimpan, sebaliknya directory itu gagal disimpan. Dilanjutkan, pada program dibawah ini.
```
  DIR* dir = opendir(folder);
  struct dirent* entry;
  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) filesCounter++;
  }
  closedir(dir);

  pthread_t ptid[filesCounter];
  char buff[filesCounter][1337];

  int x = 0; //buat iteration

  dir = opendir(folder);
  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) {
      sprintf(buff[x], "%s/%s", folder, entry->d_name);
      x++;
    }
  }
  closedir(dir);

  for(int i=0; i<filesCounter; i++) {

    char* test = (char*)buff[i];

    if(strcmp(argv[1], "-d") == 0) pthread_create(&ptid[i], NULL, &methodD, (void *)test);

    else if(strcmp(argv[1], "*") == 0) pthread_create(&ptid[i], NULL, &methodS, (void *)test);

  }

  for(int i=0; i<filesCounter; i++) pthread_join(ptid[i], NULL);
```
Potongan kode program diatas digunakan untuk menelusuri file yang ada di suatu directory. Terdapat variable filesCounter untuk menentulan jumlah thread yang diperlukan. Untuk argumen "-d" akan dilanjutkan ke fungsi methodD.
```
void* methodD(void* arg) {

  char buffExt[100], buffNamaFile[1337], buffSource[1337], buffDest[5000], cwd[1337];

  getcwd(cwd, sizeof(cwd));
  strcpy(buffSource, (char*)arg);

  DIR* dir = opendir(buffSource);
  if(dir) {
    //printf("file %s berupa folder\n", buffSource);
    pthread_exit(0);
  }
  closedir(dir);

  fetchNamaFile(buffSource, buffNamaFile);
  strcpy(buffSource, (char*)arg);

  ext(buffSource, buffExt);

  for(int i=0; i<sizeof(buffExt); i++) {

    if(buffExt == "Unknown") {
      buffExt[0] = toupper(buffExt[0]);
    }

    else if(buffExt == "Hidden") {
      buffExt[0] = toupper(buffExt[0]);
    }

    else buffExt[i] = tolower(buffExt[i]);
  }

  strcpy(buffSource, (char*)arg);

  check(buffExt);

  sprintf(buffDest, "%s/%s/%s", cwd, buffExt, buffNamaFile);

  rename(buffSource, buffDest);

  pthread_exit(0);
}
```
Cara kerja dari fungsi methodD hampir sama dengan methodS, dimana fungsi ini juga untuk mengkategorikan files-files pada suatu direktori yang diinputkan pengguna sesuai dengan ekstensinya.

## Soal 3c
Program menerima opsi asterisk. Perintah ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.
```

  if(strcmp(argv[1],"*") == 0) {

    if(argc != 2) exit(1);

    char buff[1337];

    getcwd(buff, sizeof(buff));
    folder = buff;
  }
```
Potongan kode program diatas digunakan untuk mengecek argument yang diinputkan pengguna adalah asterisk. Setelah itu, dilanjutkan pada potongan kode program dibawah ini.
```
  DIR* dir = opendir(folder);
  struct dirent* entry;
  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) filesCounter++;
  }
  closedir(dir);

  pthread_t ptid[filesCounter];
  char buff[filesCounter][1337];

  int x = 0; //buat iteration

  dir = opendir(folder);
  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) {
      sprintf(buff[x], "%s/%s", folder, entry->d_name);
      x++;
    }
  }
  closedir(dir);

  for(int i=0; i<filesCounter; i++) {

    char* test = (char*)buff[i];

    if(strcmp(argv[1], "-d") == 0) pthread_create(&ptid[i], NULL, &methodD, (void *)test);

    else if(strcmp(argv[1], "*") == 0) pthread_create(&ptid[i], NULL, &methodS, (void *)test);

  }

  for(int i=0; i<filesCounter; i++) pthread_join(ptid[i], NULL);
```
Potongan kode program diatas digunakan untuk menelusuri file yang ada di suatu directory. Terdapat variable filesCounter untuk menentulan jumlah thread yang diperlukan. Untuk argumen asterisk akan dilanjutkan ke fungsi methodS.
```
void* methodS(void* arg) {
  char buffExt[100], buffNamaFile[1337], buffSource[1337], buffDest[5000], cwd[1337];

  getcwd(cwd, sizeof(cwd));
  strcpy(buffSource, (char*)arg);

  DIR* dir = opendir(buffSource);
  if (dir) {
    printf("file %s berupa folder\n", buffSource);
    pthread_exit(0);
  }
  closedir(dir);

  fetchNamaFile(buffSource, buffNamaFile);

  strcpy(buffSource, (char*)arg);

  ext(buffSource, buffExt);

  for (int i = 0; i < sizeof(buffExt); i++) {
    if(buffExt == "Unknown") {
      buffExt[0] = toupper(buffExt[0]);
    }
    else if(buffExt == "Hidden") {
      buffExt[0] = toupper(buffExt[0]);
    }
    else buffExt[i] = tolower(buffExt[i]);
  }

  strcpy(buffSource, (char*)arg);

  check(buffExt);

  sprintf(buffDest, "%s/%s/%s", cwd, buffExt, buffNamaFile);

  rename(buffSource, buffDest);

  pthread_exit(0);
}
```
Cara kerja dari fungsi methodD hampir sama dengan methodS, dimana fungsi ini juga untuk mengkategorikan files-files pada direktori dimana program C ini kita jalankan.

## Soal 3d
Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

```
char* ext(char *namaFile, char buff[]) {
  char buffNamaFile[1337];

  char *token = strtok(namaFile, "/");

  while (token != NULL) {
    sprintf(buffNamaFile, "%s", token);
    token = strtok(NULL, "/");
  }

  int x = 0; //counter

  if(buffNamaFile[0] == '.') {
    strcpy(buff, "Hidden");
    return buff;
    exit(1);
  }

  token = strtok(buffNamaFile, ".");

  while(token != NULL) {
    x++;
    sprintf(buff, "%s", token);
    token = strtok(NULL, "!");
    continue;
  }

  if(x <= 1) {
    strcpy(buff, "Unknown");
  }

  return buff;
}
```
Untuk menentukan suatu file tidak memilik ekstensi atau termasuk file hidden, kita perlu mengecek ekstensi file nya terlebih dahulu. Dari fungsi ext yang kita buat, file hidden dapat diidentifikasi apabila karakter pertama dari file itu merupakan ".". Setelah, itu untuk file yang tidak memiliki ekstensi, dapat diidentifikasi melalui sebuah counter variabel x. Dimana variabel x ini digunakan untuk menghitung iterasi pada perulangan while. APabila x tersebut nilainya kurang dari sama dengan 1, maka file tersebut tidak memiliki ekstensi dan dikategorikan ke folder "Unknown".

## Soal 3e
Untuk soal 3e penerapannya sudah dijelaskan diatas dimana, setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

## Hasil
![alt_text](https://github.com/dyahputrin/image/blob/main/hasil1.png)
![alt_text](https://github.com/dyahputrin/image/blob/main/hasil2.png)
![alt_text](https://github.com/dyahputrin/image/blob/main/hasil3.png)


## Kendala
Pada saat mengerjakan soal ini, terdapat kesulitan diawal untuk menentukan file hidden dan unknown.






























