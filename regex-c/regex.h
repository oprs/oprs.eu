
#ifndef _CLEP_REGEX_H
#define _CLEP_REGEX_H

#include "s-expr.h"


#define SEQ( e0, e1 ) re_seq( e0, e1 )
#define ALT( e0, e1 ) re_alt( e0, e1 )
#define REP( e0 )     re_rep( e0 )
#define SYM( e0 )     re_sym( e0 )

#define RE_TYPE_SEQ  SE_SYM(0x100)
#define RE_TYPE_ALT  SE_SYM(0x101)
#define RE_TYPE_REP  SE_SYM(0x102)


extern se_elem_t re_seq( se_elem_t latom, se_elem_t ratom );
extern se_elem_t re_alt( se_elem_t latom, se_elem_t ratom );
extern se_elem_t re_rep( se_elem_t latom );
extern se_elem_t re_sym( int sym );

extern se_elem_t re_expr( const char* re );


#endif /*_CLEP_REGEX_H*/

/*EoF*/
