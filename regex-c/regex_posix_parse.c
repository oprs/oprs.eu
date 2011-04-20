
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


#include "regex_posix.h"


static atom_t
re_posix_closure( re_mbuf_t* mbuf, atom_t atom )
{
   int c = MBUF_PEEK( mbuf );

   switch( c ) {
      case '?': MBUF_SKIP( mbuf ); return re_posix_closure( mbuf, re_alt( atom, t )             );
      case '*': MBUF_SKIP( mbuf ); return re_posix_closure( mbuf, re_rep( atom )                );
      case '+': MBUF_SKIP( mbuf ); return re_posix_closure( mbuf, re_seq( atom, re_rep( atom )) );
      default : break;
   }

   return atom;
}


static atom_t
re_posix_quoted( re_mbuf_t* mbuf )
{
   atom_t atom;
   int c = MBUF_PULL( mbuf );

   switch( c ) {
      case '\0': return nil;
      case '\\': atom = ATOM_CHAR(  '\\'  ); break;  /* backslash */
      case '0' : atom = ATOM_CHAR( '\x00' ); break;  /* null      */
      case 'a' : atom = ATOM_CHAR( '\x07' ); break;  /* bell      */
      case 'b' : atom = ATOM_CHAR( '\x08' ); break;  /* backspace */
      case 't' : atom = ATOM_CHAR( '\x09' ); break;  /* tab       */
      case 'n' : atom = ATOM_CHAR( '\x0a' ); break;  /* newline   */
      case 'v' : atom = ATOM_CHAR( '\x0b' ); break;  /* vtab      */
      case 'f' : atom = ATOM_CHAR( '\x0c' ); break;  /* page      */
      case 'r' : atom = ATOM_CHAR( '\x0d' ); break;  /* return    */
      default  : atom = re_seq( ATOM_CHAR( '\\' ), ATOM_CHAR( c ) );
   }

   return atom;
}


static atom_t
re_posix_parse_r( re_mbuf_t* mbuf, int is_sub )
{
   atom_t atom = t;
   int c = MBUF_PEEK( mbuf );

   while( c ) {

      if( c == ')' ) {
         if( is_sub ) MBUF_SKIP( mbuf );
         break;
      }

      MBUF_SKIP( mbuf );

      switch( c ) {
         case '\\': atom = re_seq( atom, re_posix_closure( mbuf, re_posix_quoted( mbuf )     )); break;
         case '|' : atom = re_alt( atom, re_posix_closure( mbuf, re_posix_parse_r( mbuf, 0 ) )); break;
         case '(' : atom = re_seq( atom, re_posix_closure( mbuf, re_posix_parse_r( mbuf, 1 ) )); break;
         case '.' : atom = re_seq( atom, re_posix_closure( mbuf, RE_SYM_ANY                  )); break;
         default  : atom = re_seq( atom, re_posix_closure( mbuf, ATOM_CHAR(c)                )); break;
      }

      c = MBUF_PEEK( mbuf );
   }

   return atom;
}


atom_t
re_posix_parse( char* re )
{
   re_mbuf_t mbuf = { re, 0 };
   return re_posix_parse_r( &mbuf, 0 );
}


/*EoF*/
