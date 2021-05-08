#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>


void *methodD(void* arg);
void *methodF(void* arg);
void *methodS(void* arg);
char* fetchNamaFile(char* namaFile, char buff[]);
char* ext(char *namaFile, char buff[]);
void check(char buff[]) ;


struct args {
  char* buffer;
};


int main(int argc, char *argv[]) {

  char* folder;
  int filesCounter= 0;

  if(argc == 1) exit(1);

  if(strcmp(argv[1], "-f") != 0 && strcmp(argv[1], "*") != 0 && strcmp(argv[1], "-d")) {
    exit(1);
  }

  if(strcmp(argv[1], "-f") == 0) {

    if(argc <= 2) exit(1);

    pthread_t ptid[argc-2];
    for(int i=2; i<argc; i++) pthread_create(&ptid[i-2], NULL, &methodF, (void *)argv[i]);

    for(int i=2; i<argc; i++) pthread_join(ptid[i-2], NULL);
    exit(0);
  }

  if(strcmp(argv[1],"*") == 0) {

    if(argc != 2) exit(1);

    char buff[1337];

    getcwd(buff, sizeof(buff));
    folder = buff;
  }

  if(strcmp(argv[1],"-d") == 0) {
    if(argc != 3) exit(1);

    DIR* dir = opendir(argv[2]);
    if(dir)folder = argv[2];
  
    else if(ENOENT == errno) {
      printf("Can not find directory \n");
      exit(1);
    }
    closedir(dir);
  }


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

}

char* fetchNamaFile(char* namaFile, char buff[]) {
  char* token = strtok(namaFile, "/");

  while(token != NULL) {
    sprintf(buff, "%s", token);
    token = strtok(NULL, "/");
  }
}

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
    token = strtok(NULL, ".");
  }

  if(x <= 1) {
    strcpy(buff, "Unknown");
  }

  return buff;
}

void check(char buff[]) {

  DIR* dir = opendir(buff);

  if (ENOENT == errno) {
    mkdir(buff, 0775);
    closedir(dir);
  }
}

void *methodD(void* arg) {

  char buffExt[100], buffNamaFile[1337], buffSource[1337], buffDest[5000], cwd[1337];

  getcwd(cwd, sizeof(cwd));
  strcpy(buffSource, (char*)arg);

  if(access(buffSource, F_OK) == -1) {
    printf("Yah, gagal disimpan :(\n");
    pthread_exit(0);
  }

  else if(access(buffSource, F_OK) != -1) {
    printf("Direktori sukses disimpan!\n");
  }

  DIR* dir = opendir(buffSource);
  if(dir) {
    printf("file %s berupa folder\n", buffSource);
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

int fileNum = 1;
void *methodF(void* arg) {
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
    printf("file %s berupa folder\n", buffSource);
    pthread_exit(0);
  }
  closedir(dir);

  fetchNamaFile(buffSource, buffNamaFile);

  strcpy(buffSource, (char *) arg);

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

void *methodS(void* arg) {
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
