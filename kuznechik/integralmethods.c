#include <stdlib.h>
#include "kuznechik.h"
#include "integral_funcs.h"


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

block* generate_integral_set(int place_of_A, unsigned char c) {
    if (place_of_A > 15) {
        place_of_A = 15;
    }
    if (place_of_A < 8) {
        place_of_A = 7 - place_of_A;
    } else {
        place_of_A = 15 - place_of_A + 8;
    }
    block* res = (block*)calloc(256, sizeof(block));
    unsigned char vector[16] = {0};
    for (int i = 0; i < 16; ++i) {
        vector[i] = c;
    }

    for (int i = 0; i < 0xff + 1; ++i){
        vector[place_of_A] = i;
        res[i] = to_block((*(unsigned long*)vector), *(unsigned long*)(vector + 8));
    }
    return  res;
}

// Here we count bytes int block like .left = a0|a1|...|a7 and
// .right = a8|a9|...|a14|a15
char determine_integral_property(int place, block* texts) {
    unsigned char set[256] = {0};
    if (place > 15) {
        place = 15;
    }
    for (int i = 0; i < 256; ++i) {
        if (place < 8) {
            set[i] = (unsigned char)(texts[i].left >> (8*(7 - place)));
        } else {
            set[i] = (unsigned char)(texts[i].right >> (8*(7 - (place - 8))));
        }
    }
    sort(set, 256);
    if (is_all(set, 256)) {
        return 'A';
    } else if (is_constant(set, 256)) {
        return 'C';
    } else if (is_zerosum(set, 256)) {
        return 'S';
    } else {

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
/*
 * mode 7 - LSX; 3 - SX; 1 - X; 4 - L; 2 - S
 */
void one_round_crypt(block* texts, int size, block key, int mode){
    for (int i = 0; i < size; ++i) {
        if (mode & 1) {
            texts[i] = key_add(key, texts[i]);
        }
        if (mode & 2) {
            texts[i] = substitution(texts[i], 0);
        }
        if (mode & 4) {
            texts[i] = L(texts[i]);
        }
    }
}
