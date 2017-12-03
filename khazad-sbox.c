#include <stdio.h>
#include <stdlib.h>


unsigned gmul(unsigned a, unsigned b);
unsigned gadd(unsigned a, unsigned b);

static const unsigned char khazad_sbox_table[256u] =
{
 0xBA, 0x54, 0x2F, 0x74, 0x53, 0xD3, 0xD2, 0x4D, 0x50, 0xAC, 0x8D, 0xBF, 0x70, 0x52, 0x9A, 0x4C,
 0xEA, 0xD5, 0x97, 0xD1, 0x33, 0x51, 0x5B, 0xA6, 0xDE, 0x48, 0xA8, 0x99, 0xDB, 0x32, 0xB7, 0xFC,
 0xE3, 0x9E, 0x91, 0x9B, 0xE2, 0xBB, 0x41, 0x6E, 0xA5, 0xCB, 0x6B, 0x95, 0xA1, 0xF3, 0xB1, 0x02,
 0xCC, 0xC4, 0x1D, 0x14, 0xC3, 0x63, 0xDA, 0x5D, 0x5F, 0xDC, 0x7D, 0xCD, 0x7F, 0x5A, 0x6C, 0x5C,
 0xF7, 0x26, 0xFF, 0xED, 0xE8, 0x9D, 0x6F, 0x8E, 0x19, 0xA0, 0xF0, 0x89, 0x0F, 0x07, 0xAF, 0xFB,
 0x08, 0x15, 0x0D, 0x04, 0x01, 0x64, 0xDF, 0x76, 0x79, 0xDD, 0x3D, 0x16, 0x3F, 0x37, 0x6D, 0x38,
 0xB9, 0x73, 0xE9, 0x35, 0x55, 0x71, 0x7B, 0x8C, 0x72, 0x88, 0xF6, 0x2A, 0x3E, 0x5E, 0x27, 0x46,
 0x0C, 0x65, 0x68, 0x61, 0x03, 0xC1, 0x57, 0xD6, 0xD9, 0x58, 0xD8, 0x66, 0xD7, 0x3A, 0xC8, 0x3C,
 0xFA, 0x96, 0xA7, 0x98, 0xEC, 0xB8, 0xC7, 0xAE, 0x69, 0x4B, 0xAB, 0xA9, 0x67, 0x0A, 0x47, 0xF2,
 0xB5, 0x22, 0xE5, 0xEE, 0xBE, 0x2B, 0x81, 0x12, 0x83, 0x1B, 0x0E, 0x23, 0xF5, 0x45, 0x21, 0xCE,
 0x49, 0x2C, 0xF9, 0xE6, 0xB6, 0x28, 0x17, 0x82, 0x1A, 0x8B, 0xFE, 0x8A, 0x09, 0xC9, 0x87, 0x4E,
 0xE1, 0x2E, 0xE4, 0xE0, 0xEB, 0x90, 0xA4, 0x1E, 0x85, 0x60, 0x00, 0x25, 0xF4, 0xF1, 0x94, 0x0B,
 0xE7, 0x75, 0xEF, 0x34, 0x31, 0xD4, 0xD0, 0x86, 0x7E, 0xAD, 0xFD, 0x29, 0x30, 0x3B, 0x9F, 0xF8,
 0xC6, 0x13, 0x06, 0x05, 0xC5, 0x11, 0x77, 0x7C, 0x7A, 0x78, 0x36, 0x1C, 0x39, 0x59, 0x18, 0x56,
 0xB3, 0xB0, 0x24, 0x20, 0xB2, 0x92, 0xA3, 0xC0, 0x44, 0x62, 0x10, 0xB4, 0x84, 0x43, 0x93, 0xC2,
 0x4A, 0xBD, 0x8F, 0x2D, 0xBC, 0x9C, 0x6A, 0x40, 0xCF, 0xA2, 0x80, 0x4F, 0x1F, 0xCA, 0xAA, 0x42
};

const unsigned char matrix[8][8] = {
		0x1, 0x3, 0x4, 0x5, 0x6, 0x8, 0xB, 0x7,
		0x3, 0x1, 0x5, 0x4, 0x8, 0x6, 0x7, 0xB,
		0x4, 0x5, 0x1, 0x3, 0xB, 0x7, 0x6, 0x8,
		0x5, 0x4, 0x3, 0x1, 0x7, 0xB, 0x8, 0x6,
		0x6, 0x8, 0xB, 0x7, 0x1, 0x3, 0x4, 0x5,
		0x8, 0x6, 0x7, 0xB, 0x3, 0x1, 0x5, 0x4,
		0xB, 0x7, 0x6, 0x8, 0x4, 0x5, 0x1, 0x3,
		0x7, 0xB, 0x8, 0x6, 0x5, 0x4, 0x3, 0x1
};

