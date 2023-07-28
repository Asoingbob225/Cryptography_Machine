/**
 * @file aes.c
 * @author Jimin Yu, jyu34
 * This file is where all of the AES algorithms for encryption and decryption are implemented.
*/

#include "aes.h"
#include "field.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
   Return the sBox substitution value for a given byte value.
   @param v byte input value.
   @return substitution for the given byte.
*/
static byte substBox( byte v )
{
  // Forward-direction replacement map for the sBox.
  static const byte rule[] =
    { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
      0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
      0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
      0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
      0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
      0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
      0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
      0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
      0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
      0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
      0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
      0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
      0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
      0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
      0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
      0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
      0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
      0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
      0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
      0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
      0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
      0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
      0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
      0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
      0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
      0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
      0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
      0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
      0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
      0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
      0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
      0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 };

  // Return the replaceent for v based on the map.
  return rule[ v ];
}

/**
   Return the inverse sBox substitution value for a given byte value.
   @param v byte input value.
   @return inverse substitution for the given byte.
*/
static byte invSubstBox( byte v )
{
  // Inverse-direction replacement map for the sBox.
  static const byte irule[] =
  { 0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
    0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
    0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
    0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
    0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
    0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
    0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
    0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
    0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
    0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
    0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
    0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
    0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
    0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
    0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D };
  
  // Return the (inverse) replaceent for v based on the map.
  return irule[ v ];
}

void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r )
{
  // Constant values used in each round of the g function.
  static const byte roundConstant[ ROUNDS + 1 ] = {
    0x00, // First element not used.
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
  };

  dest[0] = fieldAdd( substBox( src[1] ), roundConstant[r] );
  dest[1] = substBox( src[INDEX2] );
  dest[INDEX2] = substBox( src[INDEX3] );
  dest[WORD_SIZE - 1] = substBox( src[0] );

}

void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] ) {
  for ( int i = 0; i < BLOCK_SIZE; i++ ) {
    subkey[0][i] = key[i];
  }

  for ( int i = 1; i < ROUNDS + 1; i++ ) {
    byte g[WORD_SIZE];
    byte lastWord[WORD_SIZE] = { subkey[i - 1][INDEX12], subkey[i - 1][INDEX13], subkey[i - 1][INDEX14], subkey[i - 1][INDEX15] };
    gFunction( g, lastWord, i );

    byte firstWord[WORD_SIZE] = { subkey[i - 1][0], subkey[i - 1][1], subkey[i - 1][INDEX2], subkey[i - 1][INDEX3] };
    for ( int j = 0; j < WORD_SIZE; j++ ) {
      firstWord[j] = fieldAdd( firstWord[j], g[j] );
    }
    byte secondWord[WORD_SIZE] = { subkey[i - 1][INDEX4], subkey[i - 1][INDEX5], subkey[i - 1][INDEX6], subkey[i - 1][INDEX7] };
    for ( int j = 0; j < WORD_SIZE; j++ ) {
      secondWord[j] = fieldAdd( secondWord[j], firstWord[j] );
    }
    byte thirdWord[WORD_SIZE] = { subkey[i - 1][INDEX8], subkey[i - 1][INDEX9], subkey[i - 1][INDEX10], subkey[i - 1][INDEX11] };
    for ( int j = 0; j < WORD_SIZE; j++ ) {
      thirdWord[j] = fieldAdd( thirdWord[j], secondWord[j] );
    }

    for ( int j = 0; j < WORD_SIZE; j++ ) {
      lastWord[j] = fieldAdd( lastWord[j], thirdWord[j] );
    }

    for ( int j = 0; j < WORD_SIZE; j++ ) {
      subkey[i][j] = firstWord[j];
      subkey[i][j + INDEX4] = secondWord[j];
      subkey[i][j + INDEX8] = thirdWord[j];
      subkey[i][j + INDEX12] = lastWord[j];
    }
  }
}

void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] ) {
  for ( int i = 0; i < BLOCK_SIZE; i++ ) {
    data[i] = fieldAdd( data[i], key[i] );
  }
}

void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] ) {
  for ( int i = 0; i < BLOCK_ROWS; i++ ) {
    for ( int j = 0; j < BLOCK_COLS; j++ ) {
      square[i][j] = data[i + INDEX4 * j];
    }
  }
}

void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  for ( int i = 0; i < BLOCK_ROWS; i++ ) {
    for ( int j = 0; j < BLOCK_COLS; j++ ) {
      data[i + INDEX4 * j] = square[i][j];
    }
  }
}

void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  byte temp;

  // second row, left shift one
  temp = square[1][0];
  square[1][0] = square[1][1];
  square[1][1] = square[1][INDEX2];
  square[1][INDEX2] = square[1][INDEX3];
  square[1][INDEX3] = temp;

  // third row, left shift two
  temp = square[INDEX2][0];
  square[INDEX2][0] = square[INDEX2][INDEX2];
  square[INDEX2][INDEX2] = temp;
  temp = square[INDEX2][1];
  square[INDEX2][1] = square[INDEX2][INDEX3];
  square[INDEX2][INDEX3] = temp;

  // fourth row, right shift one (same as left shift 3)
  temp = square[INDEX3][INDEX3];
  square[INDEX3][INDEX3] = square[INDEX3][INDEX2];
  square[INDEX3][INDEX2] = square[INDEX3][1];
  square[INDEX3][1] = square[INDEX3][0];
  square[INDEX3][0] = temp;
}

