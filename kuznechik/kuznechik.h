#ifndef KUZNECHIK_HEADERS_H
#define KUZNECHIK_HEADERS_H

typedef struct {
    unsigned long left;
    unsigned long right;
} block;

block key_add(block key, block text);
block substitution(block text, int is_inverse);
unsigned char l(block text);
block R(block text);
block L(block text);
block R_rev(block text);
block L_rev(block text);

#endif //KUZNECHIK_HEADERS_H
