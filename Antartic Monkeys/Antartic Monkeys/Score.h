#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "aes.h"

#define dest 0 
#define kil 1
#define ext 2

#define E "Easy.d"
#define N "Normal.d"
#define H "Hard.d"

typedef struct Table {
	int Score;
	char Name[16];
} table;

static void encrypt_aes(uint8_t *plaintext, int lenght);//Enkriptuje podatke zadate duzine.(Koristi isti kljuc.)
static void decrypt_aes(uint8_t *cyphertext, int lenght);//Dekriptuje podatke zadate duzine.(Koristi isti kljuc.)
int reward(int act);//Vraca nagradu u zavisnosti od akcije koju preduzme igrac
table *readFile(char mode[]);//Cita i dekriptuje fajl sa skorovima sa diska.
void addScore(table S, char mode[]);//Dodaje skor u zadatu tabelu.
