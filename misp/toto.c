
#include <stdio.h>

int
main( int argc, char* argv[] )
{
   int i;

   for( i = 0 ; i < 10 ; ++i )
      (void)printf( "toto: %d\n", i );

   return 0;
}

