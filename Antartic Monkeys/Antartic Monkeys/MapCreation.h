#define Smap 8   //Dimenzije mapa
#define Mmap 12
#define Lmap 20
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void SaveMap(const char * f, int dim, int**map); //Cuva mapu dimenzija dim u fajlu zapisanom u stringu f
int Forbidden(int i, int j, int dim);//Vraca da li je dozvoljeno upisati vrednost na poziciju i j
int ** ReadMap(const char * f);//Cita mapu iz zapisanu u fajlu f
int** RandMap(char size);// Generise nasumicnu mapu
int **CustomMap(char size, char*name);//Igrac generise mapu  zadatih dimenzija
int Possible(int**map, int size, int ip, int jp);//Ispituje da li je moguce stici do cilja
int **DelMap(int**map, int size);//Brise mapu
int **Clear(int**map, int size, int i, int j);//Oslobadja mesto za igraca
int PossEdit(int**map, int size);//Proverava da li mapa koja je napravljena preko Map Editor-a ima tacno jedna vrata
int **EditMap(int **map, int size, int i, int j, int br);//Upisuje vrednost br u mapu na mesto i 
int Door(int**map, int dim);//Vraca lokaciju vrata u formatu i*dim + j
void SFileNames(char* name);//Upisuje ime novonastale mape u fajl names.txt
char ** ReadNames();//Vraca sva imena sacuvanih matrica

/* OZNAKE NA MAPI
0-Prazno polje
1-Neotkrivena vrata (ispod bloka)
2-Otkrivena vrata
3-Bomba
4-Unistivi blok
5-Neunistivi blok
6-Igrac
7-Protivnik
8 - eksplozija
*/