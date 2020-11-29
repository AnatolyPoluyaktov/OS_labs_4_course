#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"
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
    char str[strlen(users)];

    strcpy(str,users);
    free(users);
    str[strlen(str)-1]='\0';
    printf("%s\n",str);

	 int s, len;
    struct sockaddr_un remote;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    while(connect(s, (struct sockaddr *)&remote, len)){
    	sleep(1);
   
}

    printf("Connected.\n"); 
    send(s, str, strlen(str), 0);
    close(s);

	return 0;
}