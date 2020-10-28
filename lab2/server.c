#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int now(){
    time_t now;
    struct tm *now_tm;
    int hour;
    int min;
    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;
    min  = now_tm->tm_min;
    int ans = hour*60 +min;
    return ans;

}
int charToInt(char c){
   int num = 0;


   num = c - '0';

   return num;
}

int TimeToInt(char* time){
    int hour = charToInt(time[0])*10 + charToInt(time[1]);
    int minute = charToInt(time[3])*10 + charToInt(time[4]);

    int ans = hour*60 + minute;
    return ans;
}
int  gettime(char* user){
    char buffer[256+1];
    char cmd [256];
    strcpy(cmd,"who | grep \"");
    strcat(cmd,user);
    strcat(cmd,"\" | cut -b 34-39");

    FILE* fp = popen(cmd, "r");
    size_t read_chars;
    size_t result_size = 0;
    char* users = (char*) calloc(256+1, sizeof(char));
    if ( users == NULL)
    {
        perror("problems with allocating");
            exit(1);
    }

    while( (read_chars = fread(buffer, 1, sizeof(buffer), fp) ) )
    {

        users = realloc(users, (result_size + read_chars)+1);

        memmove(users + result_size, buffer, read_chars);
        result_size += read_chars;
    }
    char time_str[10];
    strcpy(time_str, users);
    pclose(fp);
    free(users);
    time_str[strlen(time_str)-1]= '\0';
    int time = TimeToInt(time_str);
    return time;
}


int timeuser(char* beg, int len){
    char buffer[100];
    for(int i =0 ; i < len;++i){
         buffer[i]=beg[i];

    }
    buffer[len]='\0';

    printf("%s is working about ",buffer);

    return gettime(buffer);
}
void getans(char* users){
    int i = 0;
    int offset = 0;
    while (users[i]!='\0'){
            if(users[i] == '\n'){

                printf("%dm\n",now()-timeuser(users+offset, i));
                offset=i+1;
            }
        ++i;
    }
printf("%dm\n",now() - timeuser(users+offset, i));
}
//--------------------------
struct sembuf plus[1] = {{0,4,0}};
struct sembuf minus[1] = {{0,-3,0}};
char* addr;
union semun
{
    int val;
    struct semid_ds* sbuf;
    ushort* array;//массив значений семафоров в наборе
}arg;

int main()
{
    key_t key = ftok("server.c", 'a');
    if (key == -1){
        perror("error key");
        exit(1);
    }


    int shmid = shmget(key, 1024, IPC_CREAT | IPC_EXCL| 0666);
    printf("%d\n",shmid );
    if (shmid == -1)
    {
        perror("error shmget");
        exit(1);
    }
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL| 0666);
    if (semid == -1)
    {
        perror("error semget");
        exit(1);
    }
    arg.val = 1;

    if( semctl(semid,0 , SETVAL, arg) == -1)
    {
        perror("error semctl");
        exit(1);
    }
    addr = (char*)shmat(shmid, 0, 0);
      if ( addr == (char*)(-1))
      {
          perror("error shmat");
          exit(1);
      }

    if( semop(semid, minus,1) == -1 )
    {
            perror("error semop");
    }
    addr[strlen(addr)-1]='\0';


    getans(addr);
    struct semid_ds buf;
    arg.sbuf = &buf;
    if (semctl( semid, 0, IPC_STAT, arg) ==-1)
    {
        perror("bad");
        exit(1);
    }
    printf( "время когда последний процесс выполнял операции над семафорами %ld\n", arg.sbuf->sem_otime);

    sprintf(addr,"%ld",arg.sbuf->sem_otime);
    if (semop(semid,plus,1) < 0){
        perror("semop");
        exit(1);
    }
    if (shmdt(addr) < 0) {
        printf("shmdt error");
        exit(1);
    }
    //REMOVE ROP
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        printf("error shmctl");
        exit(1);
    }
    //REMOVE SEMS
    if (semctl(semid, 0, IPC_RMID) < 0) {
        printf("error semctl\n");
        exit(1);
    }



    return 0;
}
