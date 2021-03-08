#include "AI.h"
#include "Redovi.h"
#include <stdlib.h>
#include <stdio.h>

static int dimPolja;
int **matricaSigurnosti = NULL;

//extern funkcije 
	extern int Door(int**, int);
	extern plant_bomb(int x, int y, int reach, int owner);
	extern move(player *p, char dir);
	extern accessible(int);
	extern player *ar;
	extern int player_number;


static void postaviPolje(int x)
{
	dimPolja = x;
	return;
}

//podesavanje tezine
static tezina tezinaIgre = LAKO;

int setDiff(tezina diff)
{
	switch (diff)
	{
	case LAKO:
		tezinaIgre = LAKO;
		break;
	case NORMALNO:
		tezinaIgre = NORMALNO;
		break;
	case TESKO:
		tezinaIgre = TESKO;
		break;
	default:
		return 0;
	}
	return 1;
}
tezina getDiff()
{
	return tezinaIgre;
}

//deo za inicijalizaciju
//posto su sematski isti tipovi ovo ce da ih menja
static void zameniTip(player* igraci,int brIgraca) 
{
	int i;
	int prvi, drugi;
	for (i = 0; i < brIgraca; i++)
	{
		prvi = (1.*rand() / (RAND_MAX + 1))*brIgraca;
		drugi = (1.*rand() / (RAND_MAX + 1))*brIgraca;
		karakter tmp = igraci[prvi].tip;
		igraci[prvi].tip = igraci[drugi].tip;
		igraci[drugi].tip = tmp;
	}
	return;
}

//prihvata se samo broj botova deljiv sa 4 do 16 max za najvecu mapu
player *initAi(int xPolja, int brojIgraca)
{
	postaviPolje(xPolja);
	int k, l;
	if (getDiff() == TESKO)
	{
		matricaSigurnosti = malloc(dimPolja * sizeof(int*));
		for (k = 0; k < dimPolja; k++)
			matricaSigurnosti[k] = calloc(dimPolja,sizeof(int));
	}
	int i = 0;
	int xP = xPolja / 2 - 1, yP =xP;
	int x = xP, y = yP;
	player *igraci = calloc(brojIgraca, sizeof(player));
	for (i = 0; i < brojIgraca; i += 4)
	{
		igraci[i].x = xP - x;
		igraci[i].y = yP - y;
		igraci[i].tip = ZUCA;
		igraci[i].aktivnost = SEARCH;
		igraci[i].ziv = 1;
		igraci[i].brojBombi = 2;
		igraci[i].destX = -2; igraci[i].destY = -2;

		igraci[i + 1].x = xP - x;
		igraci[i + 1].y = yP + y + 1;
		igraci[i + 1].tip = ZUCA;
		igraci[i + 1].aktivnost = SEARCH;
		igraci[i + 1].ziv = 1;
		igraci[i + 1].brojBombi = 2;
		igraci[i + 1].destX = -2; igraci[i + 1].destY = -2;

		igraci[i + 2].x = xP + x + 1;
		igraci[i + 2].y = yP - y;
		igraci[i + 2].tip = CRVENI;
		igraci[i + 2].aktivnost = SEARCH;
		igraci[i + 2].ziv = 1;
		igraci[i + 2].brojBombi = 2;
		igraci[i + 2].destX = -2; igraci[i + 2].destY = -2;

		igraci[i + 3].x = xP + x + 1;
		igraci[i + 3].y = yP + y + 1;
		igraci[i + 3].tip = ZELENKO;
		igraci[i + 3].aktivnost = SEARCH;
		igraci[i + 3].ziv = 1;
		igraci[i + 3].brojBombi = 2;
		igraci[i + 3].destX = -2; igraci[i + 3].destY = -2;

		x -= x / 3 + 1;
		y -= y / 3 + 1;
	}
	zameniTip(igraci, brojIgraca);
	igraci->tip = IGRAC;
	return igraci;
}

