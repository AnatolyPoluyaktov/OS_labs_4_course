#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
struct my_msgbuf{
  long mtype;
  char mtext[200];
};
int main(){
  key_t key = ftok("kirk.c",'a');
  int msqid = msgget(key,0);
  printf("%d\n", msqid);
  struct my_msgbuf buf;
  buf.mtype = 1;
  while(1){
    ssize_t bytes = msgrcv(msqid, &buf, sizeof buf.mtext, buf.mtype, 0);
    printf("%ld\n", bytes);
    printf("%s\n",buf.mtext);
    printf("exit?\n");
    char ans[3];
    scanf("%s", ans);
    if(strcmp(ans,"yes")==0)
       break;
  }
  return 0;
}
