#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
const char sides[] = {'A','B'};
/*
      MPI_Init(&argc, &argv);
      MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      printf("%d",size);
*/

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


    srand( (unsigned)time(NULL) );

    int kids_quant = 3 + rand()%10;
    int pid;
    int i = kids_quant;
    int count = 0;
    
    char side  = ' ';
    while(i && pid != 0){
        pid = fork();
        count++;
        i--;
    }
    
    if(pid == 0){
        char t = setSide(count, side); 
        printf("%c\n%d\n",t,count);




    }


}

