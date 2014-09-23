#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "e1.h"
#include "e1rcv.h"
#include "e1ts.h"

/** \brief Analyzes byte and previous byte and tries to 
 *         find FAS word in it. Tries 8 shifts ( from 0 to 7 bits).
 *         If FAS word is found then state machine goes to TRYING state.
 *  \return 'b' if SYNC is not achieved and aligned byte otherwise.
 *          next functions should not think about shifting of received
 *          bytes.
 */
unsigned char e1_rcv_aligned_byte( e1_rcv_t *rx )
{
  unsigned short word = rx->byte + ((unsigned short)rx->prev_byte << 8); 
  if ( rx->frmsyn.fsm == SYNC_OFF ) {
    const unsigned short fas_masks[ 8 ] = {
      ( (unsigned short) E1_TS0_FAS_WORD ) << 0,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 1,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 2,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 3,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 4,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 5,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 6,
      ( (unsigned short) E1_TS0_FAS_WORD ) << 7
    };  
 
    unsigned i = 0;
    for ( i = 0; i < 8; i ++ ) {
      if ( ( word & fas_masks[ i ] ) == fas_masks[ i ] ) {
        rx->boff = i;
        e1_rcv_frmsyn_set_state( &rx->frmsyn, SYNC_TRYING );
        break;
      }
    }
  }
	
	// return aligned byte
	return ( unsigned char )( word >> rx->boff );
}

/** \brief process next received byte, received from 
 *         de-serializator
 *  
 *        Processing contains some stages
 *          - byte aligning 
 *          - frame synchronization (acc. to ITU-T G.704)
 *          - timeslot data dispatching (only if frame synchronization 
 *            is achieved)
 */
void e1_rcv_next_byte( e1_rcv_t *rx, unsigned char b )
{
  rx->prev_byte = rx->byte;
  rx->byte = b;

  // 1. byte alignment
  unsigned char al_b = e1_rcv_aligned_byte( rx ); 
 
  // 2. frame aligment
  unsigned char ts = 0; 
  
  // assign timeslot for byte
  e1_rcv_aligned_frame( &rx->frmsyn, al_b, &ts ); 

  printf( "sync = %d\n", rx->frmsyn.fsm & SYNC_DOUBLEFRAME ? 1 : 0 );
  if ( rx->frmsyn.fsm & SYNC_DOUBLEFRAME ) {
    // at least double frame synchronization available
    printf( "ts%02u %02x\n", ts, al_b );
    e1_rcv_write_ts_data( ts, al_b );
  } 
}

