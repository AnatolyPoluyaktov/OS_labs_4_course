#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
struct sembuf plus[1] = {{0,4,0}};
struct sembuf minus[1] = {{0,-3,0}};
char* addr;

int main()
 {
    char buffer[256+1];
    FILE* fp = popen("who -u | cut -d \" \" -f1", "r");
    size_t read_chars;
    size_t result_size = 0;
    char* users = (char*) calloc(256+1, sizeof(char));
    if ( users == NULL)
    {
        perror("problems with allocating");
        return 1;
    }

    while( (read_chars = fread(buffer, 1, sizeof(buffer), fp) ) )
    {

        users = realloc(users, (result_size + read_chars)+1);

        memmove(users + result_size, buffer, read_chars);
        result_size += read_chars;
    }

    pclose(fp);
    printf("%s",users);
    key_t key = ftok("server.c", 'a');
    int shmid = shmget(key, 0,0);
    if (shmid == -1)
    {
        perror("error shmget");
        exit(1);
    }
    addr = (char*)shmat(shmid, 0, 0);
      if ( addr == (char*)(-1))
      {
          perror("error shmat");
          exit(1);
      }
     strcpy(addr,users);
     int semid = semget(key,0,0);

     if (semid == -1)
     {
         perror("error semget");
         exit(1);
     }

    if (semop(semid,plus,1) < 0){
        perror("semop");
        exit(1);
    }
    if (semop(semid,minus,1) < 0){
        perror("semop");
        exit(1);
    }

    printf("kek\n");

     free(users);
    return 0;
}