//promena stanja bota
void updateState(player* igrac, stanje trStanje)
{
	//printf("Igrac sa kordinatama: %d %d menja stanje iz %i u stanje %i\n",igrac->x,igrac->y,igrac->aktivnost,trStanje);
	igrac->aktivnost = trStanje;	
	switch (trStanje)
	{
		case SEARCH:
			igrac->destX = igrac->x; igrac->destY = igrac->y;
			break;
		case RUN_AWAY:
			igrac->destX = -2; igrac->destY = -2;
			break;
	}
	return;
}

//provera da li je nesto unutar polja
static char uPolju( int xPoz, int yPoz)
{
	if ((xPoz < 0) || (xPoz >= dimPolja))
		return 0;
	if ((yPoz < 0) || (yPoz >= dimPolja))
		return 0;
	return 1;
}

//racuna razdaljinu izmedju dva polja
static int razdaljina(int x1, int y1, int x2, int y2)
{
	int razdaljina = abs(x1 - x2) + abs(y1 - y2);
	return razdaljina;
}
 
//pretrazuje okolinu u plus formatu
static char pretraziOkolinu(int** mat, player *igrac)
{
	int i, range = 5,j;
	int pom,d,max;
	if (tezinaIgre == LAKO)
		max = player_number;
	else max = 1;
	for (j = 0; j < max; j++)	// vide se na 4 bloka
	{
		if ((ar + j) == igrac || !(ar + j)->ziv)
			continue;
		pom = 0;
		if (razdaljina(igrac->x, igrac->y, (ar + j)->x, (ar + j)->y) > 4)
			continue;
		if ((igrac->x != (ar + j)->x) && (igrac->y != (ar + j)->y))
			continue;
		if (igrac->y == (ar + j)->y)
		{
			if (igrac->x > (ar + j)->x) d = -1;
			else d = 1;
			while (igrac->x + pom != (ar + j)->x)
			{
				if (!accessible(mat[igrac->x + pom][igrac->y]))
					break;
				pom += d;
			}
			if (igrac->x + pom == (ar + j)->x)
				return 1;
		}
		pom = 0;
		if (igrac->x == (ar + j)->x)
		{
			if (igrac->y > (ar + j)->y) d = -1;
			else d = 1;
			while (igrac->y + pom != (ar + j)->y)
			{
				if (!accessible(mat[igrac->y + pom][igrac->y]))
					break;
				pom += d;
			}
			if (igrac->y + pom == (ar + j)->y)
				return 1;
		}
	}
	return 0;
}

//ako se bomba postavi na trenutno mesto gde je najblize sigurno
static deoReda* findSafePathLiN(int** mat, player *igrac)
{
	int razdaljina = 0,i;
	int **pomMatrica = calloc(dimPolja, sizeof(int*));
	for (i = 0; i < dimPolja; i++)
		pomMatrica[i] = calloc(dimPolja, sizeof(int));
	int x = igrac->x, y = igrac->y;
	deoReda *pRed = NULL, *pom = NULL;
	pom = malloc(sizeof(deoReda));
	pom->x = x; pom->y = y;
	pom->sledeci = NULL;
	pom->razdaljna = razdaljina;
	insertQueue(&pRed, pom);
	while (((x == igrac->x) && (abs(y - igrac->y) < 4)) || ((y == igrac->y) && (abs(x - igrac->x) < 4)))
	{
		pom = deletePQueue(&pRed);
		if (pom == NULL) //nema sigurnog puta
			break;
		pomMatrica[pom->x][pom->y] = 1;
		x = pom->x; y = pom->y;
		razdaljina = pom->razdaljna + 1;
		free(pom);
		if ((uPolju(x - 1, y) && !pomMatrica[x - 1][y]) && ((accessible(mat[x - 1][y])) || mat[x-1][y] == 8))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x - 1; pom->y = y;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x + 1, y) && !pomMatrica[x + 1][y]) && (accessible(mat[x + 1][y]) || mat[x+1][y] == 8))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x + 1; pom->y = y;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x, y - 1) && !pomMatrica[x][y - 1]) && (accessible(mat[x][y-1]) || mat[x][y - 1] == 8))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x; pom->y = y - 1;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x, y + 1) && !pomMatrica[x][y + 1]) && (accessible(mat[x][y+1]) || mat[x][y + 1] == 8))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x; pom->y = y + 1;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
	}
	pom = malloc(sizeof(deoReda));
	pom->x = x; pom->y = y;
	pom->razdaljna = razdaljina-1;
	for (i = 0; i < dimPolja; i++)
		free(pomMatrica[i]);
	free(pomMatrica);
	clearPQueue(pRed);
	return pom;
}

