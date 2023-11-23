#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "han2unicode.h"
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_ttf.lib")

struct Object {
	int id;
	char* innerText;
	struct Color* textColor;
} *object;


struct Button {
	int id;
	int x;
	int y;
	int w;
	int h;
	int textX;
	int textY;
	int BTK;
	char* BGC;
	char* HOVC;
	char* BRC;
	char* TXC;
	char* TXT;
	char* FONT;
	int FS;
	int unicode;
} *button;

struct Color {
	int R;
	int G;
	int B;
	int A;
};
SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Event event;

void Rect(int, int, int, int, char*, int, char*);
void Button(int, int, int, int, int, int, int, char*, char*, char*, int, char*, char*, char*, int, int);
void setUI(TTF_Font*);
void buttonAction(int);
void refresh();
int Text(const char*, int, int, char*, int, char*, int);
void save();
void load();
int ctoi(char);
int setup(int, int, char[]);
struct Color toRGB(char*); char* itos(int);

const char* KOREAN_TEXT = "안녕하세요, SDL!";
int windowWidth = 960;
int windowHeight = 480;
int buttoncount = 0;
int objectcount = 0;
int last_hovered_button = -1;
int size = 5;
char color[] = "#0011ff00";
char* inputString;
int inputStringLen = 0;
int inputStringIndex = 0;
int selectStart = 0, selectEnd = 0;


FILE* textfile;


