#include <stdlib.h>
#include <stdio.h>

int main()
{
    char* arr[3][5]= {"aaaa\0","bbbb\0","cccc\0"};
    char** arr1 =arr;
    printf("%s", *(arr+1));
    //printf("%zu", sizeof(arr[0]));
    return 0;
}
