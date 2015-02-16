
// parts copyright (c) 2009 Brush Technology
//                          <http://brush.co.nz/>
// parts copyright (c) 2015 Positronic Manufacturing
//                          <http://positronicsolar.com.au/>


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <libtypes/types.h>
#include <libmacro/assert.h>

#include "ini.h"


// Strip whitespace chars off end of given string, in place. Return s.
static
char *
rstrip( char * const s )
{
    ASSERT( s != NULL );

    char * p = s + strlen( s );
    while ( p > s && isspace( ( unsigned char )( *--p ) ) ) {
        *p = '\0';
    }
    return s;
}


// Return a pointer to the first non-whitespace character in `s`.
static
char *
lskip( char * const s )
{
    ASSERT( s != NULL );

    size_t i = 0;
    while ( isspace( s[ i ] ) ) {
        i++;
    }
    return s + i;
}


// Version of strncpy that ensures dest (size bytes) is null-terminated.
static
char *
strncpy0( char * const dest,
          char const * const src,
          size_t const size )
{
    ASSERT( dest != NULL, src != NULL );

    strncpy( dest, src, size );
    dest[ size - 1 ] = '\0';
    return dest;
}


static
void
strip_comment( char * const line )
{
    ASSERT( line != NULL );

    for ( size_t i = 1; line[ i ] != '\0'; i++ ) {
        if ( line[ i ] == ';' && isspace( line[ i - 1 ] ) ) {
            line[ i ] = '\0';
            break;
        }
    }
}


IniError
ini_parse_file( FILE * const file,
                bool ( * const callback )( void * data,
                                           char const * section,
                                           char const * key,
                                           char const * value ),
                void * const data )
{
    ASSERT( file != NULL, callback != NULL );

    char line_buf[ INI_MAX_LINE_LEN + 1 ]   = { 0 };
    char section[ INI_MAX_SECTION_LEN + 1 ] = { 0 };
    bool in_section = false;

    // Step through file line by line:
    int lineno = 0;
    while ( fgets( line_buf, sizeof line_buf, file ) != NULL ) {
        lineno++;

        char * line = line_buf;

        // Check for the leading BOM if any:
        if ( lineno == 1 && ( ( uchar ) line[ 0 ] ) == 0xEF &&
                            ( ( uchar ) line[ 1 ] ) == 0xBB &&
                            ( ( uchar ) line[ 2 ] ) == 0xBF ) {
            line += 3;
        }

        // Strip leading and trailing whitespace:
        line = lskip( rstrip( line ) );

        if ( line[ 0 ] == ';' || line[ 0 ] == '\0' ) {
            continue;
        } else if ( line[ 0 ] == '[' ) {
            // We're parsing a section header line:
            in_section = true;
            char * const end = strchr( line + 1, ']' );
            if ( end == NULL ) {
                return ( IniError ){ .desc = "unclosed section header",
                                     .line = lineno };
            }
            *end = '\0';
            strncpy0( section, lskip( rstrip( line + 1 ) ), sizeof section );
        } else {
            // Must be a name=value pair:
            strip_comment( line );
            // Separate the key and value:
            char * const eq = strchr( line, '=' );
            if ( eq == NULL ) {
                return ( IniError ){ .desc = "missing assignment characater",
                                     .line = lineno };
            }
            char * const key = line;
            char * const value = lskip( eq + 1 );
            // Terminate `key` at the assignment character:
            *eq = '\0';
            // Strip any trailing whitespace characters:
            rstrip( key );
            rstrip( value );
            // Finally call the callback with this key and value:
            if ( callback( data, in_section ? section : NULL,
                           key, value ) == false ) {
                return ( IniError ){ .desc = "callback failed",
                                     .line = lineno };
            }
        }
    }

    return ferror( file ) ? ( IniError ){ "error reading file" }
                          : ( IniError ){ 0 };
}


IniError
ini_parse( char const * const filename,
           bool ( * const callback )( void * data,
                                      char const * section,
                                      char const * key,
                                      char const * value ),
           void * const data )
{
    ASSERT( filename != NULL, callback != NULL );

    FILE * const file = fopen(filename, "r");
    if ( file == NULL ) {
        return ( IniError ){ "couldn't open file" };
    }
    IniError const parse_err = ini_parse_file( file, callback, data );
    int const close_err = fclose( file );
    return ( parse_err.desc != NULL ) ? parse_err
         : ( close_err != 0 )         ? ( IniError ){ "couldn't close file" }
                                      : ( IniError ){ 0 };
}

