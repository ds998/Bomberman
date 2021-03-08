#include "Score.h"
#define max_score 10

static void encrypt_aes(uint8_t *plaintext, int lenght)
{
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	struct AES_ctx ctx;

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, plaintext, lenght);

}

static void decrypt_aes(uint8_t *cyphertext, int lenght)
{
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	struct AES_ctx ctx;

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, cyphertext, lenght);

}

int reward(int act) {
	static int b[3] = { 50, 500, 300 };
	return b[act];
}

table *readFile(char mode[]) {
	FILE *fp;
	static table *t;
	uint8_t *n;
	t = calloc(max_score, sizeof(table));
	while (!t) t = calloc(max_score, sizeof(table));
	n = t;
	int i = 0;
	fp = fopen(mode, "r");
	if (fp) {
		while (fscanf(fp, "%u ", &n[i++]) == 1);
		decrypt_aes(n, max_score * sizeof(table));
		t = n;
		fclose(fp);
	}
	return t;
}

void addScore(table S, char mode[]) {
	table *SC;
	uint8_t *n;
	FILE *fp;
	int ins = -1, i;
	//Ucitaj skortabelu
	SC = readFile(mode);

	for (i = 0; (i < max_score); i++) {
		if (S.Score > SC[i].Score) {
			ins = i;
			break;
		}
	}
	if (ins == -1) {
		//free(SC);
		return;
	}
	for (i = max_score-1; i > ins; i--) {
		SC[i] = SC[i - 1];
	}
	SC[ins] = S;
	//ZAPAMTI SCOREBORD
	fp = fopen(mode, "w");
	encrypt_aes(SC, max_score * sizeof(table));
	i = 0;
	n = SC;
	while (i<(max_score*sizeof(table))) fprintf(fp, "%u ", n[i++]);
	fclose(fp);
	//free(SC);
	
}