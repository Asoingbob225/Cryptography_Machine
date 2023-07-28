/**
 * @file aes.h
 * @author Jimin Yu, jyu34
 * This is the header file for aes.c. It contains all function declarations as well as macros used for unit testing.
*/

/** Macro used for unit testing */
#ifndef _AES_H_
/** Macro used for unit testing */
#define _AES_H_

#include "field.h"

/** Number of bytes in an AES key or an AES block. */
#define BLOCK_SIZE 16

/** Numer of rows when a data block is arranged in a square. */
#define BLOCK_ROWS 4

/** Numer of columns when a data block is arranged in a square. */
#define BLOCK_COLS 4

/** Number of bytes in a word. */
#define WORD_SIZE 4

/** Number of roudns for 128-bit AES. */
#define ROUNDS 10

/** Represents index 2 in array operations */
#define INDEX2 2

/** Represents index 3 in array operations */
#define INDEX3 3

/** Represents index 4 in array operations */
#define INDEX4 4

/** Represents index 5 in array operations */
#define INDEX5 5

/** Represents index 6 in array operations */
#define INDEX6 6

/** Represents index 7 in array operations */
#define INDEX7 7

/** Represents index 8 in array operations */
#define INDEX8 8

/** Represents index 9 in array operations */
#define INDEX9 9

/** Represents index 10 in array operations */
#define INDEX10 10

/** Represents index 11 in array operations */
#define INDEX11 11

/** Represents index 12 in array operations */
#define INDEX12 12

/** Represents index 13 in array operations */
#define INDEX13 13

/** Represents index 14 in array operations */
#define INDEX14 14

/** Represents index 15 in array operations */
#define INDEX15 15

/** Required number of command line arguments */
#define NUMARGS 4

/**
 * This function computes the g function used in generating the subkeys from the original, 16-byte key. It takes
 * a 4-byte input via the src parameter and returns a 4-byte result via the dest parameter. The value, r, gives
 * the round number, between 1 and 10 (inclusive).
 * @param dest the destination array to put the output of the gfunction
 * @param src the source array with which to compute the gfunction
 * @param r the round number used in the gfunction computation
*/
void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r );

/**
 * This function fills in the subkey array with subkeys for each round of AES, computed from the given key.
 * @param subkey the subkey array which will contain all of the subkeys
 * @param key the key with which to generate the subkeys
*/
void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

/**
 * This function adds the given subkey (key) to the given data array.
 * @param data the data array to add the subkey to
 * @param key the subkey to add to the data array
*/
void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

/**
 * This function rearranges a block of 16 data values from the one-dimensional arrangement to the 4×4 arrangement.
 * @param square the 4x4 square matrix that the 16-block array will be rearranged to
 * @param data the 16-block data array that will be rearranged to a 4x4 matrix
*/
void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] );

/**
 * This function rearranges a 4×4 arrangement of data values, returning them as a one-dimensional array of 16 values.
 * @param data the 16-block data array which the 4x4 square matrix will be rearranged to
 * @param square the 4x4 square matrix which will be rearranged to a 16-block one-dimensional data array
*/
void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * This function performs the shiftRows operation on the given 4×4 square of values. It keeps the first row the same,
 * shifts the second row to the left by 1, the third row to the left by 2, and the fourth row to the left by 3.
 * @param square the 4x4 square of values to perform the shiftRows operation on.
*/
void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * This function performs the inverse shiftRows operation on the given 4 × 4 square of values. It keeps the first row
 * the same, shifts the second row to the right by 1, the third row to the right by 2, and the fourth row to the right
 * by 3.
 * @param square the 4x4 square of values to perform the inverse shiftRows operation on.
*/
void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * This function performs the mixColumns operation on the given 4×4 square of values, multiplying each column by the matrix
 * given in the function implementation.
 * @param square the matrix to multiply the given matrix by
*/
void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * This function performs the inverse of the mixColumns operation on the given 4 × 4 square of values, multiplying each column
 * by the inverse of the matrix given in the function implementation.
 * @param square the matrix to perform the inverse of the mixColumns operation on
*/
void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

/**
 * This function encrypts a 16-byte block of data using the given key. It generates the 11 subkeys from key, adds the first subkey,
 * then performs the 10 rounds of operations needed to encrypt the block.
 * @param data the data to perform the encryption on
 * @param key the key to perform the encryption with
*/
void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );

/**
 * This function decrypts a 16-byte block of data using the given key. It generates the 11 subkeys from key, then performs the 10
 * rounds of inverse operations (and then an addSubkey) to decrypt the block.
 * @param data the data to perform the decryption on
 * @param key the key to perform the decryption with
*/
void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );

#endif
