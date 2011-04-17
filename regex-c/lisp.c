
/*
   Copyright (c) 2011, Olivier Piras <github-lisp@oprs.eu>
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


#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "lisp.h"


atom_t
cons( atom_t car, atom_t cdr )
{
   cons_t* c = (cons_t*)malloc( sizeof(cons_t) );
   c->car = car;
   c->cdr = cdr;

   return (atom_t)c;
}


static atom_t
list_r( atom_t car, va_list ap )
{
   atom_t cdr = va_arg( ap, atom_t );

   if( cdr.x == nao.x )
      return cons( car, nil );
   else
      return cons( car, list_r( cdr, ap ) );
}


atom_t
list( atom_t car, ... )
{
   va_list ap;

   va_start( ap, car );
   car = list_r( car, ap );
   va_end( ap );

   return car;
}


atom_t
car( atom_t atom )
{
   assert( consp(atom) );
   return atom.c ? atom.c->car : nil;
}


atom_t
cdr( atom_t atom )
{
   assert( consp(atom) );
   return atom.c ? atom.c->cdr : nil;
}


atom_t
cadr( atom_t atom )
{ return car( cdr( atom )); }


atom_t
cddr( atom_t atom )
{ return cdr( cdr( atom )); }


static void
dump_r( atom_t atom, const char* symtab[] )
{
   switch( atom.x & TAG_MASK ) {

      case ATYPE_CONS:
         if( nullp( atom ) ) break;

         if( consp( atom.c->car ) ) {
            (void)printf( "(" );
            dump_r( atom.c->car, symtab );
            (void)printf( ")" );
         } else {
            dump_r( atom.c->car, symtab );
         }

         if( !nullp(atom.c->cdr) ) (void)printf( " " );
         dump_r( atom.c->cdr, symtab );
         break;

      case ATYPE_INT:
         (void)printf( "%d", atom.x >> TAG_SHIFT );
         break;

      case ATYPE_SYM:
      {
         unsigned sym = atom.x >> TAG_SHIFT;

         switch( sym ) {

            case 0x1000000: (void)printf( "NaO" ); break;
            case 0x1000001: (void)printf( "T"   ); break;

            default:
               if( symtab )
                  (void)printf( "%s", symtab[sym] );
               else
                  (void)printf( "sym#%d", sym );
         }

         break;
      }

      case ATYPE_CHAR:
//         (void)printf( "#\\%c", atom.x >> TAG_SHIFT );
         (void)printf( "'%c'", atom.x >> TAG_SHIFT );
         break;

      default:
         (void)fprintf( stderr, "unknown atom type\n" );

   }
}


void
dump( atom_t atom, const char* symtab[] )
{
   if( (atom.x & TAG_MASK) == ATYPE_CONS ) {
      (void)printf( "(" );
      dump_r( atom, symtab );
      (void)printf( ")\n" );
   } else {
      dump_r( atom, symtab );
      (void)printf( "\n" );
   }
}

/*EoF*/
