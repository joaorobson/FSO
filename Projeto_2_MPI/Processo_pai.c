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
sem_t *s;
struct sembuf g_lock_sembuf[1];
struct sembuf g_unlock_sembuf[1];
char SIDE;
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
    int lock;
}kid;
void CanICross(kid a,int count){

        if (strcmp(a.state,"Reflecting")==0){
            if(SIDE == a.side){
                sem_post(&s[count]);
            }
        }

}
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
    sem_init(&bridge,1,1);
    g_lock_sembuf[0].sem_num = 0; g_lock_sembuf[0].sem_op   = -1;g_lock_sembuf[0].sem_flg = 0;
    g_unlock_sembuf[0].sem_num = 0; g_unlock_sembuf[0].sem_op = 1; g_unlock_sembuf[0].sem_flg = 0;
    semid = semget(KEY_T, nsems, IPC_CREAT|0666);
    semop (semid, g_unlock_sembuf, 1);

    srand( (unsigned)time(NULL) );

    int kids_quant = 3 + rand()%10;
    int pid = 1;
    int i = kids_quant;
    int count = 0;
    g_lock_sembuf[0].sem_num = 0; g_lock_sembuf[0].sem_op   = -1;g_lock_sembuf[0].sem_flg = 0;
    kid belgian_kid[kids_quant];
    //sem_t s[kids_quant];
    s = (sem_t*) malloc(kids_quant*10*sizeof(sem_t));

    char side  = ' ';
    while(i && pid != 0){
        pid = fork();
        count++;
        i--;

    }
    if(pid == 0){
        srand( (unsigned)time(NULL) ^ (getpid()<<16) );
        sem_init(&s[count],1,1);
        int random_time = rand()%1;
        belgian_kid[count].side = setSide(count, side);
        strcpy(belgian_kid[count].state, states[0]);
        belgian_kid[count].lock = 0;
        printf("%c --- %s  %d\n",belgian_kid[count].side,belgian_kid[count].state,count);
        sem_wait(&bridge);
        strcpy(belgian_kid[count].state, states[1]);
        SIDE = belgian_kid[count].side;
        printf("side %c\n", SIDE);
        CanICross(belgian_kid[count], count);
        sem_post(&bridge);
        sem_wait(&s[count]);
        printf("%c --- %s  %d\n",belgian_kid[count].side,belgian_kid[count].state,count);
         
            //semop(semid, g_lock_sembuf, 1);
            printf("Lado - %c ",belgian_kid[count].side);
            printf("I will cross - %d ... ",count);
            if (belgian_kid[count].side == 'A'){
                belgian_kid[count].side = 'B'; 
            }else{
                belgian_kid[count].side = 'A';
            }
            printf("Lado - %c - %d\n",belgian_kid[count].side,count);
            //semop(semid, g_unlock_sembuf, 1);
        }
        

    }



