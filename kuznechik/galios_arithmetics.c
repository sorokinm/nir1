/* Add two numbers in a GF(2^8) finite field */
unsigned char gadd(unsigned char a, unsigned char b) {
    return a ^ b;
}

/* Subtract two numbers in a GF(2^8) finite field */
unsigned char gsub(unsigned char a, unsigned char b) {
    return a ^ b;
}

/* Multiply two numbers in the GF(2^8) finite field defined
 * by the polynomial x^8 + x^7 + x^6 + x + 1 = 0
 * using the Russian Peasant Multiplication algorithm
 * (the other way being to do carry-less multiplication followed by a modular reduction)
 */
unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0; /* the product of the multiplication */
    while (a && b) {
        if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
            p ^= a; /* since we're in GF(2^m), addition is an XOR */

        if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
            a = (a << 1) ^ 0x1C3; /* XOR with the primitive polynomial x^8 + x^7 + x^6 + x + 1 (0b1_1100_0011) – you can change it but it must be irreducible */
        else
            a <<= 1; /* equivalent to a*2 */
        b >>= 1; /* equivalent to b // 2 */
    }
    return p;
}

