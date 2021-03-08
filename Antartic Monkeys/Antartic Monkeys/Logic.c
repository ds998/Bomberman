#include "Logic.h"
#include "Score.h"

extern int **map;
extern int map_size;
extern bomb **bombs;
extern player *ar;
extern int player_number;
extern int **matricaSigurnosti;
extern score;

void explode(Uint32 inter, void *c);

void pauseTimer() {
	int i, j;
	int t;
	t = SDL_GetTicks();
	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			if (bombs[i][j].id !=  0) {
				if (bombs[i][j].own == 0) {
					bombs[i][j].timetoexp =  500 - (t - bombs[i][j].time);
					bombs[i][j].id = !SDL_RemoveTimer(bombs[i][j].id);
				}
				else {
					bombs[i][j].timetoexp = timetoexplode * 1000 - (t - bombs[i][j].time);
					bombs[i][j].id = !SDL_RemoveTimer(bombs[i][j].id);
					
				}
			}
		}
	}
}

void resumeTimer() {
	int i, j;
	int t;
	cord *tmp;
	t = SDL_GetTicks();
	for (i = 0; i < map_size; i++) {
		for (j = 0; j < map_size; j++) {
			if (bombs[i][j].timetoexp != 0) {
				bombs[i][j].time = t;
				if (bombs[i][j].own == 0) {//od vatre
					tmp = (cord*)malloc(sizeof(cord));
					tmp->x = i;
					tmp->y = j;
					bombs[i][j].id = SDL_AddTimer(bombs[i][j].timetoexp, firefrei, tmp);
					tmp = NULL;
				}
				else {
					tmp = (cord*)malloc(sizeof(cord));
					tmp->x = i;
					tmp->y = j;
					bombs[i][j].id = SDL_AddTimer(bombs[i][j].timetoexp, explode, tmp);
					tmp = NULL;
				}
			}
		}
	}
}


void murder(player *ar, int n, int x, int y, player *igrac) {
	int i;
	for (i = 0; i < n; i++)
		if (((ar[i].x == x) && (ar[i].y == y)) && (ar[i].ziv == 1))
		{
			if (igrac->tip == IGRAC)
				score += reward(kil);
			ar[i].ziv = 0;
		}
}


int check_field(int **TMP, int x, int y, int tip) {
	if ((map[x][y] == 0) || (map[x][y] == 2) || (map[x][y] == 3) || (map[x][y] == 8)) {
		TMP[x][y] += tip;
		if (TMP[x][y] < 0)
			TMP[x][y] = 0;
	}
	static int a[9] = { 0,1,0,0,1,1,1,1,0 };
	return a[map[x][y]];
	
}
void safety(int **TMP, int x, int y,int tip) { //tip = +-1
	int i, j;
	int up = 1, down = 1, left = 1, right = 1;
	check_field(TMP, x , y, tip);
	for (i = 1; i < bombs[x][y].reach; i++) {
		if (down && ((x + i) < map_size)) {
			if (check_field(TMP,(x + i), y,tip)) down = 0;
		}

		if (up && ((x - i) > -1)) {
			if (check_field(TMP,(x - i), y, tip)) up = 0;
		}

		if (right && ((y + i) < map_size)) {
			if (check_field(TMP, x, (y + i), tip)) right = 0;
		}

		if (left && ((y - i) > -1)) {
			if (check_field(TMP ,x, (y - i), tip)) left = 0;
		}
	}
}



int destroy(int x, int y, player* igrac) {
	switch (map[x][y]) {
	case 0:
		murder(ar, player_number, x, y,igrac);
		map[x][y] = 8;
		return 0;
	case 1:
		map[x][y] = 2;
		if (igrac->tip == IGRAC)
			score += reward(dest);
		return 1;
	case 2:
		return 0;
	case 3:
		cord * tmp;
		tmp = (cord*)malloc(sizeof(cord));
		tmp->x = x;
		tmp->y = y;
		explode(10 ,tmp);
		break;
	case 5:
		return 1;
	case 4:
		map[x][y] = 0;
		if (igrac->tip == IGRAC)
			score += reward(dest);
		return 1;
	case 8:
		return 0;
	}
}

