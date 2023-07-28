/**
 * @file field.h
 * @author Jimin Yu, jyu34
 * This is the header file for field.c. It contains all of the function declarations as well as some macros used for unit testing.
*/

/** Macro defined for unit testing */
#ifndef _FIELD_H_

/** Macro defined for unit testing */
#define _FIELD_H_

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8

/** "Reducer" value which is shifted during multiplication calculations */
#define REDUCER 0x1b

/** Highest bit setter used during multiplcation calculations */
#define HIBIT 0x80

/**
 * This function performs the addition operation in the 8-bit Galois field used by AES. It adds a and b and returns the result.
 * @param a the first byte value to add
 * @param b the second byte value to add
 * @return the exclusive or of a and b
*/
byte fieldAdd( byte a, byte b );

/**
 * This function performs the subtraction operation in the 8-bit Galois field used by AES. It subtracts b from a and returns the result.
 * @param a the byte value to subtract from
 * @param b the byte value to subtract
 * @return the exclusive or of a and b (it is the exact same as addition in an 8-bit Galois field)
*/
byte fieldSub( byte a, byte b );

/**
 * This function performs the multiplication operation in the 8-bit Galois field used by AES. It multiplies a and b and returns the result.
 * @param a the first byte value to multiply
 * @param b the second byte value to multiply
 * @return the product of multiplication in the 8-bit Galois field
*/
byte fieldMul( byte a, byte b );

#endif
