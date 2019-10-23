#include <stdio.h>

#include "keyarray.h"

/*
 *  File: strkey2.c
 *  Status: Started
 *
 *  Complex String Key Array Example: Word statistics
 *    by Orlando Llanes
 *
 *  https://github.com/orlandol/keyarray
 */

/*
 *  Word statistics declarations
 */

  typedef struct WordStatistics {
    char* wordBeforeFirst;
    unsigned firstLine;
    unsigned firstColumn;
    unsigned repeatCount;
  } WordStatistics;

  DECLARE_STRING_KEYARRAY_TYPES( WordList, WordStatistics )

  void ReleaseStatistics( WordStatistics* data ) {
    if( data ) {
      if( data->wordBeforeFirst ) {
        free( data->wordBeforeFirst );
        data->wordBeforeFirst = NULL;
      }

      data->firstLine = 0;
      data->firstColumn = 0;
      data->repeatCount = 0;
    }
  }

/*
 *  Main program
 */
int main( int argc, char* argv[] ) {
  return 0;
}
