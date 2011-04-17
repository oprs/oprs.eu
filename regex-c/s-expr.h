
#ifndef _EU_OPRS_S_EXPR_H
#define _EU_OPRS_S_EXPR_H


enum { SE_TAG_PTR, SE_TAG_SYM };

typedef struct se_atom se_atom_t;
typedef union  se_elem se_elem_t;

union se_elem {
   se_atom_t* atom;
#if __BYTE_ORDER == __LITTLE_ENDIAN
   struct { int tag:2;  int val:30; } sym;
#elif __BYTE_ORDER == __BIG_ENDIAN
   struct { int val:30; int tag:2;  } sym;
#endif
   unsigned x;
};

struct se_atom {
   se_elem_t car;
   se_elem_t cdr;
};

extern se_atom_t se_atom_nil;

#define SE_SYM(I) (se_elem_t)(((unsigned)I<<2)|SE_TAG_SYM)
#define SE_NIL    (se_elem_t)&se_atom_nil
#define SE_T      SE_SYM(-1)

extern se_elem_t se_atom_new  ( se_elem_t car, se_elem_t cdr );
extern void      se_atom_free ( se_atom_t* atom );
extern void      se_elem_dump ( se_elem_t elem );


#endif /*_EU_OPRS_S_EXPR_H*/

/*EoF*/
