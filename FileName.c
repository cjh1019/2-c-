#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Object {
	char* name;
	int x;
	int y;
	int zIndex;
	char type;
	char* innerText;
};

char* itos(int input) {
	int n = input;
	char* output;
	int length = 0;
	int w = 1;

	for (; n != 0; n /= 10, ++length, w *= 10);
	w /= 10;
	output = (char*)malloc(sizeof(char) * length);
	for (int i = length - 1; i >= 0; i--) {
		if (i == 0) {
			output[length - i - 1] = '0' + (input % (w * 10));
			output[length] = '\0';
			break;
		}
		else {
			output[length - i - 1] = '0' + (input / w);
			input = input % w;
			w /= 10;
		}
	}

	printf("length = %d", length);
	printf("output = %s", output);
	return 0;
}

/*
int save(struct Object object, FILE* file) {
	char essentialData[1] = 0;
	strcat(essentialData, " name=");
	strcat(essentialData, object.name);
	strcat(essentialData, " x=");
	strcat(essentialData, object.x);
	strcat(essentialData, " y=");
	strcat(essentialData, object.y);
	strcat(essentialData, " z=");
	strcat(essentialData, object.zIndex);

	switch (object.type) {
	case 'T':		//Text
		fprintf(file, "<text>");
		break;
	case 'I':		//Image
		fprintf(file, "<img>");
		break;
	default:
		return -1;
		break;
	}
}
*/
int main() {
	FILE* f = fopen("20231117.txt", "w+");
	struct Object obj = { "object 1", 0, 0, 1, 'T' };
	//save(obj, f);
	itos(507);
	fclose(f);

	return 0;
}