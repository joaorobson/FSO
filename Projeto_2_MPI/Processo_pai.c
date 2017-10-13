#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <semaphore.h>
#define KEY_T 42
sem_t direction;
sem_t bridge;
struct sembuf g_lock_sembuf[1];
struct sembuf g_unlock_sembuf[1];

const char sides[] = {'A','B'};
const char states[3][20] = {"Playing", "Reflecting", "Crossing"};
/*
      MPI_Init(&argc, &argv);
      MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      printf("%d",size);
*/
typedef struct Kid{
    char side;
    char state[20];
}kid;

char setSide(int count,char side){
    

    srand( (unsigned)time(NULL) );
    int rand_side = rand()%10;
    if(count <= rand_side && side == ' '){
        side = sides[0];
    }
    else{
        side = sides[1];
    }
    return side;
}

int main(int argc, char **argv ){
    
    int rank,size;
    int semid;
    int nsems = 1;
    g_lock_sembuf[0].sem_num = 0; g_lock_sembuf[0].sem_op   = -1;g_lock_sembuf[0].sem_flg = 0;
    g_unlock_sembuf[0].sem_num = 0; g_unlock_sembuf[0].sem_op = 1; g_unlock_sembuf[0].sem_flg = 0;
    semid = semget(KEY_T, nsems, IPC_CREAT|0666);
    semop (semid, g_unlock_sembuf, 1);

    srand( (unsigned)time(NULL) );

    int kids_quant = 3 + rand()%10;
    int pid = 1;
    int i = kids_quant;
    int count = 0;
    
    kid belgian_kid[kids_quant];

    char side  = ' ';
    while(i && pid != 0){
        pid = fork();
        count++;
        i--;

    }
    if(pid == 0){
        srand( (unsigned)time(NULL) ^ (getpid()<<16) );
        int random_time = rand()%1;
        belgian_kid[count].side = setSide(count, side);
        strcpy(belgian_kid[count].state, states[0]);
        printf("%c --- %s  %d\n",belgian_kid[count].side,belgian_kid[count].state,count);
        strcpy(belgian_kid[count].state, states[1]);

        printf("%c --- %s  %d\n",belgian_kid[count].side,belgian_kid[count].state,count);
         
        sleep(random_time);
        if (strcmp(belgian_kid[count].state,"Reflecting")==0){
            semop(semid, g_lock_sembuf, 1);
            printf("Lado - %c ",belgian_kid[count].side);
            printf("I will cross - %d ... ",count);
            usleep(70);
            if (belgian_kid[count].side == 'A'){
                belgian_kid[count].side = 'B'; 
            }else{
                belgian_kid[count].side = 'A';
            }
            printf("Lado - %c - %d\n",belgian_kid[count].side,count);
            semop(semid, g_unlock_sembuf, 1);
        }
        

    }


}

