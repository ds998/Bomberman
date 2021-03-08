#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "MapCreation.h"
#include "Logic.h"
#include <string.h>

typedef struct qwe{
	int x, y;
	int attr;
}Man;
void editRender(SDL_Renderer *renderer, Man **men, int num, SDL_Rect* ViewPort, SDL_Texture *texture4, SDL_Texture *texture5, SDL_Texture *texture6);
int editmenu(SDL_Renderer* renderer, TTF_Font* font, int map_size, const char labels[][15]);
int showmenu(SDL_Renderer* renderer, TTF_Font* font,int map_size, const char labels[][15],int length,int qef);
int processEvents(SDL_Window *window, SDL_Renderer *renderer, player *man, int dim);
void doRender(SDL_Renderer *renderer, Man **men, int num, player *players, int num2,SDL_Rect* ViewPort, SDL_Texture *texture1, SDL_Texture *texture2, SDL_Texture *texture3, SDL_Texture *texture4, SDL_Texture *texture5, SDL_Texture *texture6,SDL_Texture *texture7);
int hscores(SDL_Renderer *renderer, TTF_Font *font,tezina diff);
Man **getMen(int **mat, int dim, int *n, int n2);
int showScore(SDL_Renderer* renderer, TTF_Font* font, int score);
int pSelected(SDL_Renderer* renderer, TTF_Font* font);