unsigned long sbox(unsigned long a) {
	unsigned char text[8] = {0};
	unsigned long* p = text;
	*p = a;
	for(int j = 0; j < 8; ++j) {
		text[j] = khazad_sbox_table[text[j]];
	}
 	return *p;
}

void mult_matrix(unsigned char mult[8][8]) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			for (int k = 0; k < 8; ++k) {
				mult[i][j] = gadd(gmul(matrix[i][k], matrix[k][j]), mult[i][j]);
			}
		}
	}
}

// linear transform of khazad
unsigned long theta(unsigned long vector) {
	unsigned char text[8] = {0};
	unsigned char result[8] = {0};
	unsigned long* p = text;
	*p = vector;

	for (int i = 0; i < 8; ++i) { // column
		for (int j = 0; j < 8; ++j) { // row
			result[i] = gadd(gmul(matrix[j][i], text[7 - j]), result[i]); // 7 - j because inverse order of loading
		}
	}
	p = result;
	return *p;
}

unsigned long* generate_integral_set(int place_of_A, unsigned char c) {
	if (place_of_A > 7) {
		place_of_A = 7;
	}
	place_of_A = 7 - place_of_A;

	unsigned long* res = (unsigned long*)calloc(256, sizeof(long));
	unsigned char vector[8] = {0};
	for (int i = 0; i < 8; ++i) {
		vector[i] = c;
	}

	for (int i = 0; i < 0xff + 1; ++i){
		vector[place_of_A] = i;
		res[i] = *(unsigned long*)vector;
	}
	return  res;
}

void sort(unsigned char* arr, int length) {
	int is_swap = 0;
	for (int i = 0; i < length - 1; ++i) {
		is_swap = 0;
		for (int j = 0; j < length - i - 1; ++j) {
			if (arr[j] > arr[j + 1]){
				unsigned char tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
				is_swap = 1;
			}
		}
		if (is_swap == 0) {
			break;
		}
	}
}

char determine_integral_property(int place, unsigned long* texts) {
	unsigned char set[256] = {0};
	if (place > 7) {
		place = 7;
	}
	for (int i = 0; i < 256; ++i) {
		set[i] = (unsigned char)(texts[i] >> (8*(7 - place)));
	}
	sort(set, 256);
	if (is_all(set, 256)) {
		return 'A';
	} else if (is_constant(set, 256)) {
		return 'C';
	} else if (is_zerosum(set, 256)) {
		return 'S';
	} else {
		/*
		for (int i = 0 ; i < 256; ++i) {
			printf("%02x ", set[i]);
		}
		printf("\n");*/
		return 'U'; // unknown
	}

}

int is_constant(unsigned char* set, int size) {
	unsigned char l = set[0];
	for (int i = 1; i < size; ++i) {
		if (set[i] != l) {
			return 0; // false
		}
	}
	return 1; // true
}

int is_all(unsigned char* set, int size) {
	unsigned char prev = set[0];
	for (int i = 1; i < size; ++i) {
		if (set[i] != prev + 1) {
			return 0; // false
		}
		prev = set[i];
	}
	return 1; // true
}

int is_zerosum(unsigned char* set, int size) {
	unsigned char res = set[0];
	for (int i = 1; i < size; ++i) {
		res ^= set[i];
	}
	if (res == 0) {
		return 1; // true
	} else {
		return 0; //false
	}
}


void one_round_crypt(unsigned long* texts, int size, unsigned long key, int mode){
	for (int i = 0; i < size; ++i) {
		if (mode & 1) {
			texts[i] = key ^ texts[i];
		}
		if (mode & 2) {
			texts[i] = sbox(texts[i]);
		}
		if (mode & 4) {
			texts[i] = theta(texts[i]);
		}
	}
}




int main() {
	unsigned long key = 0;
	for (int place = 0; place < 7; ++place) {
		unsigned long *integr_set = generate_integral_set(place, 5);
		for (int round = 1; round <= 3; ++round) {
			one_round_crypt(integr_set, 256, key, 7);

			for (int i = 0; i < 16; ++i) {
				char c = determine_integral_property(i, integr_set);
				printf("%c ", c);
			}
			printf("\n");
		}
		printf("\n");
	}
}


/* Multiply two numbers in the GF(2^8) finite field defined
 * by the polynomial x^8 + x^4 + x^3 + x + 1 = 0
 * using the Russian Peasant Multiplication algorithm
 * (the other way being to do carry-less multiplication followed by a modular reduction)
 */
unsigned gmul(unsigned a, unsigned b) {
	unsigned  p = 0; /* the product of the multiplication */
	while (a && b) {
		if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
			p ^= a; /* since we're in GF(2^m), addition is an XOR */

		if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
			a = (a << 1) ^ 0x11d; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible */
		else
			a <<= 1; /* equivalent to a*2 */
		b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}

/* Add two numbers in a GF(2^8) finite field */
unsigned gadd(unsigned a, unsigned b) {
	return a ^ b;
}

