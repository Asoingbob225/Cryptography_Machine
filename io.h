/**
 * @file io.h
 * @author Jimin Yu, jyu34
 * This is the header file for io.c. It contains all function declarations.
*/

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/**
 * This function reads the contents of the binary file with the given name. It returns a pointer to a dynamically allocated
 * array of bytes containing the entire file contents. The size parameter is an integer that’s passed by reference to the
 * function. The function fills in this integer with the total size of the file (i.e., how many bytes are in the returned array).
 * @param filename the file to read from
 * @param size the total size of the file, determined by the function
 * @return an array containing the entire file contents
*/
byte *readBinaryFile( char const *filename, int *size );

/**
 * This function writes the contents of the given data array (in binary) to the file with the given name. The size parameter says
 * how many bytes are contained in the data array.
 * @param filename the file to write to
 * @param data an array of bytes containing the data to write
 * @param size the size of the data array, or the number of bytes to write
*/
void writeBinaryFile( char const *filename, byte *data, int size );
