#ifndef FRMSYN__H
#define FRMSYN__H

/** \file frmsyn.h
 *
 * \brief E1 frame synchronization accordingly with ITU-T G.704.
 *        Only double-frame (non-multi-frame) synchronization is supported.
 *        I.e. bit 7 of 0th timeslot is not analyzed.
 */

// To establish stream synchronization
// N frames consequently should contain NFAS/FAS words
#define E1_RCV_SYNC_CONSEQ_FRAMES  10

// can be used as bitmask
enum e1_rcv_frmsyn_fsm {
  SYNC_OFF         = 0,
  SYNC_TRYING      = 1,
  SYNC_DOUBLEFRAME = 2,
  SYNC_MULTIFRAME  = 4 + SYNC_DOUBLEFRAME 
};

typedef enum e1_rcv_frmsyn_fsm e1_rcv_frmsyn_fsm_t;

struct e1_rcv_frmsyn {
  e1_rcv_frmsyn_fsm_t fsm; /**< frame synchronization state */ 
  unsigned ts;             /**< timeslot counter 0..31 */
  unsigned frame;          /**< frame counter 0.. */
  unsigned char nfas_cnt;  /**< number of received NFAS bytes */
  unsigned char fas_cnt;   /**< number of received FAS bytes */
};

typedef struct e1_rcv_frmsyn e1_rcv_frmsyn_t;


unsigned char e1_rcv_frmsyn_inc_ts( e1_rcv_frmsyn_t * fsyn ) ;
void e1_rcv_check_frame_alignment( e1_rcv_frmsyn_t *fsyn );
void e1_rcv_frmsyn_set_state( e1_rcv_frmsyn_t *fsyn,
                              e1_rcv_frmsyn_fsm_t state );
void e1_rcv_aligned_frame( e1_rcv_frmsyn_t *rx, unsigned char b,
                           unsigned char *ts );

#endif // FRMSYN__H

