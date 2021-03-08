#pragma once

typedef struct dR //struktura za formiranje prioritetnog reda
{
	int x, y;
	int razdaljna;
	struct dR* sledeci;
}deoReda;


void insertPQueue(deoReda**, deoReda*);
deoReda* deletePQueue(deoReda**);
void clearPQueue(deoReda*);
void insertQueue(deoReda**, deoReda*);