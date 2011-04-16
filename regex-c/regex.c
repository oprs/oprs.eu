
#include "regex.h"

#define cons se_atom_new


se_elem_t
re_seq( se_elem_t e0, se_elem_t e1 )
{ return cons( RE_TYPE_SEQ, cons( e0, cons( e1, SE_NIL ))); }


se_elem_t
re_alt( se_elem_t e0, se_elem_t e1 )
{ return cons( RE_TYPE_ALT, cons( e0, cons( e1, SE_NIL ))); }


se_elem_t
re_rep( se_elem_t e )
{ return cons( RE_TYPE_REP, cons( e, SE_NIL )); }


se_elem_t
re_sym( int sym )
{ return SE_SYM(sym); }


se_elem_t
re_expr( const char* re )
{
   se_elem_t e;

   if( !re || !re[0] )
      return SE_NIL;

   e = cons( RE_TYPE_SEQ, cons( re_sym(re[0]), SE_NIL ) );

   return e;
}

/*EoF*/
