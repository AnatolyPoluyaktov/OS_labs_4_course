#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#define SOCK_PATH "echo_socket"
int count_users(char* users)
{	int count = 0;
	for(int i = 0; i < strlen(users); ++i)
	{
		if (users[i] == '\n')
			++count;
	}
		
	++count;
	return count;
	
}
int main()
{    
	 int s, s2,len;
	 socklen_t t = 0;
    struct sockaddr_un local, remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 1) == -1) {
        perror("listen");
        exit(1);
    }

    t = sizeof(remote);
    if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
    	perror("accept");
        exit(1);
     }
     memset(str, 0, 100);
      if (recv(s2, str, sizeof(str),  0) < 0)
    {
        printf("recv failed\n");
        exit(1);
    }
    printf("logged users:\n");
    printf("%s\n", str);
    
    int num = count_users(str);
    printf("number of logged users: %d\n", num);
 

    close(s2);
    unlink(SOCK_PATH);
    
	return 0;
}