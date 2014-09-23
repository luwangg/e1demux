#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "frmsyn.h"
#include "e1.h"

/** \brief Increment timeslot number and frame counter.
 *  \return incremented number in range 0..31
 */
unsigned char e1_rcv_frmsyn_inc_ts( e1_rcv_frmsyn_t * fsyn ) 
{
  fsyn->ts = ( ++ fsyn->ts ) & E1_TS_MASK;
  if ( fsyn->ts == 0 ) 
    fsyn->frame ++;

  return fsyn->ts;
}

void e1_rcv_frmsyn_set_state( e1_rcv_frmsyn_t *fsyn, 
                              e1_rcv_frmsyn_fsm_t state )
{
  if ( state == SYNC_OFF ) {
  } else if ( state == SYNC_TRYING ) { 
    fsyn->ts = 0;
    fsyn->frame = 0;
    fsyn->nfas_cnt = 0;
    fsyn->fas_cnt = 0;
  }
  fsyn->fsm = state;
}

/** Checks the state of frame alignment and changes fsm state
 *  if possible.
 */
void e1_rcv_check_frame_alignment( e1_rcv_frmsyn_t *fsyn )
{ 
  if ( fsyn->frame == E1_RCV_SYNC_CONSEQ_FRAMES ) {
    char sync = ( E1_RCV_SYNC_CONSEQ_FRAMES + 1 ) 
                   == ( fsyn->fas_cnt + fsyn->nfas_cnt );
    if ( sync ) {
      e1_rcv_frmsyn_set_state( fsyn, SYNC_DOUBLEFRAME );
    } else {
      e1_rcv_frmsyn_set_state( fsyn, SYNC_OFF );
    }
  } 
}

/** \brief Finds synchronization pattern (FAS/NFAS pair).
 *  
 *         Synchronization is set if FAS was received 3 times
 *         consequently.
 *         Synchronization is lost if 2 consecutive FAS words
 *         was received incorrectly.
 * \param b - byte aligned value of received byte
 * \param ts - number of timeslot which 'b' corresponds
 */
void e1_rcv_aligned_frame( e1_rcv_frmsyn_t *fsyn, unsigned char b,
                           unsigned char *ts )
{
  if ( fsyn->fsm == SYNC_TRYING ) {
    if ( fsyn->ts == 0 ) {
      if ( fsyn->frame & 0x1 ) { // odd frame -> NFAS 
        if ( ( b & E1_TS0_NFAS_WORD ) == E1_TS0_NFAS_WORD ) {
          fsyn->nfas_cnt ++;
        }
      } else { // even frame -> FAS 
        if ( ( b & E1_TS0_FAS_WORD ) == E1_TS0_FAS_WORD ) {
          fsyn->fas_cnt ++;
        }
      }
    
      e1_rcv_check_frame_alignment( fsyn );
    }
  }

  *ts = fsyn->ts;
  e1_rcv_frmsyn_inc_ts( fsyn ); 
}

