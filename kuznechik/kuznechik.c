#include <stdio.h>
#include <stdlib.h>
#include "sbox.h"
#include "kuznechik.h"
#include "galois_arithmetics.h"

block key_add(block key, block text) {
    text.left ^= key.left;
    text.right ^= key.right;
    return text;
}

// is_inverse == 1 for inverse
block substitution(block text, int is_inverse) {

    const unsigned char* sb = s_box;
    if (is_inverse == 1) {
        sb = s_box_inv;
    }

    unsigned char* p = (unsigned char*)(&text.left);
    for (int i = 0; i < 8; ++i) {
        *p = sb[*p];
        p++;
    }
    p = (unsigned char*)(&text.right);
    for (int i = 0; i < 8; ++i) {
        *p = sb[*p];
        p++;
    }
    return text;
}

unsigned char l(block text) {
    return  gmul(148, (unsigned char)(text.left >> 56)) ^ gmul(32, text.left >> 48) ^ gmul(133, text.left >> 40) ^
    gmul(16, text.left >> 32) ^ gmul(194, text.left >> 24) ^ gmul(192, text.left >> 16) ^
    gmul(1, text.left >> 8) ^ gmul(251, text.left)  ^ gmul(1, text.right >> 56) ^
    gmul(192, text.right >> 48) ^ gmul(194, text.right >> 40) ^ gmul(16, text.right >> 32) ^
    gmul(133, text.right >> 24) ^ gmul(32, text.right >> 16) ^ gmul(148, text.right >> 8) ^ gmul(1, text.right);
}

block R(block text) {
    long l_res = l(text);
    text.right >>= 8;
    text.right += (text.left & 0xff) << 56;
    text.left >>= 8;
    text.left += l_res << 56;
    return text;
}

block L(block text) {
    for (int i = 0; i < 16; ++i) {
        text = R(text);
    }
    return text;
}

block R_rev(block text) {
    unsigned char a15 = (unsigned char)(text.left >> 56);
    text.left <<= 8;
    text.left += text.right >> 56;
    text.right = (text.right << 8) | a15;
    unsigned  char l_res = l(text);
    text.right = (text.right & 0xffffffffffffff00) | l_res;
    return  text;
}

block L_rev(block text) {
    for (int i = 0; i < 16; ++i) {
        text = R_rev(text);
    }
    return text;
}

void F(block key, block* parts){
    block tmp = parts[0];
    parts[0] = key_add(parts[1],L(substitution(key_add(key,parts[0]),0)));
    parts[1] = tmp;
}

block encrypt(block text, block* keys, unsigned round_num) {
    for (int i = 0; i < round_num - 1; ++i) {
        text = L(substitution(key_add(keys[i], text), 0));
    }
    return  key_add(keys[round_num - 1], text);
}
// Keys must NOT be in the inverse order
block decrypt(block text, block* keys, unsigned round_num) {
    text = key_add(keys[round_num - 1], text);
    for (int i = round_num - 2; i >= 0; --i) {
        text = key_add(keys[i],substitution(L_rev(text), 1));
    }
}

block to_block(unsigned long left, unsigned long right) {
    block res = {.left = left, .right = right};
    return res;
}

block C (unsigned long i) {
    block res = {.left = 0, .right = i};
    return L(res);
}


block* generate_round_keys(block left_master, block right_master, unsigned rnum) {
    if (rnum < 2) {
        rnum = 2;
    }
    block* keys = (block*)calloc(rnum % 2 == 0 ? rnum : rnum + 1, sizeof(block));
    keys[0] = left_master;
    keys[1] = right_master;
    block* parts = (block*)calloc(2, sizeof(block));
    int ind = 0;
    int ri = 1;
    for (int cnt = rnum - 2; cnt > 0; cnt-=2) {
        parts[0] = keys[ind];
        parts[1] = keys[ind + 1];
        for (int i = 0; i < 8; ++i) {
            F(C(8*((unsigned long)ri - 1) + i + 1), parts);
        }
        ri++;
        ind += 2;
        keys[ind] = parts[0];
        keys[ind + 1] = parts[1];
    }
    return keys;
}



