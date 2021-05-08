// MASIH SALAH (DIREVISI DULU NANTI)
// Kendala : membandingkan 2 matrix

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

// Fungsi Faktorial
int faktorial(int num) {
  if(num == 0){
    return 1;
  }
  return num * faktorial(num-1);
}

int main() {
    
    int lebihdari = 1;
    int kurangdari = 1;
    int test=5;
    int test1=10;
    int test2=3;
    int matriks1[100][100], matriks2[100][100], hasil[100][100];
    int i, j, k, m, n, p, q, jumlah = 0;

    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    sleep(5);

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

    p=4;
    q=6;

    printf("Masukkan Matriks Kedua: \n");
    for(i = 0; i < p; i++)
    {
      for(j = 0; j < q; j++)
      {
        scanf("%d", &matriks2[i][j]);
      }
    }
    for(i = 0; i < p; i++)
    {
      for(j = 0; j < q; j++)
      {
          // a lebih kecil dari b
          if(value[i*6+j] >> matriks2[i][j])
          {
                value[i*6+j] = faktorial(value[i*6+j]);
                matriks2[i][j] = 1;
          }
          else if (value[i*6+j] == 0 || matriks2[i][j] == 0)
          {
                value[i*6+j] = 0;
                matriks2[i][j] = 1;
          }
          // a lebih besar dari b
          else if(value[i*6+j] << matriks2[i][j])
          {
                value[i*6+j] = faktorial(value[i*6+j])/faktorial(value[i*6+j]-matriks2[i][j]);
                matriks2[i][j] = 1;
          }

        hasil[i][j] = value[i*6+j] * matriks2[i][j];
      }
    }

    printf("Hasil perkalian matriks: \n");
    for(i = 0; i < p; i++){
      for(j = 0; j < q; j++){
        printf("%d\t", hasil[i][j]);
      }
      printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
