
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "regex_posix.h"


int
main( int argc, char *argv[] )
{
   atom_t a0, a1;
   re_mbuf_t m;

   if( argc < 2 ) {
      (void)fprintf( stderr, "usage: %s re1 re2\n", argv[0] );
      exit(1);
   }

   m.x = argv[1];
   m.i = 0;
   a0 = re_posix_parse( &m );

   m.x = argv[2];
   m.i = 0;
   a1 = re_posix_parse( &m );

   return !re_is_equiv( a0, a1 );
}

