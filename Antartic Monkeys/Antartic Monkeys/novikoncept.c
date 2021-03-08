#include "novikoncept.h"
#include <string.h>
#include "Score.h"

extern int **matricaSigurnosti;
extern tezina diff;

void editRender(SDL_Renderer *renderer, Man **men, int num, SDL_Rect* ViewPort, SDL_Texture *texture4, SDL_Texture *texture5, SDL_Texture *texture6)
{
	SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
	SDL_RenderSetViewport(renderer, ViewPort);
	SDL_Rect* ViewPort2 = malloc(sizeof(SDL_Rect));
	ViewPort2->x = 0;
	ViewPort2->y = 0;
	ViewPort2->h = ViewPort->h;
	ViewPort2->w = ViewPort->w;
	SDL_RenderFillRect(renderer, ViewPort2);


	SDL_Rect rect;
	int i;
	if (num > 0)
	{
		for (i = 0; i < num; i++)
		{
			rect.x = men[i]->x;
			rect.y = men[i]->y;
			rect.h = 30;
			rect.w = 30;
			if (men[i]->attr == 1 || men[i]->attr == 4)
			{
				SDL_RenderCopy(renderer, texture4, NULL, &rect);
			}
			if (men[i]->attr == 2)
			{
				SDL_RenderCopy(renderer, texture6, NULL, &rect);
			}
			if (men[i]->attr == 5)
			{
				SDL_RenderCopy(renderer, texture5, NULL, &rect);
			}

		}
	}

	SDL_RenderPresent(renderer);
	SDL_RenderSetViewport(renderer, NULL);
}
int editmenu(SDL_Renderer* renderer, TTF_Font* font, int map_size, const char labels[][15])
{
	SDL_Surface *temp4 = IMG_Load("DBlock.png");
	Uint32 colorkey = SDL_MapRGB(temp4->format, 255, 255, 255);
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
	int **mat = malloc(map_size * sizeof(int*));
	font = TTF_OpenFont("Xenotron.ttf", 10);
	for (int i = 0; i < map_size; i++)
	{
		mat[i] = malloc(map_size * sizeof(int));
		for (int j = 0; j < map_size; j++)
		{
			mat[i][j] = 0;
		}
	}
	SDL_Surface* screen;
	Uint32 rmask, gmask, bmask, amask;
	int width = 700;
	int height = 700;
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	Uint32 time;
	int x, y;
	SDL_Surface* *menus = malloc(6 * sizeof(SDL_Surface*));
	int *selected = malloc(6 * sizeof(int));
	for (int i = 0; i < 6; i++)
	{
		selected[i] = 0;
	}
	SDL_Color color[2] = { { 255,255,255 },{ 255,0,0 } };
	for (int i = 0; i < 6; i++)
		menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);

	SDL_Rect *pos = malloc(6 * sizeof(SDL_Rect));
	for (int i = 0; i < 6; i++)
	{
		pos[i].x = 20 + i * 90;
		pos[i].y = 680;
		pos[i].w = 5;
		pos[i].h = 5;
	}
	int setcl = 4;
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

	SDL_Event event;
	SDL_Rect* ViewPort = malloc(sizeof(SDL_Rect));
	ViewPort->h = map_size * 30;
	ViewPort->w = map_size * 30;
	ViewPort->x = 350 - ViewPort->w / 2;
	ViewPort->y = 350 - ViewPort->h / 2;
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				for (int i = 0; i<6; i++)
					SDL_FreeSurface(menus[i]);
				//free(menus);
				exit(0);
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0; i < 6; i++)
				{
					if ((x >= pos[i].x) && (x <= pos[i].x + pos[i].w) && (y >= pos[i].y) && (y <= pos[i].y + pos[i].h))
					{
						if (!selected[i])
						{
							selected[i] = 1;
							SDL_FreeSurface(menus[i]);
							menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
						}

					}
					else
					{
						selected[i] = 0;
						SDL_FreeSurface(menus[i]);
						menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0; i < 6; i++)
				{
					x = event.motion.x;
					y = event.motion.y;
					for (int i = 0; i < 6; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y &&y <= pos[i].y + pos[i].h)
						{
							switch (i)
							{
							case 0:
							{
								setcl = 0;
								break;
							}
							case 1:
							{
								setcl = 2;
								break;
							}
							case  2:
							{
								setcl = 4;
								break;
							}
							case 3:
							{
								setcl = 5;
								break;
							}
							case 4:
							{
								if (PossEdit(mat, map_size))
								{
									if (Possible(mat, map_size, 0, 0))
									{
										char **namesmat = ReadNames();
										int f = 0;
										while (namesmat[f])
										{
											f++;
										}
										char name[15] = "editmap";
										char buffer[10];
										_itoa(f, buffer, 10);
										strcat(name, buffer);
										strcat(name, ".txt");
										SaveMap(name, map_size, mat);
										for (int j = 0; j < f; j++)
										{
											free(namesmat[j]);
										}
										free(namesmat);
										for (int j = 0; j < map_size; j++)
										{
											free(mat[j]);
										}
										free(mat);
										SDL_DestroyTexture(texture4);
										SDL_DestroyTexture(texture5);
										SDL_DestroyTexture(texture6);
										free(ViewPort);
										for (int j = 0; j< 6; j++)
										{
											if (menus[j])
											{
												SDL_FreeSurface(menus[j]);
											}
										}
										//free(menus);
										free(selected);
										free(pos);
										SDL_FreeSurface(screen);
										return 9;
										break;
									}
								}

								break;
							}
							case 5:
							{
								SDL_DestroyTexture(texture4);
								SDL_DestroyTexture(texture5);
								SDL_DestroyTexture(texture6);
								free(ViewPort);
								for (int j = 0; j < 6; j++)
								{
									if (menus[j])
									{
										SDL_FreeSurface(menus[j]);
									}
								}
								//free(menus);
								free(selected);
								free(pos);
								SDL_FreeSurface(screen);
								for (int j = 0; j < map_size; j++)
								{
									free(mat[j]);
								}
								free(mat);
								return 9;
								break;
							}
							}
						}

					}
					if ((x >= ViewPort->x) && (x <= ViewPort->x + ViewPort->w) && (y >= ViewPort->y) && (y <= ViewPort->y + ViewPort->h))
					{
						int i = (x - ViewPort->x) / 30;
						int j = (y - ViewPort->y) / 30;
						if (!Forbidden(j, i, map_size)) mat[j][i] = setcl;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					for (int j = 0; j < 6; j++)
					{
						SDL_FreeSurface(menus[j]);
					}
					//free(menus);
					return 0;
				}

			}
			for (int i = 0; i < 6; i++)
			{
				SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
			}
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_DestroyTexture(texture);
			int *n = malloc(sizeof(int));
			(*n) = 0;
			Man **men = getMen(mat, map_size, n, 0);
			editRender(renderer, men, (*n), ViewPort, texture4, texture5, texture6);
			time = SDL_GetTicks();
			for (int i = 0; i < (*n); i++)
			{
				free(men[i]);
			}
			free(men);
		}


		if (1000 / 30 >(SDL_GetTicks() - time))
		{
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
		}

	}


}

