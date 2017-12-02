#include <stdio.h>

#include "kuznechik.h"
#include "galois_arithmetics.h"

int main() {
    printf("Hello, World!\n");
    block text = {.left = 0x0e93691a0cfc6040, .right = 0x8b7b68f66b513c13};

    block subs_text = L_rev(L(text));
    printf("%#016lx\t%#016lx\n", subs_text.left, subs_text.right);
    printf("%d\n", gmul(1, 1) ^ gmul(148, 0x94));

    block* pp = generate_round_keys(to_block(0x8899aabbccddeeff,0x0011223344556677),
                        to_block(0xfedcba9876543210, 0x0123456789abcdef), 10);
    printf("%lx\n", pp[9].left);
    return 0;
}