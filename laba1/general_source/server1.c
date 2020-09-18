#include <stdio.h>
int main(){


FILE *fp;
int status;
char path[200];


fp = popen("ls *", "r");
if (fp == NULL)
    /* Handle error */;


while (fgets(path, 200, fp) != NULL)
    printf("%s", path);


status = pclose(fp);
if (status == -1) {
    /* Error reported by pclose() */
    ...
} else {
     printf("%s",path );
  return 0;
}