int showmenu(SDL_Renderer* renderer, TTF_Font* font,int map_size,  char labels[][15], int length,int qef)
{
	
	SDL_Surface* screen;
	Uint32 rmask, gmask, bmask, amask;
	int width = 700;
	int height = 700;
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{ 
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	Uint32 time;
	int x, y;
	SDL_Surface* *menus = malloc(length * sizeof(SDL_Surface*));
	int *selected = malloc(length * sizeof(int));

	SDL_Color color[2] = { {255,255,255},{255,0,0} };
	for (int i = 0; i < length; i ++)
		menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
	
	SDL_Rect *pos = malloc(length * sizeof(SDL_Rect));
	for (int i = 0; i < length; i++)
	{
		pos[i].x = 180;
		pos[i].y = 250 + i * 40;
		pos[i].w = 20;
		pos[i].h = 20;
	}
	pos[length-1].y = pos[0].y - 150;
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	
	SDL_Event event;
	while (1)
	{
		
		
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			    case SDL_QUIT:
					for(int i=0;i<length;i++)
					    SDL_FreeSurface(menus[i]);
					SDL_FreeSurface(screen);
					//free(menus);
					exit(0);
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (int i = 0; i < length-1; i++)
					{
						if ((x >= pos[i].x )&& (x <= pos[i].x + pos[i].w) &&( y >= pos[i].y) &&(y <= pos[i].y + pos[i].h))
						{
							if (!selected[i])
							{
								selected[i] = 1;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
							}
							
						}
						else
						{
							selected[i] = 0;
							SDL_FreeSurface(menus[i]);
							menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.motion.x;
					y = event.motion.y;
					for (int i = 0; i < length-1; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y &&y <= pos[i].y + pos[i].h)
						{
							for (int j = 0; j < length; j++)
							{
								SDL_FreeSurface(menus[j]);
								
							}
							free(pos); 
							//free(menus);
							//free(selected);
							SDL_FreeSurface(screen);
							switch (qef)
							{
								
								
							    case 1:
								{
									switch (i) {

									    case 0:
										{
											return 1;
											break;
										}
										case 1:
										{
											return 2;
											break;
										}
										case 2:
										{
											return 3;
											break;
										}
										case 3:
										{
											return 4;
											break;
										}
										case 4:
										{
											return 20;
											break;
										}
										case 5:
										{
											return 27;
											break;
										}
									}
									break;
								}
								case 2:
								{
									switch (i)
									{
									    case 0:
										{
											return 6;
											break;
										}
										case 1:
										{
											return 7;
											break;
										}
										case 2:
										{
											return 8;
											break;
										}
										case 3:
										{
											return 9;
											break;
										}
									}
									break;
								}
								case 3:
								{
									switch (i)
									{
									    case 0:
										{
											return 10;
											break;
										}
										case 1:
										{
											return 11;
											break;
										}
										case 2:
										{
											return 12;
											break;
										}
										case 3:
										{
											return 13;
											break;
										}
									}
									break;
								}
								case 4:
								{
									switch (i) 
									{
									    case 0:
										{
											return 14;
											break;
										}
										case 1:
										{
											return 15;
											break;
										}
									}
									break;
								}
								case 5:
								{
									switch (i)
									{
									    case 0:
										{
											return 16;
											break;
										}

									}
									break;
								}
								case 6:
								{
									switch (i)
									{
									    case 0:
										{
											return 17;
											break;
										}
										case 1:
										{
											return 18;
											break;
										}
										case 2:
										{
											return 19;
											break;
										}
										case 3:
										{
											return 9;
											break;
										}
									}
									break;
								}
							}
							
						}

					}
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (int j = 0; j < length; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						//free(selected);
						free(pos);
						SDL_FreeSurface(screen);
						return 0;
					}
					
			}
			for (int i = 0; i < length; i++)
			{
				SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
			}
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			SDL_DestroyTexture(texture);
			//SDL_FreeSurface(screen);
			time = SDL_GetTicks();
		}
		
		
		if (1000 / 30 > (SDL_GetTicks() - time))
		{
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
		}

	}
}
int processEvents(SDL_Window *window,SDL_Renderer* renderer,player *man,int dim)
{
	int done = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			    case SDLK_ESCAPE:
				{
					//int i = showmenu(renderer,font);
					done = 2;
					break;
				}
			}
		}
		break;
		case SDL_QUIT:
			done = 1;
			break;

		}
	}
	Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		if (man->y > 0)
		{
			move(man, 0);
		}
		
	}
	if (state[SDL_SCANCODE_RIGHT])		if(man->y < dim-1)

	{
		{
			move(man, 1);
		}
		
	}
	if (state[SDL_SCANCODE_UP])
	{
		if (man->x > 0)
		{
			move(man, 2);
		}
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		if(man->x < dim-1)
		{
			move(man, 3);
		}
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		plant_bomb(man->x, man->y, 4, man);
		if (getDiff() == TESKO)
			safety(matricaSigurnosti, man->x, man->y, 1);
	}
	//free((void*)state);
	return done;
}
void doRender(SDL_Renderer *renderer, Man **men, int num, player *players, int num2,SDL_Rect* ViewPort, SDL_Texture *texture1, SDL_Texture *texture2, SDL_Texture *texture3, SDL_Texture *texture4, SDL_Texture *texture5, SDL_Texture *texture6, SDL_Texture *texture7)
{
	SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
	SDL_RenderSetViewport(renderer, ViewPort);
	SDL_Rect* ViewPort2 = malloc(sizeof(SDL_Rect));
	ViewPort2->h = ViewPort->h;
	ViewPort2->w = ViewPort->w;
	ViewPort2->x = 0;
	ViewPort2->y = 0;
	SDL_RenderFillRect(renderer, ViewPort2);
	free(ViewPort2);
	SDL_Rect rect;
	int i;
	for (i = 0; i < num; i++)
	{
		rect.x = men[i]->x;
		rect.y = men[i]->y;
		rect.h = 30;
		rect.w = 30;
		if (men[i]->attr == 1 || men[i]->attr == 4)
		{
			SDL_RenderCopy(renderer, texture4,NULL, &rect);
		}
		if (men[i]->attr == 2)
		{
			SDL_RenderCopy(renderer, texture6, NULL, &rect);
		}
		if (men[i]->attr == 5)
		{
			SDL_RenderCopy(renderer, texture5, NULL, &rect);
		}
		if (men[i]->attr == 3)
		{
			
			SDL_RenderCopy(renderer, texture3, NULL, &rect);
		}
		if (men[i]->attr == 8)
		{
			SDL_RenderCopy(renderer, texture7, NULL, &rect);
		}

	}
	SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
	rect.x = players[0].y * 30;
	rect.y = players[0].x * 30;
	rect.w = 30;
	rect.h = 30;
	SDL_RenderCopy(renderer, texture1, NULL, &rect);

	for (i = 1; i < num2; i++)
	{
		if (players[i].ziv) {
			rect.x = players[i].y * 30;
			rect.y = players[i].x * 30;
			rect.w = 30;
			rect.h = 30;
			SDL_RenderCopy(renderer, texture2, NULL, &rect);
		}
	}

	SDL_RenderPresent(renderer);
	SDL_RenderSetViewport(renderer,NULL);
}


