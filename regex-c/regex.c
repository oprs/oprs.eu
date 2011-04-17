
/*
   Copyright (c) 2011, Olivier Piras <github-dev@oprs.eu>
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

#include "regex.h"


atom_t
re_seq( atom_t a0, atom_t a1 )
{
   if( nullp( a0 ) || nullp( a1 ) ) return nil;
   if( truep( a0 ) ) return a1;
   if( truep( a1 ) ) return a0;
   return list( RE_SYM_SEQ, a0, a1, nao );
}

atom_t
re_alt( atom_t a0, atom_t a1 )
{
   if( nullp( a0 ) ) return a1;
   if( nullp( a1 ) ) return a0;
   return list( RE_SYM_ALT, a0, a1, nao );
}

atom_t
re_rep( atom_t atom )
{
   if( nullp( atom ) || truep( atom ) ) return t;
   return list( RE_SYM_REP, atom, nao );
}


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

static atom_t
re_expr_r( re_mbuf_t* mbuf, int pl )
{
   atom_t atom;
   int c = MBUF_PULL( mbuf );

/* first character */

   switch( c ) {
      case 0  : return nil;
      case ')': return nil;
      default : atom = re_closure( mbuf, ATOM_CHAR(c) );
   }

/* subsequent characters */

   c = MBUF_PULL( mbuf );

   while( c && c != ')' ) {
      switch( c ) {
         case '|': atom = re_alt( atom, re_closure( mbuf, re_expr_r( mbuf, pl   ) )); break;
         case '(': atom = re_seq( atom, re_closure( mbuf, re_expr_r( mbuf, pl+1 ) )); break;
         case '.': atom = re_seq( atom, re_closure( mbuf, RE_SYM_ANY              )); break;
         default : atom = re_seq( atom, re_closure( mbuf, ATOM_CHAR(c)            )); break;
      }
      c = MBUF_PULL( mbuf );
   }

   return atom;
}

atom_t
re_expr( re_mbuf_t* mbuf )
{ return re_expr_r( mbuf, 0 ); }


void
re_dump( atom_t atom )
{
   const char* symtab[] = { "SEQ", "ALT", "REP", "ANY" };
   dump( atom, symtab );
}


/*EoF*/
