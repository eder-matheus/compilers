#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit(void) {
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		Table[i] = 0;
	}
}

int hashAddress(char *text) {
	int addr = 1;
	int i;
	for (i = 0; i < strlen(text); i++) {
		addr = (addr * text[i]) % HASH_SIZE + 1;
	}

	return addr - 1;
}

HASH_NODE *hashFind(char *text) {
	HASH_NODE *node;
	int addr = hashAddress(text);
	for (node = Table[addr]; node; node = node->next) {
		if (strcmp(node->text, text) == 0) {
			return node;
		}
	}
	return 0;
}

HASH_NODE *hashInsert(char *text, int type) {
	int addr = hashAddress(text);
	HASH_NODE *node = hashFind(text);

	// if element already existis in the table, return it
	if (node != 0) {
		return node;
	}

	node = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
	node->type = type;
	node->text = (char*) calloc (strlen(text)+1, sizeof(char));
	strcpy(node->text, text);

	// The new node will be the head of the linked list
	node->next = Table[addr];
	Table[addr] = node;
	return node;
}

void hashPrint(void) {
	int i;
	HASH_NODE *node;
	for (i = 0; i < HASH_SIZE; i++) {
		for (node = Table[i]; node; node = node->next) {
			printf("Table[%d] has %s\n", i, node->text);
		}
	}
}