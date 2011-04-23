
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

#include "regex_posix.h"
#include "brzozowski.h"


static void
re_posix_dump_r( atom_t atom )
{
   if( re_is_rep( atom ) ) {
      if( consp( cadr(atom) ) ) {
         (void)printf( "(" );
         re_posix_dump_r( cadr(atom) );
         (void)printf( ")" );
      } else {
         re_posix_dump_r( cadr(atom) );
      }
      (void)printf( "*" );
      return;
   }

   if( re_is_alt( atom ) ) {
      if( re_is_eps( caddr(atom) ) ) {
         if( consp( cadr(atom) ) ) {
            (void)printf( "(" );
            re_posix_dump_r( cadr(atom) );
            (void)printf( ")" );
         } else {
            re_posix_dump_r( cadr(atom) );
         }
         (void)printf( "?" );
      } else {
         // (void)printf( "(" );
         re_posix_dump_r( cadr(atom) );
         (void)printf( "|" );
         re_posix_dump_r( caddr(atom) );
         // (void)printf( ")" );
      }
      return;
   }

   if( re_is_seq( atom ) ) {
      atom_t a0 = cadr(atom);
      atom_t a1 = caddr(atom);

      if( re_is_rep( a1 ) && re_is_equiv( a0, cadr(a1) )) {
         if( consp( a0 ) ) {
            (void)printf( "(" );
            re_posix_dump_r( a0 );
            (void)printf( ")" );
         } else {
            re_posix_dump_r( a0 );
         }
         (void)printf( "+" );
      } else {
         if( re_is_rep( a0 ) && re_is_equiv( cadr(a0), a1 )) {
            if( consp( a1 ) ) {
               (void)printf( "(" );
               re_posix_dump_r( a1 );
               (void)printf( ")" );
            } else {
               re_posix_dump_r( a1 );
            }
            (void)printf( "+" );
         } else {
            re_posix_dump_r( a0 );
            re_posix_dump_r( a1 );
         }
      }
      return;
   }

   if( re_is_any( atom ) ) {
      (void)printf( "." );
      return;
   }

   int c = atom.x >> TAG_SHIFT;
   if( (c >= 32) && (c < 0x80) ) {
      (void)printf( "%c", c );
   } else {
      if( c ) (void)printf( "\\x%02x", c );
   }
}


void
re_posix_dump( atom_t atom )
{
   if( truep( atom ) /*|| brz_is_nullable( atom )*/ ) {
      (void)printf( "MATCH\n" );
   } else if( nullp( atom ) ) {
      (void)printf( "FAIL\n" );
   } else {
      (void)printf( "\"" );
      re_posix_dump_r( atom );
      (void)printf( "\"\n" );
   }
}


/*EoF*/
