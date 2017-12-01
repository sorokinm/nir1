#include <stdio.h>

#include "kuznechik.h"

int main() {
    printf("Hello, World!\n");
    block text = {.left = 0x9400000000000000, .right = 0x0000000000000001};

    block subs_text = R(text);
    printf("%#016lx\t%#016lx\n", subs_text.left, subs_text.right);
    return 0;
}