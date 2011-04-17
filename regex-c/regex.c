
#include <stdio.h>

#include "regex.h"


atom_t
re_seq( atom_t a0, atom_t a1 )
{ return list( RE_TYPE_SEQ, a0, a1, nao ); }


atom_t
re_alt( atom_t a0, atom_t a1 )
{ return list( RE_TYPE_ALT, a0, a1, nao ); }


atom_t
re_rep( atom_t atom )
{ return list( RE_TYPE_REP, atom, nao ); }


atom_t
re_closure( re_mbuf_t* mbuf, atom_t atom )
{
   int c = MBUF_PEEK( mbuf );

   switch( c ) {
      case '?': (void)MBUF_PULL( mbuf ); return re_alt( atom, t );
      case '*': (void)MBUF_PULL( mbuf ); return re_rep( atom );
      case '+': (void)MBUF_PULL( mbuf ); return re_seq( atom, re_rep( atom ));
      default : break;
   }

   return atom;
}


atom_t
re_expr( re_mbuf_t* mbuf )
{
   atom_t atom;
   int c = MBUF_PULL( mbuf );

/* first character */

   switch( c ) {
      case 0  :
      case ')': return nil;
      default: atom = ATOM_CHAR(c);
   }

/* subsequent characters */

   c = MBUF_PULL( mbuf );

   while( c && c != ')' ) {
      switch( c ) {
         case '|': atom = re_alt( atom, re_closure( mbuf, re_expr( mbuf ) ));
         case '(': atom = re_seq( atom, re_closure( mbuf, re_expr( mbuf ) ));
         case '.': atom = re_seq( atom, re_closure( mbuf, RE_TYPE_ANY     ));
         default : atom = re_seq( atom, re_closure( mbuf, ATOM_CHAR(c)    ));
      }
      c = MBUF_PULL( mbuf );
   }

   return atom;
}


/*EoF*/
