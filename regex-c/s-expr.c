
#include <stdio.h>
#include <stdlib.h>

#include "s-expr.h"


se_atom_t se_atom_nil = {
   .car.x = 0,
   .cdr.x = 0
};


se_elem_t
se_atom_new( se_elem_t car, se_elem_t cdr )
{
   se_atom_t* atom = (se_atom_t*)malloc( sizeof(se_atom_t) );
   atom->car = car;
   atom->cdr = cdr;

   return (se_elem_t)atom;
}


void
se_atom_free( se_atom_t* atom )
{
   if( atom ) {
      if( atom->car.sym.tag == SE_TAG_PTR ) se_atom_free( atom->car.atom );
      if( atom->cdr.sym.tag == SE_TAG_PTR ) se_atom_free( atom->cdr.atom );
      (void)free( atom );
   }
}


static void
se_elem_dump_r( se_elem_t elem )
{
   switch( elem.x & 0x03 )
   {
      case 0:
         if( elem.atom ) {
            (void)printf( "( " );
            se_elem_dump_r( elem.atom->car );
            (void)printf( ") " );
            se_elem_dump_r( elem.atom->cdr );
         }
         break;

      default:
         (void)printf( "%u ", elem.sym.val );
   }
}


void
se_elem_dump( se_elem_t elem )
{
   se_elem_dump_r( elem );
   (void)printf( "\n" );
}

/*EoF*/
