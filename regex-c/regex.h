
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


#ifndef _EU_OPRS_REGEX_H
#define _EU_OPRS_REGEX_H

#include "ulisp.h"


#define RE_SYM_SEQ ULISP_USYM(0)
#define RE_SYM_ALT ULISP_USYM(1)
#define RE_SYM_REP ULISP_USYM(2)
#define RE_SYM_ANY ULISP_USYM(3)


struct re_mbuf {
   const char *x;
   int         i;
};

typedef struct re_mbuf re_mbuf_t;

#define MBUF_PULL( m ) ((m)->x[((m)->i)++])
#define MBUF_PEEK( m ) ((m)->x[ (m)->i   ])
#define MBUF_SKIP( m ) (void)( ((m)->i)++ )

static inline int re_is_eps( atom_t atom ) { return equalp( atom, t ); }
static inline int re_is_any( atom_t atom ) { return equalp( atom, RE_SYM_ANY ); }
static inline int re_is_alt( atom_t atom ) { return consp(atom) && equalp( car(atom), RE_SYM_ALT ); }
static inline int re_is_seq( atom_t atom ) { return consp(atom) && equalp( car(atom), RE_SYM_SEQ ); }
static inline int re_is_rep( atom_t atom ) { return consp(atom) && equalp( car(atom), RE_SYM_REP ); }

extern int re_is_equiv( atom_t a0, atom_t a1 );

extern atom_t re_seq( atom_t latom, atom_t ratom );
extern atom_t re_alt( atom_t latom, atom_t ratom );
extern atom_t re_rep( atom_t latom );

extern void re_dump( atom_t atom );


#endif /*_EU_OPRS_REGEX_H*/

/*EoF*/
