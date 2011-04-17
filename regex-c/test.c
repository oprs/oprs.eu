
#include "regex.h"
#include "lisp.h"


static const char* symtab[] = {
   "SEQ", "ALT", "REP", "ANY"
};


int
main( int argc, char *argv[] )
{
   atom_t z = list( ATOM_CHAR('a'), list( ATOM_CHAR('b'), ATOM_INT(123), ATOM_CHAR('c'), nao ), ATOM_SYM(1), nao );
   dump( z, symtab );

   if( argc > 1 ) {
//   re_mbuf_t m = { "(a.b)?|c", 0 };
      re_mbuf_t m = { argv[1], 0 };
      atom_t c = re_expr( &m );
      dump( c, symtab );
//      dump( c, 0 );
   }

   return 0;
}

