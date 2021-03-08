#include <stdlib.h>
//include "MapCreation.h"
//include <stdio.h>
#include "novikoncept.h"
#include "Score.h"


int **map;
int map_size;
bomb **bombs;
player *ar;
int player_number;
SDL_Window *window;
SDL_Renderer *renderer;
void explode(Uint32 inter,cord *c);
TTF_Font* font;
int score = 0;

const char mainMenu[7][15] = {"Demo", "Play(Random)","Play(Selected)","Highscore","Edit","Exit","Main Menu" };
const char tezinaM[5][15] = {  "Easy", "Medium", "Hard", "Main Menu","AI Difficulty" };
const char velicina[5][15] = {  "Small", "Medium", "Large", "Main Menu","Size of map" };
const char pauza[3][15] = { "Continue", "Main Menu","Pause" };
const char highscore[2][15] = {  "Main Menu","Highscore" };
const char edit[6][15] = { "Empty","Door","D Block","UD Block","Save","Main Menu" };


int game(tezina dif, char demo, SDL_Texture *texture1, SDL_Texture *texture2, SDL_Texture *texture3, SDL_Texture *texture4, SDL_Texture *texture5, SDL_Texture *texture6, SDL_Texture *texture7) {

	score = 0;

	setDiff(dif);
	bomb *bomb_list;
	bomb_list = NULL;
	int reach = 4;
	int i = 0, j;
	char mv;

	if (!map)
		switch (map_size) {
		case 8:
			map = (int**)RandMap('S');
			break;
		case 12:
			map = (int**)RandMap('M');
			break;
		case 20:
			map = (int**)RandMap('L');
			break;
		default:
			map_size = 12;
			map = (int**)RandMap('M');
			break;
		}

	SDL_Rect* ViewPort = malloc(sizeof(SDL_Rect));
	ViewPort->h = map_size * 30;
	ViewPort->w = map_size * 30;
	ViewPort->x = 350 - ViewPort->w / 2;
	ViewPort->y = 350 - ViewPort->h / 2;;
	bombs = malloc(map_size * sizeof(bomb*));
	for (i = 0; i < map_size; i++)
		bombs[i] = calloc(map_size, sizeof(bomb));

	ar = initAi(map_size, map_size, player_number);
	int *n = malloc(sizeof(int));
	(*n) = 0;
	int done = 0;
	for (i = 4; i < player_number; i++)
		map = Clear(map, map_size, (ar + i)->x, (ar + i)->y);



	if (!demo)
	{
		ar->x = 0; 	ar->y = 0;
		ar->brojBombi = 3;
		ar->ziv = 1;
		ar->tip = IGRAC;
	}

	int running = 1;
	while ((!reachExit(ar)) && (ar->ziv) && (!done)) {
		/*
		1. pomeri igraca
		2. update AI
		3. nacrtaj mapu
		*/

		i = 0;
		if (running)
		{
			if (!demo)
			{
				i = 1;
			}
			for (; i < player_number; i++)
				executeState(map, ar + i, ar);
			//codina fja za crtanje
			Man **men = getMen(map, map_size, n, player_number);
			done = processEvents(renderer, window, ar, map_size, font);
			if (done == 2)
			{
				running = 0;
				pauseTimer();
				int tef = showmenu(renderer, font, 14, pauza, 3, 4);
				switch (tef)
				{
				case 14:
				{
					resumeTimer();
					running = 1;
					done = 0;
					break;
				}
				case 15:
				{
					ar->ziv = 0;
					break;
				}
				}
			}
			else if (done == 0)
			{
				running = 1;
			}
			int num = (*n);
			if (running)
			{
				doRender(renderer, men, num, ar, player_number, ViewPort, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
				for (i = 0; i < num; i++)
				{
					free(men[i]);
				}
				free(men);
				(*n) = 0;
				SDL_Delay(120);
			}

		}
	}
	free(ViewPort);
	for (i = 0; i < map_size; i++)
	{
		for (j = 0; j < map_size; j++)
			if (bombs[i][j].id)
			{
				bombs[i][j].id = !SDL_RemoveTimer(bombs[i][j].id);
			}
		free(bombs[i]);
	}
	free(ar);
	ar = NULL;
	free(bombs);
	DelMap(map,map_size);
	map = NULL;
	return score;
}

int main(int argc, char *argv[]) {


	srand(time(0));

	SDL_Init(SDL_INIT_TIMER);
	window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 700, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *temp1 = IMG_Load("Player1L.png");
	Uint32 colorkey = SDL_MapRGB(temp1->format, 255, 255, 255);
	SDL_SetColorKey(temp1, SDL_TRUE, colorkey);
	SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, temp1);
	SDL_FreeSurface(temp1);
	SDL_Surface *temp2 = IMG_Load("Enemy2L.png");
	colorkey = SDL_MapRGB(temp2->format, 255, 255, 255);
	SDL_SetColorKey(temp2, SDL_TRUE, colorkey);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, temp2);
	SDL_FreeSurface(temp2);
	SDL_Surface *temp3 = IMG_Load("bomb.png");
	colorkey = SDL_MapRGB(temp3->format, 255, 255, 255);
	SDL_SetColorKey(temp3, SDL_TRUE, colorkey);
	SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, temp3);
	SDL_FreeSurface(temp3);
	SDL_Surface *temp4 = IMG_Load("DBlock.png");
	colorkey = SDL_MapRGB(temp4->format, 255, 255, 255);
	SDL_SetColorKey(temp4, SDL_TRUE, colorkey);
	SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, temp4);
	SDL_FreeSurface(temp4);
	SDL_Surface *temp5 = IMG_Load("UDBlock.png");
	colorkey = SDL_MapRGB(temp5->format, 255, 255, 255);
	SDL_SetColorKey(temp5, SDL_TRUE, colorkey);
	SDL_Texture *texture5 = SDL_CreateTextureFromSurface(renderer, temp5);
	SDL_FreeSurface(temp5);
	SDL_Surface *temp6 = IMG_Load("Door.png");
	colorkey = SDL_MapRGB(temp6->format, 255, 255, 255);
	SDL_SetColorKey(temp6, SDL_TRUE, colorkey);
	SDL_Texture *texture6 = SDL_CreateTextureFromSurface(renderer, temp6);
	SDL_FreeSurface(temp6);
	SDL_Surface *temp7 = IMG_Load("Fire.png");
	colorkey = SDL_MapRGB(temp7->format, 255, 255, 255);
	SDL_SetColorKey(temp7, SDL_TRUE, colorkey);
	SDL_Texture *texture7 = SDL_CreateTextureFromSurface(renderer, temp7);
	SDL_FreeSurface(temp7);
	

	TTF_Init();
	font = TTF_OpenFont("Xenotron.ttf", 30);

	
	tezina dif = TESKO;
	player_number = 4;
	
	char c;
	int x = showmenu(renderer, font, 14, mainMenu, 7, 1);
	int a = 0;


	while (x != 0)
	{
		switch (x)
		{
		case 1:
		{
			map_size = 12;
			player_number = 4;
			game(TESKO, 1, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
			x = 9;
			break;
		}
		case 2:
		{
			x = showmenu(renderer, font, 14, tezinaM, 5, 2);
			break;	
		}
		case 3:
		{
			x = showmenu(renderer, font, map_size, tezinaM, 5, 3);
		    if ((x != 13) && (x != 9))
		    {
			    map = pSelected(renderer, font,dif);
				if (map)
				{
					if (x == 6)
					{
						dif = LAKO;
					}
					if (x == 7)
					{
						dif = NORMALNO;
					}
					if (x == 8)
					{
						dif = TESKO;
					}

					map_size = map[0][0];
					map[0][0] = 0;
					x = game(dif, 0, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
					showScore(renderer, font, score);
				}
				x = 13;
		    }
		    break;
			
		}
		case 4:
		{
			x = showmenu(renderer, font, map_size, tezinaM, 5, 3);
			if (x != 9)
			{
				if (x == 10)
				{
					x = hscores(renderer, font, LAKO);
				}
				if (x == 11)
				{
					x = hscores(renderer, font, NORMALNO);
				}
				if (x == 12)
				{
					x = hscores(renderer, font,TESKO);
				}
			}
			break;
			
		}
		case 5:
		{
			x = showmenu(renderer, font, 14, velicina, 5, 6);
			break;
		}
		case 27:
		{
			x = 0;
			break;
		}

		case 6:
		{
			dif = LAKO;
			x = showmenu(renderer, font, 14, velicina,5,3);
			break;
		}
		case 7:
		{
			dif = NORMALNO;
			x = showmenu(renderer, font, 14,velicina,5,3);
			break;
		}
		case 8:
		{
			dif = TESKO;
			x = showmenu(renderer, font, 14, velicina, 5, 3);
			break;
		}
		case 9:
		{
			x = showmenu(renderer, font, 14, mainMenu, 7, 1);
			break;
		}
		case 10:
		{
			map_size = 8;
			player_number = 4;
		    game(dif, 0, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
			showScore(renderer, font, score);
			x = 9;
			break;
		}
		case 11:
		{
			map_size = 12;
			player_number = 8;
			game(dif, 0, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
			showScore(renderer, font, score);
			x = 13;
			break;
		}
		case 12:
		{
			map_size = 20;
			player_number = 12;
			game(dif,0, texture1, texture2, texture3, texture4, texture5, texture6, texture7);
			showScore(renderer, font, score);
			x = 13;
			break;
		}
		case 13:
		{
			x = showmenu(renderer, font, 14, mainMenu,7,1);
			break;
		}
		case 16:
		{
			x = showmenu(renderer, font, 14, mainMenu,7,1);
			break;

		}
		case 17:
		{
			x = editmenu(renderer, font, 8, edit);
			break;
		}
		case 18:
		{
			x = editmenu(renderer, font, 12, edit);
			break;
		}
		case 19:
		{
			x = editmenu(renderer, font, 20, edit);
			break;
		}
		case 20:
		{
			x = showmenu(renderer, font, 14, velicina, 5, 6);
			switch(x)
			{
			case 10:
				x = 17;
				break;
			case 11:
				x = 18;
				break;
			case 12:
				x = 19;
				break;
			}
		}

		}
	}

	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyTexture(texture4);
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);


	SDL_Quit();

	SDL_DestroyWindow(window);
	//SDL_DestroyRenderer(renderer);

	return 0;
}