static deoReda* findSafePathT(int** mat, player *igrac)
{
	int razdaljina = 0, i;
	int **pomMatrica = calloc(dimPolja, sizeof(int*));
	for (i = 0; i < dimPolja; i++)
		pomMatrica[i] = calloc(dimPolja, sizeof(int));
	int x = igrac->x, y = igrac->y;
	deoReda *pRed = NULL, *pom = NULL;
	pom = malloc(sizeof(deoReda));
	pom->x = x; pom->y = y;
	pom->sledeci = NULL;
	pom->razdaljna = razdaljina;
	insertQueue(&pRed, pom);
	while (matricaSigurnosti[x][y])
	{
		pom = deletePQueue(&pRed);
		if (pom == NULL) //nema sigurnog puta
			break;
		pomMatrica[pom->x][pom->y] = 1;
		x = pom->x; y = pom->y;
		razdaljina = pom->razdaljna + 1;
		free(pom);
		if ((uPolju(x - 1, y) && !pomMatrica[x - 1][y]) && accessible(mat[x - 1][y]))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x - 1; pom->y = y;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x + 1, y) && !pomMatrica[x + 1][y]) && accessible(mat[x + 1][y]))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x + 1; pom->y = y;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x, y - 1) && !pomMatrica[x][y - 1]) && accessible(mat[x][y - 1]))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x; pom->y = y - 1;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
		if ((uPolju(x, y + 1) && !pomMatrica[x][y + 1]) && accessible(mat[x][y + 1]))
		{
			pom = malloc(sizeof(deoReda));
			pom->x = x; pom->y = y + 1;
			pom->sledeci = NULL;
			pom->razdaljna = razdaljina;
			insertQueue(&pRed, pom);
		}
	} 
	pom = malloc(sizeof(deoReda));
	pom->x = x; pom->y = y;
	pom->razdaljna = razdaljina - 1;
	for (i = 0; i < dimPolja; i++)
		free(pomMatrica[i]);
	free(pomMatrica);
	clearPQueue(pRed);
	return pom;
}

static deoReda* findSafePath(int** mat, player *igrac)
{
	if (tezinaIgre == TESKO)
	{
		findSafePathT(mat,igrac);
	}
	else
	{
		findSafePathLiN(mat, igrac);
	}
}

//u ovom stanju bot postavlja bombu
static void setBomb(player *igrac)
{
	plant_bomb(igrac->x, igrac->y, 4, igrac);
	if (tezinaIgre == TESKO)
		safety(matricaSigurnosti, igrac->x, igrac->y, 1);
	updateState(igrac, RUN_AWAY);
	return;
}

