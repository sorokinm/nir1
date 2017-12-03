#include <stdio.h>

#include "kuznechik.h"
#include "galois_arithmetics.h"
#include "integral_funcs.h"

int main() {

    //block text = {.left = 0x1122334455667700, .right = 0xffeeddccbbaa9988};

    //print_block(decrypt(encrypt(text,round_keys,10), round_keys, 10));
    //return 0;

    block* round_keys = generate_round_keys(to_block(0x8899aabbccddeeff,0x0011223344556677),
                                            to_block(0xfedcba9876543210, 0x0123456789abcdef), 10);

    int min_cnt_s = 16;
    for (unsigned char constanta = 0; constanta < 1; constanta++) {
        for (int place = 0; place < 16; ++place) {
            block *integr_set = generate_integral_set(place, constanta);

            for (int i = 0; i < 16; ++i) {
                char c = determine_integral_property(i, integr_set);
                printf("%c ", c);
            }
            printf("\n");

            for (int k = 1; k < 3; ++k) {
                printf("k = %d\n", k);
                if (k ==1) {
                    one_round_crypt(integr_set, 256, get_random_key(), 6);
                    print_block(integr_set[1]);
                } else {
                    one_round_crypt(integr_set, 256, get_random_key(), 6);
                }

                int cnt_s = 0;
                for (int i = 0; i < 16; ++i) {
                    char c = determine_integral_property(i, integr_set);
                    if (c == 'S') {
                        cnt_s++;
                    }
                    printf("%c ", c);
                }
                if (cnt_s == 0) {
                    printf("\n");
                } else {
                    if (min_cnt_s > cnt_s) {
                        min_cnt_s = cnt_s;
                    }
                    printf("  // cnt_s = %d \n", cnt_s);
                }
            }
            printf("\n");

        }
    }
    printf("%d\n", min_cnt_s);
    //printf("%lx\n", round_keys[9].left);
    return 0;
}