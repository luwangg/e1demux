#ifndef E1RCV__H
#define E1RCV__H

/** \file e1rcv.v
 *  
 * \brief E1 receiving functions and structures 
 */

#include "frmsyn.h"

struct e1_rcv {
  unsigned frame;          /**< frame counter */
  unsigned char prev_byte; /**< previous received byte */
  unsigned char byte;      /**< current received byte */
  unsigned char boff;      /**< bit offset of timeslot data in 'byte'
                                and 'prev_byte' */
  e1_rcv_frmsyn_t frmsyn;  /**< \sa struct e1_rcv_frmsyn */
};

typedef struct e1_rcv e1_rcv_t;

unsigned char e1_rcv_aligned_byte( e1_rcv_t *rx );
void e1_rcv_next_byte( e1_rcv_t *rx, unsigned char b );

#endif //E1RCV__H

