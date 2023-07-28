/**
 * @file field.c
 * @author Jimin Yu, jyu34
 * This file contains implementation supporting basic arithmetic operations in an 8-bit (1 byte) Galois field.
 * These operations are used internally by AES for many steps in the encryption and decryption process.
*/

#include "field.h"
#include <stdlib.h>

byte fieldAdd( byte a, byte b ) {
    return a ^ b;
}

byte fieldSub( byte a, byte b ) {
    return a ^ b;
}

byte fieldMul( byte a, byte b ) {
    byte p = 0; // product
    byte bitcount;
    byte most_sig_bit;

    for ( bitcount = 0; bitcount < BBITS; bitcount++ ) {
        if ( b & 1 ) {
            p ^= a;
        }
        most_sig_bit = ( a & HIBIT );
        a <<= 1;
        if ( most_sig_bit ) {
            a ^= REDUCER;
        }
        b >>= 1;
    }
    return p;   
}
