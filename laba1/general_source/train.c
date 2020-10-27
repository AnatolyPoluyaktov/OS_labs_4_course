#include <stdio.h>
#include <sys/msg.h>

#define PROJECT_ID 1337

int main()
{
    char kek[]="ddsada";
    printf("%d\n",strlen(kek));
    printf("\e[7m\e[38;5;82m[INFO] Server started\n\e[0m");

    printf("\e[38;5;82m[INFO] Touching temp file\n");
    system("touch /tmp/lab4");
    key_t ipckey = ftok("/tmp/lab4", PROJECT_ID);

    int mq_id;
    if ((mq_id = msgget(ipckey, IPC_EXCL | IPC_CREAT | 0666)) == -1)
        if ((mq_id = msgget(ipckey, 0)) == -1)
        {
            printf("\e[38;5;198m[ERROR] Sorry, babe, but this f*cking PC can't create or open message query...=(\n");
            exit(1);
        }

    printf("\e[38;5;82m[INFO] Message identifier is %d\n", mq_id);

    FILE *shell = popen("file * | grep ':.* shell' | cut -f1 -d':' | xargs", "r");
    char buf[256];
    size_t read_chars;
    size_t current_lenght = 256;
    size_t result_lenght = 0;
    char  *shell_files   = calloc(current_lenght,sizeof(char));

    while ((read_chars = fread(buf, 1, sizeof(buf), shell)) != 0) {

        if (result_lenght + read_chars >= result_lenght) {

            current_lenght *= 2;
            shell_files = realloc(shell_files, current_lenght);
        }
        printf("%d\n",sizeof (shell_files));
        memmove(shell_files + result_lenght, buf, read_chars);
        result_lenght += read_chars;
    }

    pclose(shell);
    struct {
        long type;
        char shell_files[strlen(shell_files)];
    } message;

    message.type = 1;
    strcpy(&message.shell_files, shell_files);

    printf("\e[38;5;82m[INFO] Sending: %s with size=%d\n", message.shell_files, strlen(message.shell_files));
    msgsnd(mq_id, &message, strlen(message.shell_files), 0);

    free(shell_files);
    printf("\e[0mDone.\n");
    return 0;
}
