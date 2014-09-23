/** \file  e1ts.c
 *  \brief transferring timeslot data to real world (Files)
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "params.h"
#include "e1ts.h"

static e1_rcv_timeslot_t gl_e1_rcv_ts[ E1_TS_NUM ]; 

void e1_rcv_timeslots_clear( void )
{
  memset( gl_e1_rcv_ts, 0, sizeof( gl_e1_rcv_ts  ) );
}

/** initializes timeslots, opens all required files. 
 *  should be called after options parsing.
 *  
 *  If fname is NULL, timeslot is set up for sending the silence byte.
 *  If fname is non-zero, timeslot is set up for sending data from the file.
 */
int e1_rcv_timeslots_init( void )
{
  unsigned i = 0;
  // opening all files, if any
  for ( i = 0; i < E1_TS_NUM; i++ ) { 
    e1_rcv_timeslot_t *t = &gl_e1_rcv_ts[ i ];
    t->num = i;

    if ( t->fname != NULL ) {
      if ( t->file == NULL ) {
        // file name is set but the file is not opened yet
        t->file = fopen( t->fname, "w" );
        if ( t->file == NULL ) {
          perror( t->fname );
          return errno; 
        }

        // lookup other timeslots with the same fname.
        // and set 'file' to t->file value.
        // this is done for consequtive reading of same file
        // when it is used for some timeslots (more then one)
        unsigned j = 0;
        for ( j = i + 1; j < E1_TS_NUM; j ++ ) {
          e1_rcv_timeslot_t *_t = &gl_e1_rcv_ts[ j ];
          if ( _t->fname && ( strcmp( _t->fname, t->fname ) == 0 ) ) {
            _t->file = t->file;
          }
        }
      }
    }
  }

  return 0;
}

void e1_rcv_timeslots_close( void )
{
  unsigned i = 0;
  for ( i = 0; i < E1_TS_NUM; i ++ ) {
    if ( gl_e1_rcv_ts[ i ].file ) {
      fclose( gl_e1_rcv_ts[ i ].file );
    }
  }
}

/** \brief Associates a file with specified timeslot.
 */
void e1_demux_set_file_for_timeslot( unsigned char ts, const char *fname )
{
  assert( ts < E1_TS_NUM );

#ifdef DEBUG
  fprintf( stderr, "%s: ts%u = %s\n", __FUNCTION__, ts, fname );
#endif
  gl_e1_rcv_ts[ ts ].fname = (const char*) strdup( fname );
}

void e1_rcv_write_ts_data( unsigned char ts, unsigned char b )
{
  assert( ts < E1_TS_NUM );

  FILE *f = gl_e1_rcv_ts[ ts ].file;

  if ( f ) {
    int ret = fwrite( &b, 1, 1, f );
    if ( ret != 1 ) {
      fprintf( stderr, "fwrite() to '%s' failed (err=%d)\n",
               gl_e1_rcv_ts[ ts ].fname, ret );
      gl_e1_rcv_ts[ ts ].file = NULL;
      gl_e1_rcv_ts[ ts ].fname = NULL;
    }
  }
}
