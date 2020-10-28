#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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

                printf("%d\n",now()-timeuser(users+offset, i));
                offset=i+1;
            }
        ++i;
    }
printf("%d\n",now() - timeuser(users+offset, i));
}
int main()
{//   TimeToInt("01:00");
//     gettime("robin");
  getans("batman\nrobin\0");

  char * s;
  int = 1000;
  sprintf(addr())


 //printf("%d\n",gettime("batman") );


    return 0;
}
