#include <sys/msg.h>
#include <stdio.h> -
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
int main()
{

    key_t key = ftok("server.c", 'a');

    if ( key == -1)
    {
        perror("problems with key");
        return 2;
    }
    int mq_id;

    while(1) {
        sleep(1);
        mq_id = msgget(key, 0);
        if (mq_id != -1)
            break;

   }

    printf("%d\n", mq_id);

    struct msqid_ds ds;

    int ans = msgctl(mq_id, IPC_STAT, &ds);
    if ( ans == -1)
    {
        perror("problems with msgctl");
        return 4;
    }

    printf("Messages count: %ld\n", ds.msg_qnum);
    printf("Max bytes: %ld \n", ds.msg_qbytes);
    printf("Last send PID: %d\n", ds.msg_lspid);
    printf("All bytes: %ld\n", ds.msg_cbytes);

    struct
    {
        long type;
        char text[ds.msg_qbytes];
    } message;
    while(1){
    int st = msgrcv(mq_id, &message, sizeof(message.text), 0, 0);

    if (st > 0)
    {
        message.text[st-1] = 0;
        printf("Message: %s with len=%zu\n", message.text, strlen(message.text));
        break;
    }
    else if( st == -1)
    {
        perror("error of receiving");
        return  5;
    }
}

    char *command;
    int s = asprintf(&command, "grep -l -w 'trap' %s", message.text);
    if ( s == -1)
    {
        perror("problems with allocating or memory");
        return 6;
    }
    system(command);
    free(command);

    msgctl(mq_id, IPC_RMID, NULL);
    printf("Done.\n");
    return 0;
}
