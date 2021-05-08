#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

// Fungsi untuk menscan atau mengambil nilai dari matrix
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
// Fungsi untuk mengkalikan dua matrix
// a sebagai matrix pertama
// b sebagai matrix kedua
// ans sebagai hasil dari perkalian matrix a dan b
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
// Fungsi Main
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
