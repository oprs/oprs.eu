
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "regex_posix.h"


struct cmd_s {
   char *cmd;
   int  (*cfn)( int, char* [] );
};

static int cmd_equiv ( int argc, char* argv[] );
static int cmd_dump  ( int argc, char* argv[] );

struct cmd_s cv[] = {
   { "equiv", cmd_equiv },
   { "dump",  cmd_dump  },
   { 0,       0         }
};


static int
cmd_equiv( int argc, char* argv[] )
{
   if( argc < 3 ) {
      (void)fprintf( stderr, "equiv expects 2 arguments\n" );
      return 1;
   }

   re_mbuf_t m;

   m.x = argv[1];
   m.i = 0;
   atom_t a0 = re_posix_parse( &m );

   m.x = argv[2];
   m.i = 0;
   atom_t a1 = re_posix_parse( &m );

   return !re_is_equiv( a0, a1 );
}


static int
cmd_dump( int argc, char* argv[] )
{
   if( argc < 2 ) {
      (void)fprintf( stderr, "dump expects 1 argument\n" );
      return 1;
   }

   re_mbuf_t m = { argv[1], 0 };
   atom_t atom = re_posix_parse( &m );
   re_posix_dump( atom );

   return 0;
}


int
main( int argc, char *argv[] )
{
   int ci;

   if( argc < 2 ) {
      (void)fprintf( stderr, "usage: %s command [args...]\n", argv[0] );
      exit(1);
   }

   ci = 0;
   for( ci = 0 ; cv[ci].cmd ; ++ci )
      if( !strcmp( cv[ci].cmd, argv[1] ) ) break;

   if( !cv[ci].cmd ) {
      (void)fprintf( stderr, "unknown command \"%s\"\n", argv[1] );
      exit(1);
   }

   return cv[ci].cfn( argc-1, argv+1 );
}