Man **getMen(int **mat,int dim,int *n,int n2)
{
	int i, j;
	int *posit1 = malloc(dim*dim * sizeof(int));
	int *posit2 = malloc(dim*dim * sizeof(int));
	int dimx = 0, dimy = 0;
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			if (mat[i][j] != 0 && mat[i][j] != 6)
			{
				(*n)++;
				posit1[dimx] = i;
				posit2[dimy] = j;
				dimx++;
				dimy++;
			}
		}
	}
	Man **men = malloc((*n) * sizeof(Man*));
	for (i = 0; i < (*n); i++)
	{
		men[i] = malloc(sizeof(Man));
		men[i]->y = posit1[i] * 30;
		men[i]->x = posit2[i] * 30;
		men[i]->attr = mat[posit1[i]][posit2[i]];

	}
	free(posit1);
	free(posit2);
	return men;

}

	int showScore(SDL_Renderer* renderer, TTF_Font* font, int score)
	{

		char labels[5][15] = { "Name:","AAAAA", "Score: ","Next"};
		int length = 4, poz = 0;

		char br[10];
		_itoa(score, br, 10);
		strcat(labels[2], br);

		SDL_Surface* screen;
		Uint32 rmask, gmask, bmask, amask;
		int width = 700;
		int height = 700;
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		}
		else
		{
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		}
		screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
		Uint32 time;
		int x, y;
		SDL_Surface* *menus = malloc(length * sizeof(SDL_Surface*));
		int *selected = malloc(length * sizeof(int));

		SDL_Color color[2] = { { 255,255,255 },{ 255,0,0 } };
		for (int i = 0; i < length; i++)
			menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);

		SDL_Rect *pos = malloc(length * sizeof(SDL_Rect));
		for (int i = 0; i < length; i++)
		{
			pos[i].x = 180;
			pos[i].y = 200 + i * 40;
			pos[i].w = 20;
			pos[i].h = 20;
		}
		pos[2].y += 100;
		pos[3].y += 200;
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
		Uint8 *state = SDL_GetKeyboardState(NULL);
		SDL_Event event;
		while (1)
		{


			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					for (int i = 0; i<length; i++)
						SDL_FreeSurface(menus[i]);
					//free(menus);
					exit(0);
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					if ((x >= pos[3].x) && (x <= pos[3].x + pos[3].w) && (y >= pos[3].y) && (y <= pos[3].y + pos[3].h))
					{
						if (!selected[3])
						{
							selected[3] = 1;
							SDL_FreeSurface(menus[3]);
							menus[3] = TTF_RenderText_Solid(font, labels[3], color[1]);
						}

					}
					else
					{
						selected[3] = 0;
						SDL_FreeSurface(menus[3]);
						menus[3] = TTF_RenderText_Solid(font, labels[3], color[0]);
					}
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (int j = 0; j < length; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						//free(menus);
						SDL_FreeSurface(screen);
						return 0;
					}
					if (state[SDL_SCANCODE_RIGHT])
					{
						poz = (poz + 1) % 5;
						break;
					}
					if ( state[SDL_SCANCODE_LEFT])
					{
						poz = (poz - 1) % 5;
						break;
					}
					if (state[SDL_SCANCODE_DOWN])
					{
						labels[1][poz]++;
						if (labels[1][poz] > 90)
							labels[1][poz] = 65;
						SDL_FillRect(screen, &pos[1], SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
						SDL_FreeSurface(menus[1]);
						menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
						break;
					}
					if ( state[SDL_SCANCODE_UP])
					{
						labels[1][poz]--;
						if (labels[1][poz] < 65)
							labels[1][poz] = 90;
						SDL_FillRect(screen, &pos[1], SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
						SDL_FreeSurface(menus[1]);
						menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
						break;
					}
					int ok = 1;
					if (event.key.keysym.sym != SDLK_RETURN)
					{
						break;
					}
				case SDL_MOUSEBUTTONDOWN:
					x = event.motion.x;
					y = event.motion.y;
					if ((x >= pos[3].x && x <= pos[3].x + pos[3].w && y >= pos[3].y &&y <= pos[3].y + pos[3].h) || ok)
					{
						for (int j = 0; j < length; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						//free(menus);
						free(pos);
						SDL_FreeSurface(screen);
						table S;
						strcpy(&(S.Name), labels[1]);
						S.Score = score;
						switch (getDiff())
						{
						case LAKO:
							addScore(S, E);
							break;
						case TESKO:
							addScore(S, H);
							break;
						case NORMALNO:
							addScore(S, N);
							break;
						}
						return 0;
					}
					break;

				}
				for (int i = 0; i < length; i++)
				{
					SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
				}
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
				SDL_DestroyTexture(texture);
				//SDL_FreeSurface(screen);
				time = SDL_GetTicks();
			}


			if (1000 / 30 > (SDL_GetTicks() - time))
			{
				SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
			}

		}
	}

	
