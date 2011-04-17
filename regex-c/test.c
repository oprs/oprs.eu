
#include <stdio.h>

#include "brzozowski.h"
#include "regex.h"
#include "lisp.h"


int
main( int argc, char *argv[] )
{
   atom_t z = list( ATOM_CHAR('a'), list( ATOM_CHAR('b'), ATOM_INT(123), ATOM_CHAR('c'), nao ), ATOM_SYM(1), nao );
   re_dump( z );

   if( argc > 1 ) {
      re_mbuf_t m = { argv[1], 0 };
      atom_t c = re_expr( &m );
      re_dump( c );
      (void)printf( "is-nullable: %d\n", brz_is_nullable( c ) );
      (void)printf( "a: " ); re_dump( brz_derive( c, 'a' ) );
      (void)printf( "b: " ); re_dump( brz_derive( c, 'b' ) );
      (void)printf( "c: " ); re_dump( brz_derive( c, 'c' ) );
   }

   return 0;
}

