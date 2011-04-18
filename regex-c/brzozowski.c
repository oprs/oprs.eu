
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

/*
   This code implements Brzozowski's regular expression derivatives.
   For details on RE derivatives, see:

      . Derivatives of Regular Expressions
        Janusz A. Brzozowski - 1964
        http://doc.oprs.eu/brz64.pdf

      . Regular expression derivatives reexamined
        Scott Owens, John Reppy, Aaron Turon - 2009
        http://doc.oprs.eu/ort09.pdf
*/

#include "brzozowski.h"


int
brz_is_nullable( atom_t atom )
{
   return re_is_eps( atom )
      ||  re_is_rep( atom )
      || (re_is_alt( atom ) && (brz_is_nullable( cadr(atom) ) || brz_is_nullable( caddr(atom) )))
      || (re_is_seq( atom ) &&  brz_is_nullable( cadr(atom) ) && brz_is_nullable( caddr(atom) ));
}


atom_t
brz_deriv( atom_t atom, int c )
{
   if( characterp(atom) )
      return PREDICATE( equalp(atom, ATOM_CHAR(c)) );

   if( re_is_rep(atom) )
      return re_seq( brz_deriv(cadr(atom), c), re_rep(cadr(atom)) );

   if( re_is_alt(atom) )
      return re_alt( brz_deriv(cadr(atom), c), brz_deriv(caddr(atom), c) );

   if( re_is_seq(atom) )
      return re_alt(
         re_seq( brz_deriv(cadr(atom), c), caddr(atom) ),
         re_seq( PREDICATE(brz_is_nullable(cadr(atom))), brz_deriv(caddr(atom), c) ));

   return PREDICATE( re_is_any(atom) );
}


/*EoF*/
