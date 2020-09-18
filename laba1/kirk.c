#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
struct my_msgbuf{
  long mtype;
  char mtext[200];
};

int main(){

  key_t key = ftok("kirk.c" , 'a');
  int msqid = msgget(key,0600|IPC_CREAT|IPC_EXCL);
  printf("%d",msqid);
  struct my_msgbuf buf;
  buf.mtype=1;
  while(1){
  printf("input message : \n");
  scanf("%s", buf.mtext);
  printf("%s\n",buf.mtext);
  int a =msgsnd(msqid, &buf, strlen(buf.mtext), 0);
  printf("%d\n", a );
  printf("exit?\n");
  char ans[3];
  scanf("%s", ans);
  if(strcmp(ans,"yes")==0){
    msgctl(msqid, IPC_RMID, 0);
        return 0;

  }
}
  return 0;
}
