// SESUDAH REVISI
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int matriks_kedua[24];
int *value;


// Membuat struct yang mana menyimpan 2 variable, yaitu matriks pertama dan matriks kedua
typedef struct matrix {
    int matriks_pertama;
    int matriks_kedua;
}matrix;

// Fungsi yang digunakan untuk ketentuan dalam faktorial
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

// Fungsi Faktorial
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
