/*
Header za koriscenje botova u igrici Bomberman.
Datum: 11.05.2018.
Autor: Martin Mitrovic
*/
#pragma once

//!!!!!!! potrebno ucitati time.h i odraditi srand da bi botovi pravilno funkcionisali

// definicije karakteristicnih tipova
typedef enum stanje { SEARCH, SET_BOMB, RUN_AWAY } stanje;
typedef enum karakter { ZELENKO, ZUCA, CRVENI, IGRAC } karakter;
typedef enum tezina { LAKO, NORMALNO, TESKO } tezina;


typedef struct player
{
	int x, y; //pozicije igraca
	int destX, destY; //dodatne promenljive za pozicije (nekad je tu meta, a nekad predhodni korak)
	stanje aktivnost; //trenutna aktivnost igraca
	karakter tip; 
	char ziv; 
	int brojBombi;
}player;


//fje dostupne drugima na koriscenje
player *initAi(int, int); //inicijalizacija AI-a,parametri (sirinaMape, duzinaMape,brBotova,demoVerzija(0/1))
int setDiff(tezina); // postavlja tezinu
tezina getDiff(); //vraca tezinu
void updateState(player*, stanje);
void executeState(int**, player*, player*); //pokrece botove




