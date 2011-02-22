
/* very simple disassembler for MIPS ELF objects */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

/*

  I-Type:

   31       26 25     21 20     16 15                            0
  +-----------+---------+---------+-------------------------------+
  |  opcode   |    rs   |    rt   |           immediate           |
  +-----------+---------+---------+-------------------------------+
              |   base  |   hint  |            offset             |
              +---------+---------+-------------------------------+
                        |    ft   |
                        +---------+

  J-Type:

   31       26 25                                                0
  +-----------+---------------------------------------------------+
  |  opcode   |                   instr_index                     |
  +-----------+---------------------------------------------------+

  R-Type:

   31       26 25     21 20     16 15     11 10      6 5         0
  +-----------+---------+---------+---------+---------+-----------+
  |  opcode   |    rs   |    rt   |    rd   |    sa   | function  |
  +-----------+---------+---------+---------+---------+-----------+
                                            |  stype  |
                                            +---------+

  @a: sa               @b: base
  @c: cache_op         @d: rd
  @f: ft               @h: hint
  @i: immediate (dec)  @n: instr_index
  @o: offset           @s: rs
  @t: rt               @w: word (instruction code)
  @x: immediate (hex)  @y: stype

 */

static const char *opcode[64] = {
   "(special #x@w)",   "(regimm #x@w)",    "(j @n)",           "(jal @n)",
   "(beq @s @t @o)",   "(bne @s @t @o)",   "(blez @s @o)",     "(bgtz @s @o)",
   "(addi @t @s @i)",  "(addiu @t @s @i)", "(slti @t @s @i)",  "(sltiu @t @s @i)",
   "(andi @t @s @i)",  "(ori @t @s @i)",   "(xori @t @s @i)",  "(lui @t @x)",

   "(cop0 #x@w)",      "(cop1 #x@w)",      "(cop2 #x@w)",      "(cop1x #x@w)",
   "(beql @s @t @o)",  "(bnel @s @t @o)",  "(blezl @s @o)",    "(bgtlz @s @o)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",
   "(special2 #x@w)",  "(jalx @n)",        "(xxx #x@w)",       "(special3 #x@w)",

   "(lb @t @o @b)",    "(lh @t @o @b)",    "(lwl @t @o @b)",   "(lw @t @o @b)",
   "(lbu @t @o @b)",   "(lhu @t @o @b)",   "(lwr @t @o @b)",   "(xxx #x@w)",
   "(sb @t @o @b)",    "(sh @t @o @b)",    "(swl @t @o @b)",   "(sw @t @o @b)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(swr @t @o @b)",   "(cache @c @o @b)",

   "(ll @t @o @b)",    "(lwc1 @f @o @b)",  "(lwc2 @t @o @b)",  "(pref @h @o @b)",
   "(xxx #x@w)",       "(ldc1 @f @o @b)",  "(ldc2 @t @o @b)",  "(xxx #x@w)",
   "(sc @t @o @b)",    "(swc1 @f @o @b)",  "(swc2 @t @o @b)",  "(xxx #x@w)",
   "(xxx #x@w)",       "(sdc1 @f @o @b)",  "(sdc2 @t @o @b)",  "(xxx #x@w)"
};

static const char *special[64] = {
   "(sll @d @t @a)",   "(movci #x@w)",     "(srl @d @t @a)",   "(sra @d @t @a)",
   "(sllv @d @t @s)",  "(xxx #x@w)",       "(srlv @d @t @s)",  "(srav @d @t @s)",
   "(jr @s)",          "(jalr @d @s)",     "(movz @d @s @t)",  "(movn @d @s @t)",
   "(syscall)",        "(break)",          "(xxx #x@w)",       "(sync @y)",

   "(mfhi @d)",        "(mthi @s)",        "(mflo @d)",        "(mtlo @s)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",
   "(mult @s @t)",     "(multu @s @t)",    "(div @s @t)",      "(divu @s @t)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",

   "(add @d @s @t)",   "(addu @d @s @t)",  "(sub @d @s @t)",   "(subu @d @s @t)",
   "(and @d @s @t)",   "(or @d @s @t)",    "(xor @d @s @t)",   "(nor @d @s @t)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(slt @d @s @t)",   "(sltu @d @s @t)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",

   "(tge @s @t)",      "(tgeu @s @t)",     "(tlt @s @t)",      "(tltu @s @t)",
   "(teq @s @t)",      "(xxx #x@w)",       "(tne @s @t)",      "(xxx #x@w)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",
   "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)",       "(xxx #x@w)"
};

static const char *reg[32] = {
   "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
   "t0",   "t1", "t2", "t3", "t4", "t5", "t6", "t7",
   "s0",   "s1", "s2", "s3", "s4", "s5", "s6", "s7",
   "t8",   "t9", "k0", "k1", "gp", "sp", "s8", "ra"
};


static const char *stype[16] = {
   "NOTYPE",  "OBJECT",  "FUNCTION", "SECTION",
   "FILE",    "UNKNOWN", "UNKNOWN",  "UNKNOWN",
   "UNKNOWN", "UNKNOWN", "UNKNOWN",  "UNKNOWN",
   "UNKNOWN", "LOPROC",  "UNKNOWN",  "HIPROC"
};

static const char *btype[16] = {
   "LOCAL",   "GLOBAL",  "WEAK",    "UNKNOWN",
   "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN",
   "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN",
   "UNKNOWN", "LOPROC",  "UNKNOWN", "HIPROC"
};