int hscores(SDL_Renderer* renderer, TTF_Font* font, tezina diff)
	{

		int length = 11;
		static table* tabela = NULL;
		switch (diff)
		{
		case LAKO:
			tabela = readFile(E);
			break;
		case TESKO:
			tabela = readFile(H);
			break;
		case NORMALNO:
			tabela = readFile(N);
			break;
		}

		char labels[11][15];
		for (int i = 0; i < length - 1; i++)
		{
			strcpy(labels[i], &(tabela[i].Name));
			strcat(labels[i], " ");
			char br[10];
			_itoa(tabela[i].Score, br, 10);
			strcat(labels[i], br);
		}
		strcpy(labels[10], "Main Menu");

		SDL_Surface* screen;
		Uint32 rmask, gmask, bmask, amask;
		int width = 700;
		int height = 700;
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		}
		else
		{
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		}
		screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
		Uint32 time;
		int x, y;
		SDL_Surface* *menus = malloc(6 * sizeof(SDL_Surface*));
		int selected = 0;

		SDL_Color color[2] = { { 255,255,255 },{ 255,0,0 } };
		for (int i = 0; i < length; i++)
			menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);

		SDL_Rect *pos = malloc(length * sizeof(SDL_Rect));
		for (int i = 0; i < length; i++)
		{
			pos[i].x = 150 + 16 * 30 / 2 - 16 * 15;
			pos[i].y = 100 + 16 * 30 / 2 - 16 * 10 + i * 30;
			pos[i].w = 20;
			pos[i].h = 20;
		}
		pos[10].y += 150;


