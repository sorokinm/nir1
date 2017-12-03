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
block encrypt(block text, block* keys, unsigned round_num);
block decrypt(block text, block* keys, unsigned round_num);
block get_random_key();
unsigned long lrand();

void F(block key, block* parts);
block* generate_round_keys(block left_master, block right_master, unsigned rnum);
block to_block(unsigned long left, unsigned long right);
void print_block(block text);

#endif //KUZNECHIK_HEADERS_H
