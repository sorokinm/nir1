#ifndef KUZNECHIK_INTEGRAL_FUNCS_H
#define KUZNECHIK_INTEGRAL_FUNCS_H
#include "kuznechik.h"

block* generate_integral_set(int place_of_A, unsigned char c);
char determine_integral_property(int place, block* texts);
void sort(unsigned char* arr, int length);
int is_zerosum(unsigned char* set, int size);
int is_all(unsigned char* set, int size);
int is_constant(unsigned char* set, int size);
void one_round_crypt(block* texts, int size, block key, int mode);

#endif //KUZNECHIK_INTEGRAL_FUNCS_H
