### Laporan Resmi Soal Shift Modul 3 (D07)

----------------------

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
### c. Membuat program untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5”
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






























