#include <stdio.h>
#include "sbox.h"
#include "kuznechik.h"
#include "galois_arithmetics.h"

block key_add(block key, block text) {
    text.left ^= key.left;
    text.right ^= key.right;
    return text;
}

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

