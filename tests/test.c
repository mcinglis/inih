
// Run these tests by doing something like:
//
//     $ diff stdout_expected.txt <(./unittest)
//
// The tests pass if there was no diff output.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ini.h"


int count = 1;


bool dumper( void * const vn,
             char const * const section,
             char const * const key,
             char const * const value)
{
    int * const n = vn;
    *n = count;
    count++;

    printf( "[%s] \"%s\" = \"%s\"\n", section, key, value );
    return true;
}


void
parse( char const * const filepath ) {
    int n = 0;
    printf( "%s:\n", filepath );
    IniError const err = ini_parse( filepath, dumper, &n );
    printf( "%s: n = %d\n", filepath, n );
    if ( err.desc != NULL ) {
        printf( "%s: error at line %zu: %s\n",
                filepath, err.line, err.desc );
    }
    printf( "------\n" );
}


int
main( void )
{
    parse( "no_file.ini" );
    parse( "normal.ini" );
    parse( "bad_section.ini" );
    parse( "bad_comment.ini" );
    parse( "user_error.ini" );
    parse( "bom.ini" );
    parse( "colon.ini" );
}

