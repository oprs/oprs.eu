
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


#include "regex.h"


atom_t
re_seq( atom_t a0, atom_t a1 )
{
   if( !a0 || !a1 ) return nil;
   if( truep(a0) ) return a1;
   if( truep(a1) ) return a0;

   if( re_is_rep(a0)) {
      if( equalp( a0, a1 )      ) return a0;
      if( equalp( cadr(a0), a1 )) return re_seq( a1, a0 );
   }

   if( re_is_seq(a0) ) {
      if( !consp(a1) ) {
         /* (SEQ (SEQ a b) c) -> (SEQ a (SEQ b c)) */
         return re_seq( cadr(a0), re_seq( caddr(a0), a1 ));
      }

      if( re_is_seq(a1) ) {
         /* (SEQ (SEQ a b) (SEQ c d)) -> (SEQ a (SEQ b (SEQ c d))) */
         return re_seq( cadr(a0), re_seq( caddr(a0), re_seq( cadr(a1), caddr(a1) )));
      }
   }

   return list3( RE_SYM_SEQ, a0, a1 );
}


atom_t
re_alt( atom_t a0, atom_t a1 )
{
   if( !a0 ) return a1;
   if( !a1 ) return a0;

   if( equalp(a0, a1) )
      return a0;

/*
   if( ((a0.x & TAG_MASK) == (a1.x & TAG_MASK))
    && ( a0.x > a1.x ) )
      return list3( RE_SYM_ALT, a1, a0 );
*/

   return list3( RE_SYM_ALT, a0, a1 );
}


atom_t
re_rep( atom_t atom )
{
   if( !atom || truep(atom) ) return t;

   if( re_is_rep(atom) )
      return re_rep( cadr(atom) );

   return list2( RE_SYM_REP, atom );
}


int
re_is_equiv( atom_t a0, atom_t a1 )
{ return equalp( a0, a1 ); }


void
re_dump( atom_t atom )
{
//   const char* symtab[] = { "SEQ", "ALT", "REP", "ANY" };
   dump( atom );
}


/*EoF*/
