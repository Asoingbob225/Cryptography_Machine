/**
 * @file io.c
 * @author Jimin Yu, jyu34
 * This file contains the functionality responsible for reading input files and writing output files when 
 * the encrypt or decrypt program is done.
*/

#include "io.h"
#include <stdio.h>
#include <stdlib.h>

byte *readBinaryFile( char const *filename, int *size ) {
    FILE *input = fopen( filename, "rb" );
    if ( !input ) {
        fprintf( stderr, "Can't open file: %s\n", filename );
        exit( EXIT_FAILURE );
    }

    fseek( input, 0, SEEK_END );
    long fileSize = ftell( input );
    rewind( input );

    byte *filecontents = ( byte * ) malloc ( fileSize * sizeof( byte ) );
    *size = ( int ) fileSize;
    fread( filecontents, 1, *size, input );

    fclose( input );

    return filecontents;
}

void writeBinaryFile( char const *filename, byte *data, int size ) {
    FILE *output = fopen( filename, "wb" );
    if ( !output ) {
        fprintf( stderr, "Can't open file: %s\n", filename );
        exit(EXIT_FAILURE);
    }

    fwrite( data, 1, size, output );

    fclose( output );
}
