### Laporan Resmi Soal Shift Modul 3 (D07)

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






