void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] ) {
  byte temp;

  // second row, right shift one
  temp = square[1][3];
  square[1][3] = square[1][2];
  square[1][2] = square[1][1];
  square[1][1] = square[1][0];
  square[1][0] = temp;

  // third row, right shift two
  temp = square[2][3];
  square[2][3] = square[2][1];
  square[2][1] = temp;
  temp = square[2][2];
  square[2][2] = square[2][0];
  square[2][0] = temp;

  // fourth row, left shift one (same as right shift 3)
  temp = square[3][0];
  square[3][0] = square[3][1];
  square[3][1] = square[3][2];
  square[3][2] = square[3][3];
  square[3][3] = temp;
}

void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{
  // Matrix by which each column of square is multiplied.
  static const byte mixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ] = {
    { 0x02, 0x03, 0x01, 0x01 },
    { 0x01, 0x02, 0x03, 0x01 },
    { 0x01, 0x01, 0x02, 0x03 },
    { 0x03, 0x01, 0x01, 0x02 }
  };

  byte temp[BLOCK_ROWS];

  for ( int i = 0; i < BLOCK_COLS; i++ ) {
    // Copy the column into a temporary array
    for (int j = 0; j < BLOCK_ROWS; j++ ) {
      temp[j] = square[j][i];
    }

    square[0][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( mixMatrix[0][0], temp[0] ), fieldMul(mixMatrix[0][1], temp[1] ) ), fieldMul( mixMatrix[0][INDEX2], temp[INDEX2] ) ), fieldMul( mixMatrix[0][INDEX3], temp[INDEX3] ) );
    square[1][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( mixMatrix[1][0], temp[0] ), fieldMul( mixMatrix[1][1], temp[1] ) ), fieldMul( mixMatrix[1][INDEX2], temp[INDEX2] ) ), fieldMul( mixMatrix[1][INDEX3], temp[INDEX3] ) );
    square[INDEX2][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( mixMatrix[INDEX2][0], temp[0] ), fieldMul( mixMatrix[INDEX2][1], temp[1] ) ), fieldMul( mixMatrix[INDEX2][INDEX2], temp[INDEX2] ) ), fieldMul( mixMatrix[INDEX2][INDEX3], temp[INDEX3] ) );
    square[INDEX3][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( mixMatrix[INDEX3][0], temp[0] ), fieldMul( mixMatrix[INDEX3][1], temp[1] ) ), fieldMul( mixMatrix[INDEX3][INDEX2], temp[INDEX2] ) ), fieldMul( mixMatrix[INDEX3][INDEX3], temp[INDEX3] ) );
  }
}

void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{
  // Matrix by which each column of square is multiplied.
  static const byte invMixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ] = {
    { 0x0E, 0x0B, 0x0D, 0x09 },
    { 0x09, 0x0E, 0x0B, 0x0D },
    { 0x0D, 0x09, 0x0E, 0x0B },
    { 0x0B, 0x0D, 0x09, 0x0E }
  };

    byte temp[BLOCK_ROWS];

  for ( int i = 0; i < BLOCK_COLS; i++ ) {
    // Copy the column into a temporary array
    for (int j = 0; j < BLOCK_ROWS; j++) {
      temp[j] = square[j][i];
    }

    square[0][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( invMixMatrix[0][0], temp[0] ), fieldMul(invMixMatrix[0][1], temp[1] ) ), fieldMul( invMixMatrix[0][INDEX2], temp[INDEX2] ) ), fieldMul( invMixMatrix[0][INDEX3], temp[INDEX3] ) );
    square[1][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( invMixMatrix[1][0], temp[0] ), fieldMul( invMixMatrix[1][1], temp[1] ) ), fieldMul( invMixMatrix[1][INDEX2], temp[INDEX2] ) ), fieldMul( invMixMatrix[1][INDEX3], temp[INDEX3] ) );
    square[INDEX2][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( invMixMatrix[INDEX2][0], temp[0] ), fieldMul( invMixMatrix[INDEX2][1], temp[1] ) ), fieldMul( invMixMatrix[INDEX2][INDEX2], temp[INDEX2] ) ), fieldMul( invMixMatrix[INDEX2][INDEX3], temp[INDEX3] ) );
    square[INDEX3][i] = fieldAdd( fieldAdd( fieldAdd( fieldMul( invMixMatrix[INDEX3][0], temp[0] ), fieldMul( invMixMatrix[INDEX3][1], temp[1] ) ), fieldMul( invMixMatrix[INDEX3][INDEX2], temp[INDEX2] ) ), fieldMul( invMixMatrix[INDEX3][INDEX3], temp[INDEX3] ) );
  }
}

void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] ) {
  byte subKey[ROUNDS + 1][BLOCK_SIZE];
  generateSubkeys( subKey, key );
  byte square[BLOCK_ROWS][BLOCK_COLS];

  addSubkey( data, subKey[0] );

  for ( int i = 1; i < ROUNDS + 1; i++ ) {
    for ( int j = 0; j < BLOCK_SIZE; j++ ) {
      data[j] = substBox( data[j] );
    }
    blockToSquare( square, data );
    shiftRows( square );
    if ( i != INDEX10 ) {
      mixColumns( square );
    }
    squareToBlock( data, square );
    addSubkey( data, subKey[i] );
  }
}

void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] ) {
  byte subKey[ROUNDS + 1][BLOCK_SIZE];
  generateSubkeys( subKey, key );
  byte square[BLOCK_ROWS][BLOCK_COLS];

  for ( int i = ROUNDS; i > 0; i-- ) {
    addSubkey( data, subKey[i] );
    blockToSquare( square, data );
    if ( i != INDEX10 ) {
      unMixColumns( square );
    }
    unShiftRows( square );
    squareToBlock( data, square );
    for ( int j = 0; j < BLOCK_SIZE; j++ ) {
      data[j] = invSubstBox( data[j] );
    }
  }
  addSubkey( data, subKey[0] );
}
