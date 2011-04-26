
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
#include <string.h>

#include "retool.h"
#include "ulisp.h"


struct cmd_s {
   char *cmd;
   int (*cfn)( int, char* [] );
};

struct cmd_s cv[] = {
   { "dump",  retool_dump  },
   { "echo",  retool_echo  },
   { "equiv", retool_equiv },
   { "deriv", retool_deriv },
   { "match", retool_match },
   { "dfa",   retool_dfa   },
   { 0,       0            }
};


int
main( int argc, char *argv[] )
{
   int i;

   if( argc < 2 ) {
      (void)fprintf( stderr, "usage: %s command [args...]\n", argv[0] );
      exit(1);
   }

   for( i = 0 ; cv[i].cmd ; ++i )
      if( !strcmp( cv[i].cmd, argv[1] ) ) {
         int rv = cv[i].cfn( argc-1, argv+1 );
         // pool_free();
         return rv;
      }

   (void)fprintf( stderr, "unknown command \"%s\"\n", argv[1] );
   return 1;
}


/*EoF*/
