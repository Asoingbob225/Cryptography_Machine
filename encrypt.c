/**
 * @file encrypt.c
 * @author Jimin Yu, jyu34
 * This file contains the program execution for the encrypt functionality.
*/

#include "aes.h"
#include "field.h"
#include "io.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * This is the main method for the encrypt functionality. It carries program execution.
 * @param argc the number of command line arguments given
 * @param argv an array of all the command line arguments
 * @return program exit status
*/
int main( int argc, char *argv[] ) {
    if ( argc !=  NUMARGS ) {
        fprintf( stderr, "usage: encrypt <key-file> <input-file> <output-file>\n" );
        exit( EXIT_FAILURE );
    }

    byte* key = NULL;
    int sizeKey = 0;
    key = readBinaryFile( argv[1], &sizeKey );

    byte* plainText = NULL;
    int sizePlainText = 0;
    plainText = readBinaryFile( argv[INDEX2], &sizePlainText );

    if ( sizeKey != BLOCK_SIZE ) {
        free( key );
        free( plainText );
        fprintf( stderr, "Bad key file: %s\n", argv[1] );
        exit( EXIT_FAILURE );
    }

    if ( sizePlainText % BLOCK_SIZE != 0 ) {
        free( plainText );
        free( key );
        fprintf( stderr, "Bad plaintext file length: %s\n", argv[INDEX2] );
        exit( EXIT_FAILURE );
    }

    byte allData[sizePlainText];
    for ( int i = 0; i < sizePlainText; i += BLOCK_SIZE ) {
        byte temp[BLOCK_SIZE];
        for ( int j = 0; j < BLOCK_SIZE; j++ ) {
            temp[j] = plainText[i + j];
        }
        encryptBlock( temp, key );
        
        for ( int j = 0; j < BLOCK_SIZE; j++ ) {
            allData[i + j] = temp[j];
        }
    }
    writeBinaryFile( argv[INDEX3], allData, sizePlainText );

    free( key );
    free( plainText );
    exit( EXIT_SUCCESS );
}
