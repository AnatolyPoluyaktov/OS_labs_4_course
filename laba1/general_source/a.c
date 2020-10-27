#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_WORD "end"
#define SIZE_INCREMENT 10

void main() {
    //Массив указателей на слова
    char **words;
    //Строка, которая используется для считывания введённого пользователем слова
    char buffer[128];
    //Счётчик слов
    unsigned wordCounter = 0;
    //Длина введённого слова
    unsigned length;
    //Размер массива слов. Для уменьшения издержек на выделение памяти
    //каждый раз будем увеличивать массив слов не на одно значение, а на
    //SIZE_INCREMENT слов
    unsigned size = SIZE_INCREMENT;
    int i;

    //Выделяем память под массив из size указателей
    words = (char**) malloc(size*sizeof(char*));
    do {
        printf("%d: ", wordCounter);
        scanf("%127s", buffer);
        //Функция strcmp возвращает 0, если две строки равны
        if (strcmp(TERM_WORD, buffer) == 0) {
            break;
        }
        //Определяем длину слова
        length = strlen(buffer);
        //В том случае, если введено слов больше, чем длина массива, то
        //увеличиваем массив слов
        if (wordCounter >= size) {
            size += SIZE_INCREMENT;
            words = (char**) realloc(words, size*sizeof(char*));
        }
        //Выделяем память непосредственно под слово
        //на 1 байт больше, так как необходимо хранить терминальный символ
        words[wordCounter] = (char*) malloc(length + 1);
        //Копируем слово из буффера по адресу, который
        //хранится в массиве указателей на слова
        strcpy(words[wordCounter], buffer);
        wordCounter++;
    } while(1);

    for (i = 0; i < wordCounter; i++) {
        printf("%s\n", words[i]);
    }
    _getch();

    for (i = 0; i < wordCounter; i++) {
        free(words[i]);
    }
    free(words);
}
