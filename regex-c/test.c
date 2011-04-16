
#include "regex.h"

int
main( int argc, char *argv[] )
{
   se_elem_t a = SEQ( SYM('a'), SYM('b') );
   se_elem_t b = ALT( a, REP(SYM('c')) );
   se_elem_dump( b );

   se_elem_t c = re_expr( "zzz" );
   se_elem_dump( c );

   return 0;
}