int main(int argc, char* argv[]) {
	setup(windowWidth, windowHeight, "C Simple text editor");
	if (TTF_Init() == -1)
	{
		return 0;
	}
	font = TTF_OpenFont("fonts/NanumGothic.ttf", 30);
	if (font == NULL) {
		printf("Could not open font! (%s)\n", TTF_GetError());
		return -1;
	}
	setUI(font);
	FILE* file = fopen("20231117.txt", "w+");

	if (file == NULL) {
		printf("Failed to open save file!\n");
		return 0;
	}


	printf("Start\n");
	int done = 0;
	double mouseX = -999;
	double mouseY = -999;
	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			mouseX = event.button.x;
			mouseY = event.button.y;
			for (int i = 0; i < buttoncount; i++) {
				if (mouseX >= button[i].x && mouseX <= button[i].x + button[i].w && mouseY >= button[i].y && mouseY <= button[i].y + button[i].h) {
					printf("%d클릭됨\n", button[i].id);
					buttonAction(button[i].id);
				}
			}
		}
		else if (event.type == SDL_MOUSEMOTION) {
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			if (last_hovered_button == -1) {
				for (int i = 0; i < buttoncount; i++) {
					if (mouseX >= button[i].x && mouseX <= button[i].x + button[i].w && mouseY >= button[i].y && mouseY <= button[i].y + button[i].h) {
						Button(1, button[i].x, button[i].y, button[i].w, button[i].h,
							button[i].textX, button[i].textY, button[i].BGC, button[i].HOVC,
							button[i].BRC, button[i].BTK, button[i].TXC, button[i].TXT,
							button[i].FONT, button[i].FS, button[i].unicode);
						SDL_Delay(100);
						last_hovered_button = button[i].id;
						break;
					}
				}
			}
		}
		if (last_hovered_button != -1) {
			if (!(mouseX >= button[last_hovered_button - 1].x && mouseX <= button[last_hovered_button - 1].x + button[last_hovered_button - 1].w && mouseY >= button[last_hovered_button - 1].y && mouseY <= button[last_hovered_button - 1].y + button[last_hovered_button - 1].h)) {
				Button(1, button[last_hovered_button - 1].x, button[last_hovered_button - 1].y,
					button[last_hovered_button - 1].w, button[last_hovered_button - 1].h,
					button[last_hovered_button - 1].textX, button[last_hovered_button - 1].textY, button[last_hovered_button - 1].BGC,
					button[last_hovered_button - 1].HOVC, button[last_hovered_button - 1].BRC,
					button[last_hovered_button - 1].BTK, button[last_hovered_button - 1].TXC,
					button[last_hovered_button - 1].TXT,
					button[last_hovered_button - 1].FONT, button[last_hovered_button - 1].FS,
					button[last_hovered_button - 1].unicode);
				last_hovered_button = -1;
			}
		}

		if (event.type == SDL_KEYDOWN) {
			//Rect((inputStringIndex * 14 - 14) % 960, (inputStringIndex * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
			//Rect((inputStringIndex * 14 + 14) % 960, ((inputStringIndex * 14 + 14) / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
			char key = event.key.keysym.sym;
			if (SDL_GetModState() & KMOD_CTRL && key == SDLK_s) {
				save();
				SDL_Delay(100);
			}
			else if (SDL_GetModState() & KMOD_CTRL && key == SDLK_o) {
				load();
				SDL_Delay(100);
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (inputStringIndex > 0) {
					for (int i = inputStringIndex + 1; i <= inputStringLen + 1; i++) {
						Rect((i * 14) % 960, (i * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
					}
					inputStringIndex--;
				}
				SDL_Delay(100);
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (inputStringIndex < inputStringLen) {
					for (int i = 0; i <= inputStringIndex + 1; i++) {
						Rect((i * 14) % 960, (i * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
					}
					inputStringIndex++;
				}
				SDL_Delay(100);
			}
			else if (key == SDLK_BACKSPACE) {
				if (inputStringLen > 0) {
					inputStringLen--;
					inputString = (char*)realloc(inputString, sizeof(char) * (inputStringLen + 1));
					for (int i = inputStringIndex; i < inputStringLen; i++) {
						inputString[i] = inputString[i + 1];
					}
					Rect(0, 35, 960, (inputStringIndex * 14 / 960 + 1) * 26, "w", 0, "00000000");
					inputString[inputStringLen] = '\0';
					printf("\n%s", inputString);
					for (int i = 0; i < inputStringLen; i++) {
						char text[2] = { inputString[i], '\0' };
						Text(text, (i * 14 - 14) % 960, (i * 14 / 960) * 18 + 35, "fonts/NanumGothic.ttf", 18, "#00000000", 0);
					}
					Rect((inputStringIndex * 14 - 42) % 960, ((inputStringIndex * 14 - 14) / 960) * 18 + 56, 12, 3, "#000000", 0, "00000000");
					inputStringIndex--;
					printf("index=%d len=%d\n", inputStringIndex, inputStringLen);
				}
			}
			else if (SDL_GetModState() & KMOD_SHIFT && (key >= 32 && key <= 126)) {
				char temp = key;
				inputStringLen++;
				if (key >= 'a' && key <= 'z') {
					temp = key - 'a' + 'A';
				}
				else if (key >= SDLK_0 && key <= SDLK_9) {
					switch (key)
					{
					case '1':
						temp = '!';
						break;
					case '2':
						temp = '@';
						break;
					case '3':
						temp = '#';
						break;
					case '4':
						temp = '$';
						break;
					case '5':
						temp = '%';
						break;
					case '6':
						temp = '^';
						break;
					case '7':
						temp = '&';
						break;
					case '8':
						temp = '*';
						break;
					case '9':
						temp = '(';
						break;
					case '0':
						temp = ')';
						break;

					default:
						break;
					}
				}
				else {
					switch (key)
					{
					case '`':
						temp = '~';
						break;
					case '-':
						temp = '_';
						break;
					case '=':
						temp = '+';
						break;
					case '[':
						temp = '{';
						break;
					case ']':
						temp = '}';
						break;
					case '\\':
						temp = '|';
						break;
					case ';':
						temp = ':';
						break;
					case '\'':
						temp = '"';
						break;
					case ',':
						temp = '<';
						break;
					case '.':
						temp = '>';
						break;
					case '/':
						temp = '?';
						break;
					default:
						break;
					}
				}
				inputString = (char*)realloc(inputString, sizeof(char) * (inputStringLen + 1));
				for (int i = inputStringLen - 1; i > inputStringIndex; i--) {
					inputString[i] = inputString[i - 1];
				}
				for (int i = inputStringIndex + 1; i <= inputStringLen + 1; i++) {
					Rect((i * 14) % 960, (i * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");char text[2] = { inputString[i], '\0' };
					Text(text, (i * 14 - 14) % 960, (i * 14 / 960) * 18 + 35, "fonts/NanumGothic.ttf", 18, "#00000000", 0);
				}
				inputString[inputStringIndex] = temp;
				inputString[inputStringLen] = '\0';
				inputStringIndex++;
				printf("%s\n", inputString);
				printf("index=%d len=%d\n", inputStringIndex, inputStringLen);
				for (int i = inputStringIndex + 1; i < inputStringLen; i++) {
					Rect((i * 14) % 960, (i * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
					char text[2] = { inputString[i], '\0' };
					Text(text, (i * 14 - 14) % 960, (i * 14 / 960) * 18 + 35, "fonts/NanumGothic.ttf", 18, "#00000000", 0);
				}
				Rect((inputStringIndex * 14 - 14) % 960, ((inputStringIndex * 14 - 14) / 960) * 18 + 56, 12, 3, "#000000", 0, "00000000");
				Rect((inputStringIndex * 14 - 28) % 960, ((inputStringIndex * 14 - 14) / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
			}
			else if (key >= 32 && key <= 126) {
				char temp = key;
				inputStringLen++;
				inputString = (char*)realloc(inputString, sizeof(char) * (inputStringLen + 1));
				for (int i = inputStringLen - 1; i > inputStringIndex; i--) {
					inputString[i] = inputString[i - 1];
				}
				inputString[inputStringIndex] = temp;
				inputString[inputStringLen] = '\0';
				inputStringIndex++;
				printf("%s\n", inputString);
				printf("index=%d len=%d\n", inputStringIndex, inputStringLen);
				for (int i = inputStringIndex + 1; i < inputStringLen; i++) {
					Rect((i * 14) % 960, (i * 14 / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
					char text[2] = { inputString[i], '\0' };
					Text(text, (i * 14 - 14) % 960, (i * 14 / 960) * 18 + 35, "fonts/NanumGothic.ttf", 18, "#00000000", 0);
				}				
				Rect((inputStringIndex * 14 - 14) % 960, ((inputStringIndex * 14 - 14) / 960) * 18 + 56, 12, 3, "#000000", 0, "00000000");
				Rect((inputStringIndex * 14 - 28) % 960, ((inputStringIndex * 14 - 14) / 960) * 18 + 56, 12, 3, "w", 0, "00000000");
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();

	fclose(file);
	printf("End\n");

	return 0;
}





//함수들
struct Color toRGB(char* hex) {
	struct Color returnColor = {
		ctoi(hex[1]) * 16 + ctoi(hex[2]),
		ctoi(hex[3]) * 16 + ctoi(hex[4]),
		ctoi(hex[5]) * 16 + ctoi(hex[6]),
		ctoi(hex[7]) * 16 + ctoi(hex[8]),
	};

	return returnColor;
}

//창 생성 함수
int setup(int window_width, int window_height, char name[]) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL! (%s)\n", SDL_GetError());
		return -1;
	}

	// Create window
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf("Could not create window! (%s)\n", SDL_GetError());
		return -1;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		printf("Could not create renderer! (%s)\n", SDL_GetError());
		return -1;
	}

	// Clear renderer (white)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}
//UI 생성
void setUI(TTF_Font* font) {
	Rect(0, 0, 960, 30, "#ccccccff", 2, "0xffffffff", 1, 0);
	Button(0, 0, 2, 60, 26, 18, 5, "#ccccccff", "#ffffffff", "#00000000", 1, "#00000000", "파일", "fonts/NanumGothic.ttf", 15, 1);
	Button(0, 60, 2, 80, 26, 62, 5, "#cccccc00", "#ffffff00", "#00000000", 1, "#00000000", "텍스트 추가", "fonts/NanumGothic.ttf", 15, 1);
	Button(0, 140, 2, 60, 26, 149, 5, "#cccccc00", "#ffffff00", "#00000000", 1, "#00000000", "그리기", "fonts/NanumGothic.ttf", 15, 1);
	Button(0, windowWidth - 60, 2, 60, 26, windowWidth - 60 + 1, 5, "#cccccc00", "#ffffff00", "#00000000", 1, "#00000000", "다시실행", "fonts/NanumGothic.ttf", 15, 1);
	Button(0, windowWidth - 120, 2, 60, 26, windowWidth - 120 + 1, 5, "#cccccc00", "#ffffff00", "#00000000", 1, "#00000000", "실행취소", "fonts/NanumGothic.ttf", 15, 1);
}

void Button(int update, int boxx, int boxy, int boxw, int boxh, int textx, int texty, char* boxc, char* hoverc, char* borderc, int bordert, char* textc, char* text, char* font, int fonts, int unicode) {
	if (!update) {
		buttoncount++;
		button = (struct Button*)realloc(button, sizeof(struct Button) * buttoncount);
		(button[buttoncount - 1]).id = buttoncount;
		(button[buttoncount - 1]).x = boxx;
		(button[buttoncount - 1]).y = boxy;
		(button[buttoncount - 1]).w = boxw;
		(button[buttoncount - 1]).h = boxh;
		(button[buttoncount - 1]).textX = textx;
		(button[buttoncount - 1]).textY = texty;
		(button[buttoncount - 1]).BGC = boxc;
		(button[buttoncount - 1]).HOVC = hoverc;
		(button[buttoncount - 1]).BRC = borderc;
		(button[buttoncount - 1]).BTK = bordert;
		(button[buttoncount - 1]).TXC = textc;
		(button[buttoncount - 1]).TXT = text;
		(button[buttoncount - 1]).FONT = font;
		(button[buttoncount - 1]).FS = fonts;
		(button[buttoncount - 1]).unicode = unicode;
	}
	if (last_hovered_button != -1 || update == 0) {
		Rect(boxx, boxy, boxw, boxh, boxc, bordert, borderc);
	}
	else {
		Rect(boxx, boxy, boxw, boxh, hoverc, bordert, borderc);
	}
	Text(text, textx, texty, font, fonts, textc, unicode);
}

//사각형 생성
void Rect(int x, int y, int width, int height, char* fillColor, int borderThickness, char* borderColor) {
	struct Color Fillc = toRGB(fillColor);
	if (fillColor[0] == 'w' || fillColor[0] == 'W') {
		Fillc.R = 255;
		Fillc.G = 255;
		Fillc.B = 255;
		Fillc.A = 0;
	}
	struct Color Borderc = toRGB(borderColor);
	SDL_Rect shape = { x + borderThickness, y + borderThickness, width - borderThickness * 2, height - borderThickness * 2 };
	SDL_Rect outline = { x , y , width, height };
	SDL_SetRenderDrawColor(renderer, Borderc.R, Borderc.G, Borderc.B, Borderc.A);
	SDL_RenderFillRect(renderer, &outline);
	SDL_SetRenderDrawColor(renderer, Fillc.R, Fillc.G, Fillc.B, Fillc.A);
	SDL_RenderFillRect(renderer, &shape);

	// Update screen
	SDL_RenderPresent(renderer);
}

int Text(const char* strings, int x, int y, char* fontRoot, int fontSize, char* color, int unicode) {
	TTF_Font* font = TTF_OpenFont(fontRoot, fontSize);
	if (font == NULL) {
		printf("폰트 로딩 오류!\n");
		return -1;
	}
	struct Color c = toRGB(color);
	SDL_Color tcolor = { c.R , c.G, c.B, c.A };
	SDL_Surface* surface;

	if (unicode == 1) {
		Uint8* unicodeString = (Uint8*)malloc(sizeof(strings) * 8);
		han2unicode(strings, unicodeString);
		surface = TTF_RenderUNICODE_Solid(font, unicodeString, tcolor);
		free(unicodeString);
	}
	else {
		surface = TTF_RenderText_Solid(font, strings, tcolor);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect r = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &r);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return 0;
}

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

int ctoi(char c) {
	if ((c >= 'a' && c <= 'f') && (c < '0' || c > '9')) {
		c += 'A';
	}
	switch (c)
	{
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	case 'A':
		return 10;
		break;
	case 'B':
		return 11;
		break;
	case 'C':
		return 12;
		break;
	case 'D':
		return 13;
		break;
	case 'E':
		return 14;
		break;
	case 'F':
		return 15;
		break;
	default:
		break;
	}
}

void buttonAction(int id) {
	char str[1000];
	int pointCount = 0;
	int mouseX = -999, mouseY = -999;
	switch (id) {
	case 1:
		break;
	case 2:
		printf("텍스트를 입력해 주세요 >> ");
		scanf("%[^\n]s", str);
		getchar();
		object = (struct object*)realloc(object, sizeof(object) * objectcount + 1);
		object[objectcount].id = objectcount;
		objectcount++;

		printf("텍스트를 넣을 위치를 선택하세요 >> \n\n");
		while (1) {
			SDL_PollEvent(&event);
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				mouseX = event.button.x;
				mouseY = event.button.y;
				if (mouseY <= 30) {
					mouseY = 30;
				}
				Text(str, mouseX, mouseY, "fonts/NanumGothic.ttf", 15, "#00000000", 1, objectcount);
				break;
			}
			else if (event.type == SDL_MOUSEMOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;

				refresh();
				SDL_Delay(100);
			}
		}
		break;
	case 3:
		object = (struct object*)realloc(object, sizeof(object) * objectcount + 1);
		object[objectcount].id = objectcount;
		objectcount++;
		SDL_PollEvent(&event);
		while (1) {
			if (event.type == SDL_MOUSEMOTION) {
				//object[objectcount].point->x = (struct Point*)realloc(object[objectcount].point->x, sizeof(int) * pointCount + 1);
				//object[objectcount].point->y = (struct Point*)realloc(object[objectcount].point->y, sizeof(int) * pointCount + 1);
				if (mouseY <= 30) {
					mouseY = 30;
				}
				Rect(mouseX, mouseY, 5, 5, "#00000000", 0, "#00000000");

				//object[objectcount].point->x[pointCount] = mouseX;
				//object[objectcount].point->y[pointCount] = mouseY;
				//printf("그리는 중... x=%d, y=%d\n", object[objectcount].point->x[pointCount],
				//	object[objectcount].point->y[pointCount]);
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				SDL_Delay(100);
				pointCount++;
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				break;
			}
		}
		printf("빠져나옴");
		break;
	}
}

void refresh() {
	/*for (int i = 0; i < objectcount - 1; i++) {
		if ((object[i].x <= disposableObject[disposableObjectcount].x && object[i].x + object[i].width >= disposableObject[disposableObjectcount].x) ||
			(object[i].x + object[i].width <= disposableObject[disposableObjectcount].x &&
				object[i].x + object[i].width >= disposableObject[disposableObjectcount].x + disposableObject[disposableObjectcount].w)) {
			printf("겹침  ");
		}
	}*/
}

void save() {
	char filePath[256];
	printf("\n파일 경로와 이름을 입력하세요 (예: C:/path/to/file.txt): ");
	scanf("%s", filePath);
	filePath[strlen(filePath) + 1] = '\0';
	getchar(); // 개행 문자 처리

	FILE* File = fopen(filePath, "w+");
	fprintf(File, "%s", inputString);
	fclose(File);
}

void load() {
	char filePath[256];
	char buffer[1000];
	printf("\n파일 경로와 이름을 입력하세요 (예: C:/path/to/file.txt): ");
	scanf("%s", filePath);
	filePath[strlen(filePath) + 1] = '\0';
	getchar(); // 개행 문자 처리

	FILE* File = fopen(filePath, "r");
	if (File == NULL) {
		printf("파일 열기 실패\n");
	}
	fgets(buffer, sizeof(buffer), File);
	
	inputString = (char*)realloc(inputString, sizeof(char) * (strlen(buffer) + 2));
	inputStringLen = strlen(buffer) + 1;
	inputStringIndex = inputStringLen;
	for (int i = 0; i <= inputStringLen; i++) {
		printf("불러오는 중...");
		inputString[i] = buffer[i];
		char text[2] = { inputString[i], '\0' };
		if (i != inputStringLen - 1) {
			Text(text, (i * 14 - 14) % 960, (i * 14 / 960) * 18 + 35, "fonts/NanumGothic.ttf", 18, "#00000000", 0);
		}
	}
	printf("불러오기 완료...");

	fclose(File);

		printf("불러오기 완료...");
}