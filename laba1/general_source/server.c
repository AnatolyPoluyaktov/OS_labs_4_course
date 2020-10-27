
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 #include <unistd.h>
int main(){
    char buffer[256+1];
    FILE* fp = popen("file * | grep ':.* shell' | cut -f1 -d':' | xargs", "r");
    size_t read_chars;
    size_t result_size = 0;
    char* files = (char*) calloc(256+1, sizeof(char));
    if (files == NULL)
    {
        perror("problems with allocating");
        return 1;
    }

    while( (read_chars = fread(buffer, 1, sizeof(buffer), fp) ) )
    {

        files = realloc(files, (result_size + read_chars)+1);

        memmove(files + result_size, buffer, read_chars);
        result_size += read_chars;
    }

    pclose(fp);
    struct my_msgbuf{
        long mtype;
        char mtext[strlen(files)];
    };

    struct my_msgbuf buf;

    strcpy(buf.mtext, files);

    free(files);

    buf.mtype = 1;

    key_t key = ftok("server.c", 'a');

    if ( key == -1)
    {
        perror("problems with key");
        return 2;
    }

    int msqid = msgget(key,0600|IPC_CREAT|IPC_EXCL);

    if (msqid == -1)
    {
        perror("problems with message queue's id");
        return 3;
    }

    printf(" Sending: %s with size=%zu\n", buf.mtext, strlen(buf.mtext));
    sleep(3);
    int ans = msgsnd(msqid, &buf, strlen(buf.mtext), 0);
    if (ans == -1)
    {
        perror("problems with sending");
        msgctl(msqid, IPC_RMID, NULL);
        return 4;
    }
    printf("Message has been sent\n");


    return 0;
}
