
/*
   Copyright (c) 2011, Olivier Piras <github@oprs.eu>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in
        the documentation and/or other materials provided with the
        distribution.

      * The name of the author may not be used to endorse or promote
        products derived from this software without specific prior
        written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "retool.h"
#include "regex_posix.h"
#include "brzozowski.h"


int
retool_match( int argc, char* argv[] )
{
   if( argc < 3 ) {
      (void)fprintf( stderr, "match expects 2 arguments\n" );
      return 1;
   }

   atom_t atom = re_posix_parse( argv[1] );
   char *x = argv[2];

   while( *x ) {
      atom = brz_deriv( atom, *x );
      if( !atom || truep( atom )) break;
      ++x;
   }

   return !(truep( atom ) || brz_is_nullable( atom ));
}


/*EoF*/
