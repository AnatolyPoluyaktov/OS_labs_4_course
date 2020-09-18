#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
struct my_msgbuf{
  long mtype;
  struct  info_array{
  char mtext[200][200];
  int size;
} text;
};

int main(){
  key_t key = ftok("server.c",'a');
  int msqid = msgget(key,0);

  struct my_msgbuf buf;
  buf.mtype = 1;
  while(1){
    ssize_t bytes = msgrcv(msqid, &buf, 200, buf.mtype, 0);
    printf("%ld\n", bytes);
    printf("%d\n",buf.text.size);
    printf("exit?\n");
    char ans[3];
    scanf("%s", ans);
    if(strcmp(ans,"yes")==0)
       break;
  }
  return 0;
}
