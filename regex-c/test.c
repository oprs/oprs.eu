
#include <stdio.h>

#include "brzozowski.h"
#include "regex.h"
#include "lisp.h"


int
main( int argc, char *argv[] )
{
   if( argc > 1 ) {
      re_mbuf_t m = { argv[1], 0 };
      atom_t c = re_expr( &m );
      re_dump( c );
      (void)printf( "is-nullable: %d\n", brz_is_nullable( c ) );
      (void)printf( "a: " ); re_dump( brz_deriv( c, 'a' ) );
      (void)printf( "b: " ); re_dump( brz_deriv( c, 'b' ) );
      (void)printf( "c: " ); re_dump( brz_deriv( c, 'c' ) );
   }

   return 0;
}