static char *
mips_format( const char *x, unsigned w )
{
   static char str[32];
   int i;

   i = 0;
   while( *x ) {
      if( *x == '@' ) {
         ++x;
         switch( *x ) {

            case 'a': i += sprintf( &str[i], "%d", (w>>6)&0x1f ); break;
            case 'd': i += sprintf( &str[i], "%s", reg[(w>>11)&0x1f] ); break;
            case 'i': i += sprintf( &str[i], "%d", (short)(w&0xffff) ); break;
            case 'x': i += sprintf( &str[i], "#x%x", (short)(w&0xffff) ); break;
            case 'o': i += sprintf( &str[i], "%d", (short)(w&0xffff) ); break;
//            case 'O': i += sprintf( &str[i], "#x%x", (short)((w&0xffff)<<2)+4 ); break;
            case 'b':
            case 's': i += sprintf( &str[i], "%s", reg[(w>>21)&0x1f] ); break;
            case 't': i += sprintf( &str[i], "%s", reg[(w>>16)&0x1f] ); break;
            case 'w': i += sprintf( &str[i], "%x", w ); break;
            case '@': str[i++] = '@'; break;

            default:
               str[i++] = '@';
               str[i++] = *x;
               break;
         }
         ++x;
      } else
         str[i++] = *(x++);
   }

   str[i] = '\0';
   return str;
}


static void
mips_disassemble( unsigned char *x, unsigned len )
{
   unsigned w, i;
   int op, sp;

   (void)printf( "\n  (code" );
   for( i = 0 ; i < len ; i += 4 ) {
      w  = *((unsigned*)(x+i));
      op = (w>>26) & 0x3f;

      (void)printf( "\n    " );
      switch( op ) {
         case 0x00:
            if( w == 0 ) {
               (void)printf( "(nop)" );
            } else {
               sp = w & 0x3f;
               switch( sp ) {

               case 0x21: /* addu */
                  if( ((w>>16) & 0x1f) == 0 ) {
                     (void)printf( "%s", mips_format( "(move @d @s)", w ));
                     break;
                  }
                  /*FALLTHROUGH*/

               default:
                  (void)printf( "%s", mips_format( special[sp], w ));
                  break;
               }
            }
            break;

         case 0x04: /* beq */
            if( ((w>>16)&0x3ff) == 0 ) {
               (void)printf( "%s", mips_format( "(b @o)", w ));
               break;
            }
            /*FALLTHROUGH*/

         default:
            (void)printf( "%s", mips_format( opcode[op], w ));
            break;
      }
   }
   (void)printf( ")\n" );
}


static void
symtab_dump( unsigned char *x, unsigned len, unsigned char *strt )
{
   Elf32_Sym *sym;
   unsigned off = 0;
   int i;

   for( i = 0 ; off < len ; ++i ) {
      sym = (Elf32_Sym*)(x+off);
      (void)printf( "  % 3i: %s %s %08x %u %u \"%s\"\n",
                     i,
                     btype[ELF32_ST_BIND(sym->st_info)],
                     stype[ELF32_ST_TYPE(sym->st_info)],
                     sym->st_value,
                     sym->st_size,
                     sym->st_shndx,
                     strt+sym->st_name
                  );
      off += sizeof( Elf32_Sym );
   }
}


int
main( int argc, char *argv[] )
{
   int fd, i;
   unsigned char *x;
   struct stat st;
   Elf32_Ehdr *ehdr;
   Elf32_Shdr *shdr;

   if( argc != 2 ) {
      (void)fprintf( stderr, "usage: %s file\n", argv[0] );
      exit(1);
   }

   fd = open( argv[1], O_RDONLY, 0);
   if( fd < 0 ) {
      (void)perror( argv[1] );
      exit(1);
   }

   if( fstat( fd, &st ) < 0 ) {
      (void)perror( argv[1] );
      (void)close( fd );
      exit(1);
   }

   x = mmap( 0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
   if( x == MAP_FAILED ) {
      (void)perror( argv[1] );
      (void)close( fd );
      exit(1);
   }

   ehdr = (Elf32_Ehdr*)x;

   (void)printf( "  e_ident: [%.*s]\n", 16, ehdr->e_ident   );
   (void)printf( "   e_type: %u\n",         ehdr->e_type    );
   (void)printf( "e_machine: %u\n",         ehdr->e_machine );
   (void)printf( "\n" );

   shdr = (Elf32_Shdr*)(x+ehdr->e_shoff);
   for( i = ehdr->e_shnum ; i > 0 ; --i ) {

      switch( shdr->sh_type ) {

         case SHT_PROGBITS:
            if( shdr->sh_flags & SHF_EXECINSTR )
               mips_disassemble( x+shdr->sh_offset, shdr->sh_size );
            break;

         case SHT_SYMTAB:
            {
               (void)printf( "\nSYMBOLS:\n" );
               Elf32_Shdr *strt = (Elf32_Shdr*)(x + ehdr->e_shoff + shdr->sh_link*sizeof(Elf32_Shdr));
               symtab_dump( x+shdr->sh_offset, shdr->sh_size, x+strt->sh_offset );
            }
            break;

         default:
            (void)printf( "sh_type: [%08x], sh_flags: %08x, sh_offset: %08x +%u\n",
                          shdr->sh_type, shdr->sh_flags, shdr->sh_offset, shdr->sh_size );
            break;

      }
      ++shdr;
   }

   (void)munmap( x, st.st_size );
   (void)close( fd );

   return 0;
}

/*EoF*/
