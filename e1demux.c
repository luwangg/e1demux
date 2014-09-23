/** \file  e1demux.c
 *  \brief main for e1demux program. Parameters parsing and 
 *         top-level calls 
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "params.h"
#include "e1rcv.h"
#include "e1ts.h"

/** \brief parses command line options and then serialize 
 *         and parse e1 stream
 */
int main( int argc, char *argv[] )
{
  set_params_to_default( &gl_params );
  
  e1_rcv_t rx;
  memset( &rx, 0, sizeof( rx ) );

  e1_rcv_timeslots_clear();

  int err = parse_args( argc, argv, &gl_params );

  if ( !err ) {
    err = e1_rcv_timeslots_init();
    if ( err ) {
      exit( err );
    }

    while ( !feof( stdin ) ) {
      unsigned char b = 0;
      int ret = fread( &b, 1, 1, stdin );
      if ( ret == 1 ) {
          e1_rcv_next_byte ( &rx, b );            
      } else {
          exit (0);
      }
    }
  } else {
    exit( err );
  }

  return 0;
}