void firefrei(Uint32 inter, void *c) {
	if (!ar)
		return;
	int i;
	int x = ((cord*)c)->x;
	int y = ((cord*)c)->y;
	bombs[x][y].time = 0;
	for (i = 0; i < bombs[x][y].reach; i++) {
		if (((x + i) < map_size)) {
			if (map[x + i][y] == 8) map[x + i][y] = 0;
		}

		if (((x - i) > -1)) {
			if (map[x - i][y] == 8) map[x - i][y] = 0;
		}

		if (((y + i) < map_size)) {
			if (map[x][y + i] == 8) map[x][y + i] = 0;
		}

		if (((y - i) > -1)) {
			if (map[x][y - i] == 8) map[x][y - i] = 0;
		}
	}
	bombs[x][y].id = !SDL_RemoveTimer(bombs[x][y].id);
	bombs[x][y].reach = 0;
	if (bombs[x][y].id) exit(101);
	free(c);
}

void explode(Uint32 inter, void *c) {
	if (!ar)
		return;
	int x = ((cord*)c)->x;
	int y = ((cord*)c)->y;
	bombs[x][y].id = !SDL_RemoveTimer(bombs[x][y].id);
	bombs[x][y].time = SDL_GetTicks();
	bombs[x][y].timetoexp = 0;
	if (getDiff()==TESKO)
		safety(matricaSigurnosti, x, y,-1);
	int i, j;
	map[x][y] = 0;
	int up = 1, down = 1, left = 1, right = 1;
	for (i = 0; i < bombs[x][y].reach; i++) {
		if (down && ((x + i) < map_size)) {
			if (destroy((x + i), y, bombs[x][y].own)) down = 0;
		}

		if (up && ((x - i) > -1)) {
			if (destroy((x - i), y, bombs[x][y].own)) up = 0;
		}

		if (right && ((y + i) < map_size)) {
			if (destroy(x, (y + i), bombs[x][y].own)) right = 0;
		}

		if (left && ((y - i) > -1)) {
			if (destroy(x, (y - i), bombs[x][y].own)) left = 0;
		}
	}

	bombs[x][y].own->brojBombi++;
	bombs[x][y].own->destX = -2;
	bombs[x][y].own->destY = -2;
	updateState(bombs[x][y].own, SEARCH);
	bombs[x][y].own = 0;
	bombs[x][y].id = SDL_AddTimer(500, firefrei, c);
}

void plant_bomb(int x, int y, int reach, player* owner) {
	cord *tmp;
	if (map[x][y] == 3) return;
	if (owner->brojBombi > 0) {
		owner->brojBombi--;
		tmp = (cord*)malloc(sizeof(cord));
		tmp->x = x;
		tmp->y = y;
		bombs[x][y].time = SDL_GetTicks();
		bombs[x][y].reach = reach;
		bombs[x][y].own = owner;
		bombs[x][y].id = SDL_AddTimer(timetoexplode * 1000, explode, tmp);
		if (bombs[x][y].id == 0) exit(123);
		map[x][y] = 3;
	}
}


int accessible(int block) {
	static int a[9] = { 1,0,1,0,0,0,0,0,0 };
	return a[block];
}

void move(player *p, char dir) {
	enum orijent { L,R,U,D };
	enum orijent a = dir;
	switch (a) {
	case U:
		if (((p->x - 1) > -1) && (accessible(map[p->x - 1][p->y]))) {
			p->x = p->x - 1;
		}
		break;
	case D:
		if (((p->x + 1) < map_size) && (accessible(map[p->x + 1][p->y]))) {
			p->x = p->x + 1;
		}
		break;
	case L:
		if (((p->y - 1) > -1) && (accessible(map[p->x][p->y - 1]))) {
			p->y = p->y - 1;
		}
		break;
	case R:
		if (((p->y + 1) < map_size) && (accessible(map[p->x][p->y + 1]))) {
			p->y = p->y + 1;
		}
		break;
	}
}

int reachExit(player* p) {
	if (map[p->x][p->y] == 2)
	{
		score += reward(ext);
		return 1;
	}
	else return 0;
}
