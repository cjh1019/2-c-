#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

char* getkey();

int main() {
    char* inputText;

    printf("�ؽ�Ʈ�� �Է��ϼ��� (�����Ϸ��� Ctrl+s�� ��������):\n");
    inputText = getkey();
    printf("�Էµ� �� : %s", inputText);
    FILE* file = fopen("test.txt", "w");
    if (file != NULL) {
        fputs(inputText, file);
        printf("�ؽ�Ʈ�� ����ȭ���� 'test.txt' ���Ϸ� ����Ǿ����ϴ�.\n");
    }
    else {
        printf("���� ���� ����!\n");
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
                printf("�Լ� �� : %s\n", buffer);
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