// ako se igrac zakuca u lomljiv zid ili spazi drugog igraca
static int threat(int** polje, player *igrac)
{
	deoReda * b = NULL;
	switch (igrac->tip)
	{
	default:
	case ZELENKO:
		b = findSafePath(polje, igrac);
		if ((b->razdaljna < 3) && (polje[igrac->x][igrac->y] != 2))
		{
			updateState(igrac, SET_BOMB);
			return 1;
		}
		return 0;
	case CRVENI:
		if (polje[igrac->x][igrac->y] != 2)
		{
			updateState(igrac, SET_BOMB);
			return 1;
		}
		return 0;
		break;
	case ZUCA:
		if (((igrac - 1)->ziv) && (polje[igrac->x][igrac->y] != 2))
		{
			updateState(igrac, SET_BOMB);
			return 1;
		}
		else if (polje[igrac->x][igrac->y] != 2)
		{
			b = findSafePath(polje, igrac);
			if ((b->razdaljna < 4) && (polje[igrac->x][igrac->y] != 2))
			{
				updateState(igrac, SET_BOMB);
				return 1;
			}
		}
		else
			return 0;
		break;
	}
return 0 ;
}


//kretanja botova u fazi pretrazivanja mape
// LiN -- Lako i Normalno
// T -- tesko
static void searchLiN(int** mat, player* igrac)
{
	if (pretraziOkolinu(mat, igrac))
	{
		threat(mat,igrac);
		return;
	}
	char a = (1.0*rand() / RAND_MAX) * 4;
	move(igrac, a);
	switch (a)
	{
	case 2:
		if (uPolju(igrac->x - 1, igrac->y) && ((mat[igrac->x - 1][igrac->y] == 4) || (mat[igrac->x - 1][igrac->y] == 1)))
			threat(mat,igrac);
		break;
	case 3:
		if (uPolju(igrac->x + 1, igrac->y) && ((mat[igrac->x + 1][igrac->y] == 4) || (mat[igrac->x + 1][igrac->y] == 1)))
			threat(mat,igrac);
		break;
	case 0:
		if (uPolju(igrac->x, igrac->y-1) && ((mat[igrac->x][igrac->y - 1] == 4) || (mat[igrac->x][igrac->y-1] == 1 )))
			threat(mat,igrac);
		break;
	case 1:
		if (uPolju(igrac->x, igrac->y+1) && ((mat[igrac->x][igrac->y + 1] == 4) || (mat[igrac->x][igrac->y+1] == 1)))
			threat(mat,igrac);
		break;
	}
}

