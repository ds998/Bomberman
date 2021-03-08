#pragma once
#include "AI.h"
#include "SDL.h"

#define timetoexplode 3 
typedef struct bomb bomb;
struct bomb {
	int reach;
	player *own;
	SDL_TimerID id;
	int time;
	int timetoexp;
};

typedef struct cord {
	int x;
	int y;
} cord;

void explode(Uint32,cord*);//Ova funkcija se poziva nakon tajmera koj je postavio plant bomb. Onistava polja i ubija igrace pomocu funkcija murder i destroy. Umesto praznih polja postavlja vatru. Vatra se naknadno brise tajmerom koji je postavila ova funkcija, a tajmer poziva firefrei.
void murder(player *ar, int n, int x, int y,player*);//Ova funkcija ubija igraca polju x i y i dodaje poene ukoliko je bota ubio igrac.
int destroy(int, int,int,player*);//Ovu funkciju poziva funkcija explode i ona vrsi promene nad kordinatama matricom u zavisnosti od trenute vrednosti polja u matrici.
void plant_bomb(int x, int y, int reach, player* owner);//Funkcija za postavljanje bombi. Kao ulaz prima kordinate i domet bombe kao i pokazivac na igraca koji je bombu postavio. U funkciji se postavlja tajmer koj kasnije poziva explode funkciju, a u matrici se obelezava bomba.
int accessible(int block);//Ova funkcija proverava da li igrac moze da stane na polje.
void move(player *p, char dir);//Za ulaz ima pokazivac igraca koji se pomera, kao i smer u kome se krece. Ova funkcija vodi racuna o kolizijama igraca sa blokovima
int reachExit(player* p);//Proverava da li je igrac dosao do izlaza.
void safety(int**, int, int,int);//Ova funkcija poponjava matricu sigurnosti.
int check_field(int**, int, int,int);//Ovu funkciju poziva funkcija safety i ona odredjuje da li je dato polje bezbedno.
void resumeTimer(void);//Ova funkcija nastavlja tajmere za bombe i vatru.
void pauseTimer(void);//Ova funkcija zaustavlja tajmere za bombe i vatru i cuva preostalo vreme.
void firefrei(Uint32 inter, void *c);//Ova funkcija brise vatrena polja i postavlja prazna. Poziva se tajmerom nakon explode.