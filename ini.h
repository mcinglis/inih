
// Copyright (c) 2009 Brush Technology
//                    <http://brush.co.nz/>
//
// Copyright (c) 2015 Positronic Manufacturing
//                    <http://positronicsolar.com.au/>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Brush Technology, nor Positronic Manufacturing, nor
//       the names of other contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED ''AS IS'' BY THE AUTHORS AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


#ifndef INIH_INI_H
#define INIH_INI_H


#include <stdio.h>
#include <libtypes/types.h>


// Maximum length for lines in an .ini file:
#ifndef INI_MAX_LINE_LEN
#define INI_MAX_LINE_LEN 1024
#endif

// Maximum length of section headers:
#ifndef INI_MAX_SECTION_LEN
#define INI_MAX_SECTION_LEN 64
#endif

// Maximum length of property keys:
#ifndef INI_MAX_KEY_LEN
#define INI_MAX_KEY_LEN 64
#endif


typedef struct ini_error {
    char const * desc;
    size_t line;
} IniError;


// Parse given INI-style file. May have [section]s, name=value pairs
// (whitespace stripped), and comments starting with ';' (semicolon). Section
// is "" if name=value pair parsed before any section heading. name:value
// pairs are also supported as a concession to Python's ConfigParser.
//
// For each name=value pair parsed, call handler function with given user
// pointer as well as section, name, and value (data only valid for duration
// of handler call). Handler should return nonzero on success, zero on error.
//
// Returns 0 on success, line number of first error on parse error (doesn't
// stop on first error), -1 on file open error, or -2 on memory allocation
// error (only when INI_USE_STACK is zero).
IniError
ini_parse( char const * filename,
           bool ( * callback )( void * data,
                                char const * section,
                                char const * key,
                                char const * value ),
           void * data );


// Same as `ini_parse()`, but takes a `FILE *` instead of filename. This
// doesn't close the file when it's finished -- the caller must do that.
IniError
ini_parse_file( FILE * file,
                bool ( * callback )( void * data,
                                     char const * section,
                                     char const * key,
                                     char const * value ),
                void * data );


#endif

