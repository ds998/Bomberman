#include <SDL.h>
#include <stdlib.h>
#include "MapCreation.h"
#include "AI.h"
#include <stdio.h>


typedef struct bomb bomb;
struct bomb {
	int reach;
	int own;
	SDL_TimerID id;
};

typedef struct cord {
	int x;
	int y;
} cord;

int **map;
int map_size;
bomb **bombs;
player *ar;
int player_number;

void explode(cord *c);

void murder(player *ar, int n, int y, int x) {
	int i;
	for (i = 0; i < n; i++)
		if ((ar[i].x == x) && (ar[i].y == y) && (ar[i].ziv == 1)) ar[i].ziv = 0;
}

int destroy(int x, int y) {
	cord tmp;
	switch(map[y][x]) {
		case 0:
			murder(ar, player_number, y, x);
			return 0;
		case 1:
			map[y][x] = 2;
			return 1;
		case 2:
			return 0;
		case 3:
			tmp.x = x;
			tmp.y = y;
			explode(&tmp);
			break;
		case 4:
			return 1;
		case 5:
			map[y][x] = 0;
			return 1;
	}
}

void explode(cord *c) {
	int x = c->x;
	int y = c->y;
	int i,j;
	map[y][x] = 0;
	int up = 1, down = 1, left = 1, right = 1;
	for (i = 0; i < bombs[y][x].reach; i++) {
		if (right && ((x + i) < map_size)) {
			if (destroy((x + i), y)) right = 0;
		}

		if (left && ((x - i) > -1)) {
			if (destroy((x - i), y)) left = 0;
		}

		if (down && ((y + i) < map_size)) {
			if (destroy(x, (y + i))) down = 0;
		}

		if (up && ((y - i) > -1)) {
			if (destroy(x, (y - i))) up = 0;
		}
	}
	bombs[y][x].reach = 0;
	bombs[y][x].own = 0;
	bombs[y][x].id = SDL_RemoveTimer(bombs[y][x].id);
	if (bombs[y][x].id) exit(101);
}

void plant_bomb(int y, int x, int reach, int owner) {
	cord tmp;
	tmp.x = x;
	tmp.y = y;
	bombs[y][x].reach = reach;
	bombs[y][x].own = owner;
	bombs[y][x].id = SDL_AddTimer(30 * 1000, explode, &tmp);
	map[y][x] = 3;
}


int accessible(int block) {
	static int a[8] = { 1,0,1,0,0,0 };
	return a[block];
}

void move(player *p, char dir) {
	switch (dir) {
	case 'L':
		if ((((*p).x - 1) > -1) && (accessible(map[(*p).y][(*p).x - 1]))) {
			(*p).x = (*p).x - 1;
		}
		break;
	case 'R':
		if ((((*p).x + 1) < map_size) && (accessible(map[(*p).y][(*p).x + 1]))) {
			(*p).x = (*p).x + 1;
		}
		break;
	case 'U':
		if ((((*p).y - 1) > -1) && (accessible(map[(*p).y - 1][(*p).x]))) {
			(*p).y = (*p).y - 1;
		}
		break;
	case 'D':
		if ((((*p).y + 1) < map_size) && (accessible(map[(*p).y + 1][(*p).x]))) {
			(*p).y = (*p).y + 1;
		}
		break;
	}
}

int reachExit(player p) {
	if (map[p.y][p.x] == 2) return 1;
	else return 0;
}

int game(char dif, char gmap, char f[] ) {
	player pl;
	bomb *bomb_list;
	bomb_list = NULL;
	int reach = 4;
	int i, j;
	char mv;
	switch (gmap) {
		case 'S':
			map = (int**)RandMap('S');
			map_size = 8;
			break;
		case 'M':
			map = (int**)RandMap('M');
			map_size = 14;
			break;
		case 'L':
			map = (int**)RandMap('L');
			map_size = 20;
			break;
		case 'P':
			map = (int**)ReadMap(f);
			break;
		default:
			map = (int**)RandMap('M');
			map_size = 14;
			break;

	}
	pl.brojBombi = 3;
	pl.x = 0;
	pl.y = 0;
	while (1) {
		scanf("%c", &mv);
		move(&pl, mv);
		for (int i = 0; i < map_size; i++) {
			for (int j = 0; j < map_size; j++)
				printf("%d ", map[i][j]);
			printf("\n");
		}
		printf("%d %d\n", pl.x, pl.y);
	}
}

int main() {
	game('E', 'S', ' s');

	return 0;
}