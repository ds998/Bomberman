#include "MapCreation.h"
int Forbidden(int i, int j, int dim) {
	int f = 0;
	if (i == 0 && (j == 0||j==1||j==dim-2||j==dim-1))
		f = 1;
	if (!f&&i == 1 && (j == 0 || j == dim - 1))
		f = 1;
	if (!f&&i == dim - 2 && (j == 0 || j == dim - 1))
		f = 1;
	if (!f&&i == dim - 1 && (j == 0 || j == 1 || j == dim - 2 || j == dim - 1))
		f = 1;
	return f;
}
void SaveMap(const char * f, int dim, int**map) {
	FILE* fp;
	int i, j,coef;
	fp = fopen(f, "w");
	//printf("%c", dim);
	fprintf(fp, "%c", dim);
	coef = rand()%50;
	//printf("%d",coef);
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++)
			fprintf(fp, "%c", (coef=(map[i][j]+coef)%156+100));
	}
	if(strcmp(f,"randmapa.txt"))
		SFileNames(f);
	fclose(fp);
}
int ** ReadMap(const char * f) {
	FILE* fp;
	int i, j, coef=0,**map;
	fp = fopen(f, "r");
	int dim = fgetc(fp);
	//printf("%d", dim);
	map = malloc(dim * sizeof(int*));
	while (!map)
		map = malloc(dim * sizeof(int*));
	for (i = 0; i < dim; i++) {
		map[i] = calloc(dim, sizeof(int));
		while (!map[i])
			map[i] = calloc(dim, sizeof(int));

	}
	//printf("\n");
		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				unsigned char p = fgetc(fp);
				if(i==0&&j==0)
					coef = (p -100 < 0 ? p -100 +256 : p -100);
				/*printf("%d ", */p = p - coef >= 100 ? p - coef - 100 : p - coef + 156 - 100;
				map[i][j] = p;
				coef = (coef+p)%156+100;
			}
			//printf("\n");
		}
	fclose(fp);
	map[0][0] = dim;
	return map;
}
int** RandMap(char size) {
	int dim,i,j;
	int**map;
	switch (size) {
		case 'S': {
			dim = Smap;
			break;
		}
		case 'L': {
			dim = Lmap;
			break;
		}
		default: {
			dim = Mmap;
			break;
		}
	}
	do {
		srand(time(0));
		map = malloc(dim * sizeof(int*));
		while(!map)
			map = malloc(dim * sizeof(int*));
		for (i = 0; i < dim; i++) {
			map[i] = calloc(dim, sizeof(int));
			while(!map[i])
				map[i] = calloc(dim, sizeof(int));
		}
		
		i = rand() % dim;
		j = rand() % dim;
		while (Forbidden(i, j, dim)) {
			i = rand() % dim;
			j = rand() % dim;
		}
		map[i][j] = 1;
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				if (!Forbidden(i, j, dim) && map[i][j] == 0) {
					int p = rand() % 6;
					map[i][j] = p <= 3 ? (!p ? 0 : 4) : p;
				}

			}
		/*for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++)
				printf("%d ", map[i][j]);
			printf("\n");
		}*/
		char* file = "randmapa.txt";
		SaveMap(file, dim, map);
	} while (!Possible(map, dim, 0, 0));

	//ReadMap(file);
	return map;
}
int Possible(int**map,int size,int ip,int jp) {
	int*reachable;
	reachable = malloc(400 * sizeof(int));
	while(!reachable)
		reachable = malloc(400 * sizeof(int));
	int br=1, curr=0,move,pom,k,f;
	reachable[0] = ip * size + jp;
	while (br > curr) {
		int j = reachable[curr] % size;
		int i = reachable[curr] / size;
		if (i + 1 < size&&map[i + 1][j] <5) {
			pom = (i + 1)*size + j;
			f = 1;
			for (k = 0; k < br; k++)
				if (reachable[k] == pom) {
					f = 0;
					break;
				}
			if (f) {
				reachable[br++] = pom;
			}
		}
		if (i - 1 >= 0&&map[i - 1][j] <5) {
			pom = (i - 1)*size + j;
			f = 1;
			for (k = 0; k < br; k++)
				if (reachable[k]== pom) {
					f = 0;
					break;
				}
			if (f) {
				reachable[br++] = pom;
			}
		}
		if (j + 1 < size&&map[i][j+1] <5) {
			pom = (i)*size + j+1;
			f = 1;
			for (k = 0; k < br; k++)
				if (reachable[k]== pom) {
					f = 0;
					break;
				}
			if (f) {
				reachable[br++] = pom;
			}
		}
		if (j - 1 >=0&&map[i][j-1] <5) {
			pom = (i)*size + j-1;
			f = 1;
			for (k = 0; k < br; k++)
				if (reachable[k]== pom) {
					f = 0;
					break;
				}
			if (f) {
				reachable[br++] = pom;
			}
		}
		curr++;
	}
	f = 0;
	for (int i = 0; i < br; i++) {
		//printf("%d%d\t", reachable[i] / size, reachable[i] % size);
		if ((map[reachable[i] / size][reachable[i] % size] == 1) || (map[reachable[i] / size][reachable[i] % size] == 2) ){
			f = 1;
			break;
		}
	}
	/*if (f)
		printf("Moguce je stici do cilja\n");
	else
		printf("Nije moguce stici do cilja\n");*/
	return f;
}
int **CustomMap(char size, char*name) {
	int dim,i,j;
	switch (size) {
		case 'S': {
			dim = Smap;
			break;
		}
		case 'L': {
			dim = Lmap;
			break;
		}
		default: {
			dim = Mmap;
			break;
		}
	}
	int** map = malloc(dim * sizeof(int*));
	while(!map)
		map = malloc(dim * sizeof(int*));

	do {
		printf("\n");
		for (i = 0; i < dim; i++) {
			map[i] = calloc(dim, sizeof(int));
			while(!map[i])
				map[i] = calloc(dim, sizeof(int));
		}

		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++)
				scanf("%d", &map[i][j]);
	} while (!Possible(map, dim, 0, 0));
	SaveMap(name, dim, map);
	ReadMap(name);
	return map;
}
int **DelMap(int**map,int size){
	for (int i = 0; i < size; i++) {
		free(map[i]);
	}
	free(map);
	return map;
}
int **Clear(int**map,int size,int i,int j) {
	if (i >= size || j >= size)
		return map;
	if (map[i][j] == 1) {
		if (size - i != 1)
			i = i + 1;
		else
			i = i - 1;
	}
	map[i][j] = 0;
	if (map[i + 1][j] != 1)
		map[i + 1][j] = 0;
	if (map[i - 1][j] != 1)
		map[i - 1][j] = 0;
	if (map[i][j+1] != 1)
		map[i][j+1] = 0;
	if (map[i][j - 1] != 1)
		map[i][j - 1] = 0;
	return map;
}
int PossEdit(int**map,int size) {
	int i, j, f=0;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if ((map[i][j] == 1)|| (map[i][j] == 2)) {
				if (f == 0)
					f = 1;
				else
					return 0;
			}
	return f;
}
int **EditMap(int **map, int size, int i, int j,int br) {
	if (i >= size || j >= size||br!=0||br!=1||br!=4||br!=5)
		return map;
	if (!Forbidden(i, j, size))
		map[i][j] = br;
	return map;
}
int Door(int**map,int dim) {
	int door=0,i,j;
	for(i=0;i<dim;i++)
		for (j = 0; j < dim; j++) 
			if (map[i][j] == 1||map[i][j]==2)
				door = i * dim + j;
	return door;
}
void SFileNames(char* name) {
	FILE*fp;
	fp = fopen("names.txt", "a");
	fprintf(fp, "%s\n", name);
	fclose(fp);
}
char ** ReadNames() {
	FILE*fp = fopen("names.txt", "r");
	char**names,c;
	int i,j;
	names = calloc(100 , sizeof(char*));
	while(!names)
		names = calloc(100, sizeof(char*));
	i = 0;
	names[i] = malloc(30);
	while(!names[i])
		names[i] = malloc(30);
	fscanf(fp, "%c", &c);
	j = 0;
	while (c != '\n'&&c!=EOF) {
		names[i][j++] = c;
		fscanf(fp, "%c", &c);
	}
	names[i++][j] = 0;

	while (!feof(fp)) {
		names[i] = malloc(30);
		while (!names[i])
			names[i] = malloc(30);
		fscanf(fp, "%c", &c);
		j = 0;
		while (c != '\n'&&c != EOF) {
			names[i][j++] = c;
			fscanf(fp, "%c", &c);
		}
		names[i++][j] = 0;
	}
	names[i-1] = 0;
	return names;
}