static void searchT(int** mat, player *igrac, int x, int y)//x i y su pozicije igraca
{
	if ((igrac->aktivnost == SEARCH) && (pretraziOkolinu(mat, igrac)))
	{
		threat(mat, igrac);
		return;
	}
	if ((igrac->x == x) && (igrac->y == y))
		return;
	deoReda *pRed = NULL, *pom = NULL;
	if ((uPolju(igrac->x - 1, igrac->y)) && (mat[igrac->x - 1][igrac->y] != 5) && (mat[igrac->x - 1][igrac->y] != 3) && (mat[igrac->x - 1][igrac->y] != 8))
	{
		pom = malloc(sizeof(deoReda));
		pom->x = igrac->x - 1;
		pom->y = igrac->y;
		pom->sledeci = NULL;
		pom->razdaljna = razdaljina(igrac->x - 1, igrac->y, x, y);
		insertPQueue(&pRed, pom);
	}
	if ((uPolju(igrac->x + 1, igrac->y)) && ((mat[igrac->x + 1][igrac->y] != 5) && (mat[igrac->x + 1][igrac->y] != 3) && (mat[igrac->x + 1][igrac->y] != 8)))
	{
		pom = malloc(sizeof(deoReda));
		pom->x = igrac->x + 1;
		pom->y = igrac->y;
		pom->sledeci = NULL;
		pom->razdaljna = razdaljina(igrac->x + 1, igrac->y, x, y);
		insertPQueue(&pRed, pom);
	}
	if ((uPolju(igrac->x, igrac->y - 1)) && ((mat[igrac->x][igrac->y - 1] != 5) && (mat[igrac->x][igrac->y-1] != 3) && (mat[igrac->x][igrac->y-1] != 8)))
	{
		pom = malloc(sizeof(deoReda));
		pom->x = igrac->x;
		pom->y = igrac->y - 1;
		pom->sledeci = NULL;
		pom->razdaljna = razdaljina(igrac->x, igrac->y - 1, x, y);
		insertPQueue(&pRed, pom);
	}
	if ((uPolju(igrac->x, igrac->y + 1)) && ((mat[igrac->x][igrac->y + 1] != 5) && (mat[igrac->x][igrac->y+1] != 3) && (mat[igrac->x][igrac->y+1] != 8)))
	{
		pom = malloc(sizeof(deoReda));
		pom->x = igrac->x;
		pom->y = igrac->y + 1;
		pom->sledeci = NULL;
		pom->razdaljna = razdaljina(igrac->x, igrac->y + 1, x, y);
		insertPQueue(&pRed, pom);
	}
	while (pRed != NULL)
	{
		pom = deletePQueue(&pRed);
		if ((pom->x != igrac->destX) || (pom->y != igrac->destY)) 
		{
			//ako udari u zid koji moze da srusi
			if (mat[pom->x][pom->y] == 4 || mat[pom->x][pom->y] == 1) 
			{
				if (igrac->aktivnost == SEARCH)
				{
					if (threat(mat, igrac))
					{
						free(pom);
						clearPQueue(pRed);
						break;
					}
					else
						continue;
				}
				else
					continue;
			}
			igrac->destX = igrac->x; igrac->destY = igrac->y;
			igrac->x = pom->x; igrac->y = pom->y;
			clearPQueue(pRed);
			pRed = NULL;
			free(pom);
		}
		else
		{
			if (pRed != NULL)
			{
				free(pom);
				continue;
			}
			igrac->destX = igrac->x; igrac->destY = igrac->y;
			igrac->x = pom->x; igrac->y = pom->y;
			free(pom);
		}
	}
	return;
}

//ova fja bira koja se fja za pretrazivanje koristi
static void search(int** mat, player *bot, player *igrac)
{
	if (bot->tip == IGRAC)
	{
		int vrata = Door(mat, dimPolja);
		searchT(mat, bot, vrata/dimPolja, vrata%dimPolja);
	}
	else
	{
		if (tezinaIgre == TESKO)
			searchT(mat, bot, igrac->x, igrac->y);
		else
			searchLiN(mat, bot);
	}
	return;
}

//faza bezanja od bombi
static void runAway(int** polje, player* igrac)
{
	if (pretraziOkolinu(polje, igrac))
		threat(polje, igrac);
	deoReda *pok=NULL;
	// -2 da nisu postavljenje -1 da nema
	if (igrac->destX == -1)//deluje problematicno ali nije jer ce taj koji dospe u ovu situaciju uskoro biti ubijen :D
	{
		updateState(igrac, SEARCH);
		return;
	}
    if ((tezinaIgre == TESKO) && (matricaSigurnosti[igrac->x][igrac->y]))
	{
		pok = findSafePath(polje, igrac);
		igrac->destX = pok->x;
		igrac->destY= pok->y;
		free(pok);
	}
	else if (igrac->destX == -2)
	{
		pok = findSafePath(polje, igrac);
		igrac->destX = pok->x;
		igrac->destY = pok->y;
		free(pok);
	}
	int xP = igrac->destX, yP = igrac->destY;
	igrac->destX = igrac->x;
	igrac->destY = igrac->y;
	searchT(polje, igrac, xP, yP);
	igrac->destX = xP;
	igrac->destY = yP;

	return;
}

//ovo se salje Ticmiju na koriscenje
void executeState(int** polje, player *bot, player *igrac)
{	
	if (bot->ziv)
	{
		switch (bot->aktivnost)
		{
		case SEARCH:
			search(polje,bot, igrac);
			break;
		case RUN_AWAY:
			runAway(polje, bot);
			break;
		case SET_BOMB:
			setBomb(bot);
			break;
		}
	}
	return;
}





