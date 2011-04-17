
#ifndef _EU_OPRS_REGEX_H
#define _EU_OPRS_REGEX_H

#include "lisp.h"


#define SEQ( e0, e1 ) re_seq( e0, e1 )
#define ALT( e0, e1 ) re_alt( e0, e1 )
#define REP( e0 )     re_rep( e0 )
#define SYM( e0 )     re_sym( e0 )

#define RE_TYPE_SEQ ATOM_SYM(0)
#define RE_TYPE_ALT ATOM_SYM(1)
#define RE_TYPE_REP ATOM_SYM(2)
#define RE_TYPE_ANY ATOM_SYM(3)


struct re_mbuf {
   const char *x;
   int         i;
};

typedef struct re_mbuf re_mbuf_t;

#define MBUF_PULL( m ) ((m)->x[((m)->i)++])
#define MBUF_PEEK( m ) ((m)->x[ (m)->i   ])

extern atom_t re_seq( atom_t latom, atom_t ratom );
extern atom_t re_alt( atom_t latom, atom_t ratom );
extern atom_t re_rep( atom_t latom );
extern atom_t re_sym( int sym );

extern atom_t re_closure( re_mbuf_t* mbuf, atom_t e );

extern atom_t re_expr( re_mbuf_t* mbuf );


#endif /*_EU_OPRS_REGEX_H*/

/*EoF*/
