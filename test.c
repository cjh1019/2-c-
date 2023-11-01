#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>


int main(int argc, char* argv[]) {
    // SDL �ʱ�ȭ
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL �ʱ�ȭ ����: %s\n", SDL_GetError());
        return 1;
    }

    // ������ ����
    SDL_Window* window = SDL_CreateWindow("SDL ����", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("������ ���� ����: %s\n", SDL_GetError());
        return 1;
    }

    // ������ ����
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("������ ���� ����: %s\n", SDL_GetError());
        return 1;
    }

    // ���� ����
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // �׸��� �ڵ� ���⿡ �߰�

        // ������
        SDL_RenderPresent(renderer);
    }

    // ����
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}