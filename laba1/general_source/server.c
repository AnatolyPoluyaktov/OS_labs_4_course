#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct person
{
    char name[20];
    int age;
    char dob[10];
};

struct student
{
    struct person info;
    int rollno;
    float marks[10];
}

struct student student_1 = {
                               {"Adam", 25, 1990},
                               101,
                               90
                           };
struct my_msgbuf{
  long mtype;
  struct data {
  char mtext[200][200];
  int size;
} text;
}buf;

 char paths[200][200];
char path[1000];
int main()
{
  FILE * fp;

  //key_t key = ftok("server.c" , 'a');
  //int msqid = msgget(key,0600|IPC_CREAT|IPC_EXCL);
  //struct my_msgbuf buf;

  fp = popen("ls *", "r");
  int count = 0;

  while (fgets(path, 200,fp)!= NULL)
  {
       path[strlen(path)-1] = '\0';
       strcpy(paths[count], path);
       ++count;

  }

  pclose(fp);
  printf("%s\n",student_1.info.name);
   // for(int i = 0 ; i < count; i++){
   //   printf("%s\n", buf.text.mtext[i]);
   // }

  return 0;
}
