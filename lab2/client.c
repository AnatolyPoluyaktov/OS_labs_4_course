#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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
    key_t key = ftok("server.c", 'a');
    int shmid;
    while((shmid =shmget(key, 0,0))== -1){
        sleep(1);
    }

    addr = (char*)shmat(shmid, 0, 0);
      if ( addr == (char*)(-1))
      {
          perror("error shmat");
          exit(1);
      }
     strcpy(addr,users);
     int semid;
     while((semid = semget(key,0,0) )== -1){
         sleep(1);
     }



    if (semop(semid,plus,1) < 0){
        perror("semop");
        exit(1);
    }
    if (semop(semid,minus,1) < 0){
        perror("semop");
        exit(1);
    }
    printf("полученное время в клиенте: ");
    printf("%s\n",addr );
    if (shmdt(addr) < 0) {
        printf("shmdt error");
        exit(1);
    }


     free(users);
    return 0;
}
