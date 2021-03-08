#include "Redovi.h"
#include <stdlib.h>

void insertPQueue(deoReda **red, deoReda *deo)
{
	deoReda *tr = *red, *pred = NULL;
	if (tr == NULL)
	{
		*red = deo;
		(*red)->sledeci = NULL;
	}
	else
	{
		while ((tr != NULL) && (deo->razdaljna>tr->razdaljna))
		{
			pred = tr;
			tr = tr->sledeci;
		}
		if (pred == NULL)
		{
			deo->sledeci = *red;
			*red = deo;
		}
		else
			if (tr == NULL)
			{
				pred->sledeci = deo;
				deo->sledeci = NULL;
			}
			else
			{
				deo->sledeci = tr;
				pred->sledeci = deo;
			}
	}
	return;
}

deoReda* deletePQueue(deoReda **red)
{
	deoReda *tr = *red;
	if (tr == NULL)
		return NULL;
	else
	{
		*red = tr->sledeci;
		tr->sledeci = NULL;
	}
	return tr;
}

void clearPQueue(deoReda *red)
{
	deoReda *tr = red, *pom;
	while (tr != NULL)
	{
		pom = tr->sledeci;
		tr->sledeci = NULL;
		free(tr);
		tr = pom;
	}
	return;
}

//fje za rad sa prioritetnim redom po razdaljini
void insertQueue(deoReda **red, deoReda *deo)
{
	deoReda *tr = *red;
	if (tr == NULL)
	{
		*red = deo;
		(*red)->sledeci = NULL;
	}
	else
	{
		while (tr->sledeci != NULL)
		{
			tr = tr->sledeci;
		}
		tr->sledeci = deo;
	}
	return;
}