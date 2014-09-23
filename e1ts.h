#ifndef E1TS__H
#define E1TS__H 

/** \file e1ts.h
 *  \brief Types and functions for timeslot data writing
 */
#include <stdio.h>

typedef struct e1_rcv_timeslot {
  unsigned char num; /**< number of timeslot = 0..31 */
  const char *fname; /**< filename for reading data for timeslot */
  FILE *file; /**< file for writing timeslot data */
} e1_rcv_timeslot_t;

void e1_rcv_timeslots_clear( void );
int e1_rcv_timeslots_init( void );
void e1_rcv_set_file_for_timeslot( unsigned char ts, const char *fname );
void e1_rcv_write_ts_data( unsigned char ts, unsigned char b );

#endif // E1TS__H