//		int setcl = 4;
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		SDL_Event event;
		
		while (1)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					for (int i = 0; i<length;i++)
						SDL_FreeSurface(menus[i]);
					//free(menus);
					SDL_FreeSurface(screen);
					free(selected);
					exit(0);
					if ((x >= pos[10].x) && (x <= pos[10].x + pos[10].w) && (y >= pos[10].y) && (y <= pos[10].y + pos[10].h))
					{
						if (!selected)
						{
							selected = 1;
							SDL_FreeSurface(menus[10]);
							menus[10] = TTF_RenderText_Solid(font, labels[10], color[1]);
						}

					}
					else
					{
						selected = 0;
						SDL_FreeSurface(menus[10]);
						menus[10] = TTF_RenderText_Solid(font, labels[10], color[0]);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.motion.x;
					y = event.motion.y;
					if (x >= pos[10].x && x <= pos[10].x + pos[10].w && y >= pos[10].y &&y <= pos[10].y + pos[10].h)
					{
						for (int j = 0; j < length; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						free(pos);
						//free(menus);
						free(selected);
						SDL_FreeSurface(screen);
						return 9;
						break;
					}
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (int j = 0; j < 6; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						free(pos);
						free(selected);
						SDL_FreeSurface(screen);
						//free(menus);
						return 0;
					}

				}
				for (int i = 0; i < length; i++)
				{
					SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
				}
				/*
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_DestroyTexture(texture);
				time = SDL_GetTicks();*/
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
				SDL_DestroyTexture(texture);
				//SDL_FreeSurface(screen);
				time = SDL_GetTicks();
				
			}


			if (1000 / 30 >(SDL_GetTicks() - time))
			{
				SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
			}

		}


	}

	int pSelected(SDL_Renderer* renderer, TTF_Font* font, tezina diff)
	{

		static char labels[4][15] = { "","Next", "Play", "Main Menu" };
		const char **imenaMatrica = ReadNames(), **trMap = imenaMatrica;
		int **map = NULL;
		strcpy(labels[0], imenaMatrica[0]);
		int poz = 0;
		int brojMapi = 0;


		while (*trMap)
		{
			brojMapi++;
			trMap++;
		}

		int length = 4;

		for (int i = 0; i < brojMapi; i++)
			imenaMatrica[i] = realloc(imenaMatrica[i], (strlen(imenaMatrica[i]) + 1) * sizeof(char));
		imenaMatrica = realloc(imenaMatrica, brojMapi * sizeof(char*));


		SDL_Surface* screen;
		Uint32 rmask, gmask, bmask, amask;
		int width = 700;
		int height = 700;

		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		}
		else
		{
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		}
		screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
		Uint32 time;
		int x, y;
		SDL_Surface* *menus = malloc(length * sizeof(SDL_Surface*));
		int *selected = malloc(length * sizeof(int));


		SDL_Color color[2] = { { 255,255,255 },{ 255,0,0 } };
		for (int i = 0; i < length; i++)
			menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);

		SDL_Rect *pos = malloc(length * sizeof(SDL_Rect));
		for (int i = 0; i < length; i++)
		{
			pos[i].x = 180;
			pos[i].y = 250 + i * 40;
			pos[i].w = 20;
			pos[i].h = 20;
		}
		pos[0].y -= 100;
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		SDL_Event event;

		while (1)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					for (int i = 0; i<length; i++)
						SDL_FreeSurface(menus[i]);
					//free(menus);
					return -1;
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (int i = 1; i < length; i++)
					{
						if ((x >= pos[i].x) && (x <= pos[i].x + pos[i].w) && (y >= pos[i].y) && (y <= pos[i].y + pos[i].h))
						{
							if (!selected[i])
							{
								selected[i] = 1;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
							}

						}
						else
						{
							selected[i] = 0;
							SDL_FreeSurface(menus[i]);
							menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.motion.x;
					y = event.motion.y;
					for (int i = 1; i < length; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							switch (i) {
							case 1:
							{
								SDL_FillRect(screen, &pos[0], SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
								SDL_FreeSurface(menus[0]);
								poz = (poz + 1) % brojMapi;
								strcpy(labels[0], imenaMatrica[poz]);
								menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
								break;
							}
							case 2:
							{
								map = ReadMap(imenaMatrica[poz]);
								for (int j = 0; j < length; j++)
								{
									SDL_FreeSurface(menus[j]);

								}
								//free(menus);
								free(pos);
								free(selected);
								for (int j = 0; j < brojMapi; j++)
								{
									free(imenaMatrica[j]);
								}
								free(imenaMatrica);
								return map;
								break;
							}
							case 3:
							{
								for (int j = 0; j < 4; j++)
								{
									SDL_FreeSurface(menus[i]);
								}
								free(selected);
								//SDL_FreeSurface(screen);
								free(pos);
								for (int j = 0; j < brojMapi; j++)
								{
									free(imenaMatrica[j]);
								}
								free(imenaMatrica);
								return NULL;
								break;
							}
							}
						}

					}
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (int j = 0; j < length; j++)
						{
							SDL_FreeSurface(menus[j]);
						}
						free(selected);
						free(pos);
						return 0;
					}

				}

				for (int i = 0; i < length; i++)
				{
					SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
				}
				/*
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_DestroyTexture(texture);
				time = SDL_GetTicks();*/
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
				SDL_DestroyTexture(texture);
				//SDL_FreeSurface(screen);
				time = SDL_GetTicks();

			}


			if (1000 / 30 >(SDL_GetTicks() - time))
			{
				SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
			}

		}


	}

