
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


#ifndef _EU_OPRS_LISP_H
#define _EU_OPRS_LISP_H


#define TAG_SHIFT 2
#define TAG_MASK  0x03

#define ATOM_INT(v)  ((atom_t)( (((unsigned)(v)) << TAG_SHIFT) | ATAG_INT  ))
#define ATOM_SYM(v)  ((atom_t)( (((unsigned)(v)) << TAG_SHIFT) | ATAG_SYM  ))
#define ATOM_CHAR(v) ((atom_t)( (((unsigned)(v)) << TAG_SHIFT) | ATAG_CHAR ))
#define PREDICATE(p) (p ? t : nil)

#define nil ((atom_t)0)
#define nao ATOM_SYM(0x1000000)  /* Not An Atom: sentinel value */
#define t   ATOM_SYM(0x1000001)


enum { ATAG_CONS, ATAG_INT, ATAG_SYM, ATAG_CHAR };

typedef struct cons_s cons_t;
typedef union  atom_u atom_t;

union atom_u {
   cons_t*  c;
   unsigned x;
   int      i;
};

struct cons_s {
   atom_t car;
   atom_t cdr;
};

static inline int nullp( atom_t atom )      { return !atom.x; }
static inline int truep( atom_t atom )      { return (atom.x == t.x); }
static inline int consp( atom_t atom )      { return (atom.x && ((atom.x & TAG_MASK) == ATAG_CONS)); }
static inline int symbolp( atom_t atom )    { return (atom.x & TAG_MASK) == ATAG_SYM;  }
static inline int integerp( atom_t atom )   { return (atom.x & TAG_MASK) == ATAG_INT;  }
static inline int characterp( atom_t atom ) { return (atom.x & TAG_MASK) == ATAG_CHAR; }
static inline int equalp( atom_t a0, atom_t a1 ) { return a0.x == a1.x; }

extern atom_t cons( atom_t car, atom_t cdr );
extern atom_t list( atom_t car, ... );

extern atom_t car( atom_t atom );
extern atom_t cdr( atom_t atom );
extern atom_t cadr( atom_t atom );
extern atom_t cddr( atom_t atom );
extern atom_t caddr( atom_t atom );

extern void dump( atom_t atom, const char* symtab[] );


#endif /*_EU_OPRS_LISP_H*/

/*EoF*/
