#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")

struct Object {
	char* name;
	int x;
	int y;
	int width;
	int height;
	int zIndex;
	char type;
	char* innerText;
	struct Color* color;
	struct Color* textColor;
};

struct Color {
	int R;
	int G;
	int B;
	int A;
};

SDL_Window* window;
SDL_Renderer* renderer;

int setup(int, int, char[]);
struct Color toRGB(int);
void Rect(int, int, int, int, int, int, int);
char* itos(int);

int main(int argc, char* argv[]) {
	FILE* file = fopen("20231117.txt", "w+");
	TTF_Font* pFont = TTF_OpenFont("C:\\Users\\chjih\\source\\repos\\Project20231103\\Project20231103\\font\\NotoSansKR - VariableFont_wght.ttf", 30);
	if (file == NULL) {
		printf("Failed to open!\n");
		return 0;
	}
	struct Object obj = { "object 1", 0, 0, 1, 'T' };
	//save(obj, f);
	printf("Start\n");
	setup(960, 480, "TEST window");

	Rect(0, 0, 960, 30, 0xcccccc00, 1, 0x000000ff);
	SDL_Event event;
	int done = 0;


	while (!done) {
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) {
			done = 1;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			printf("%d\n", event.button.x);
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


struct Color toRGB(int hex) {
	struct Color returnColor = {
		hex / 0x01000000,
		hex % 0x01000000 / 0x00010000,
		hex % 0x00010000 / 0x00000100,
		hex % 0x00000100
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


//사각형 생성
void Rect(int x, int y, int width, int height, int fillColor, int borderThickness, int borderColor) {
	struct Color Fillc = toRGB(fillColor);
	struct Color Borderc = toRGB(borderColor); 
	SDL_Rect shape = { x, y, width, height };
	SDL_Rect outline = { x - borderThickness , y - borderThickness , width + borderThickness * 2, height + borderThickness * 2 };
	SDL_SetRenderDrawColor(renderer, Borderc.R, Borderc.G, Borderc.B, Borderc.A);
	SDL_RenderFillRect(renderer, &outline);
	SDL_SetRenderDrawColor(renderer, Fillc.R, Fillc.G, Fillc.B, Fillc.A);
	SDL_RenderFillRect(renderer, &shape);
	
	// Update screen
	SDL_RenderPresent(renderer);
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
