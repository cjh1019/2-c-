#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

char* getkey();

int main() {
    char* inputText;

    printf("텍스트를 입력하세요 (종료하려면 Ctrl+s를 누르세요):\n");
    inputText = getkey();
    printf("입력된 값 : %s", inputText);
    FILE* file = fopen("test.txt", "w");
    if (file != NULL) {
        fputs(inputText, file);
        printf("텍스트가 바탕화면의 'test.txt' 파일로 저장되었습니다.\n");
    }
    else {
        printf("파일 저장 실패!\n");
    }
    free(inputText);
    return 0;
}



char* getkey() {
    char ret[] = "";
    char *buffer;
    int size = 0;
    while (1) {
        char c = _getch();
        if (c != EOF) {
            if (c == 19) {
                printf("함수 안 : %s\n", buffer);
                return buffer;
            }
            else {
                printf("%c", c);
                ret = (char)malloc(sizeof(char));
                *ret = c;
            }
            size++;
        }
    }
}