#include <windows.h>
#include "mask.h"

char * unmask_rc4(RESOURCE * src, RESOURCE * key) {
	char * dst;

	dst = KERNEL32$VirtualAlloc( NULL, src->length, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
	// dprintf("ALLOC %p (%d bytes)", dst, src->length);

	unsigned char S[256];
	for (int i = 0; i < 256; i++)
		S[i] = i;

	unsigned char j = 0;
	for (int i = 0; i < 256; i++) {
		j = j + S[i] + (unsigned char)key->value[i % key->length];
		unsigned char tmp = S[i];
		S[i] = S[j];
		S[j] = tmp;
	}

	unsigned char si = 0, sj = 0;
	for (int x = 0; x < src->length; x++) {
		si++;
		sj += S[si];
		unsigned char tmp = S[si];
		S[si] = S[sj];
		S[sj] = tmp;
		dst[x] = src->value[x] ^ S[(unsigned char)(S[si] + S[sj])];
	}

	return dst;
}

char * unmask_xor(RESOURCE * src, RESOURCE * key) {
	char * dst;
	dst = KERNEL32$VirtualAlloc(NULL, src->length, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	for (int i = 0; i < src->length; i++) {
		dst[i] = src->value[i] ^ key->value[i % key->length];
	}
	return dst;
}