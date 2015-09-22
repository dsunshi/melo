/*
 * Copyright (c) 2015 David Sunshine, <http://sunshin.es>
 * 
 * This file is part of melo.
 * 
 * melo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * melo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with melo.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MELO_PRIV_H_
#define __MELO_PRIV_H_

/******************************************************************************
*                                   Includes                                  *
******************************************************************************/
#include "melo.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
*                             Private Data Types                              *
******************************************************************************/

typedef union
{
	struct
	{
		unsigned subfunction : 3;
		unsigned service     : 2;
		unsigned reserved    : 1;
		unsigned status      : 2;
	} fields;
	uint8_t raw_byte;
} _m_command;

typedef struct
{
	_m_command command;
    MeloList   data;
    uint8_t    byte_order;
} _m_packet;

typedef bool (*_m_service)(const _m_packet * const request, _m_packet * const response);

typedef struct
{
	_m_packet packet;
    uint8_t   crc;
} _m_frame;

typedef struct
{
	_m_frame frame;
    MeloList buffer;
    uint8_t  escape_buffer;
    bool     crc_present;
} _m_frame_buffer;

#define MELO_CMD_REQUEST_RESPONSE      0u
#define MELO_CMD_POSITIVE_RESPONSE     1u
#define MELO_CMD_NEGATIVE_RESPONSE     2u
#define MELO_CMD_PENDING_RESPONSE      3u

#define MELO_PACKET_SIZE               2u
#define MELO_FRAME_SIZE                3u
#define MELO_WAIT_DATA_SIZE            1u
#define MELO_MAX_PACKET_SIZE           (MELO_CFG_MAX_DATA_LENGTH + MELO_PACKET_SIZE)
#define MELO_MAX_FRAME_SIZE            (MELO_MAX_PACKET_SIZE     + MELO_FRAME_SIZE)
#define MELO_MAX_WAIT_FRAME_SIZE       (MELO_PACKET_SIZE + MELO_FRAME_SIZE + MELO_WAIT_DATA_SIZE)

#define MELO_CMD_HEAD                  0u
#define MELO_CMD_TAIL                  1u

#define MELO_EVENT_IDLE                0u
#define MELO_EVENT_REQUEST_RECEIVED    1u
#define MELO_EVNET_TX_CONFIRMATION     2u

#define BIT_MASK(n)                    ( ((uint8_t) 1u) << ((uint8_t) (n)) )
#define IS_BIT_SET(b,p)                ( ( ((b) & BIT_MASK((p))) != 0 ) ? true : false )
#define IS_BIT_CLEAR(b,p)              ( ( ((b) & BIT_MASK((p))) == 0 ) ? true : false )
#define BIT_SET(p,n)                   ((p) |=  ( ((uint8_t) 1u) << ((uint8_t) (n)) ))
#define BIT_CLEAR(p,n)                 ((p) &= ~( ((uint8_t) 1u) << ((uint8_t) (n)) ))

#define FRAME_ENDIAN_BIT_POS           7u
#define FRAME_ESCAPE_BIT_POS           6u
#define FRAME_RESERVED_BIT_POS         5u
#define FRAME_CRC_BIT_POS              4u
#define FRAME_MARKER_BIT_POS           3u

#define RESERVED_BIT_MASK              ( BIT_MASK(FRAME_RESERVED_BIT_POS)           )
#define RESERVED_LOW_MASK              ( RESERVED_BIT_MASK - 1u                     )
#define RESERVED_TX_HIGH_MASK          ( (~RESERVED_LOW_MASK) & 0x7Fu               )
#define RESERVED_RX_HIGH_MASK          ( (~RESERVED_LOW_MASK) ^ RESERVED_BIT_MASK   )

#define IS_FRAME_CONTROL(b)            (  IS_BIT_SET((b),   FRAME_RESERVED_BIT_POS) )
#define IS_FRAME_HEAD(b)               (  IS_BIT_SET((b),   FRAME_MARKER_BIT_POS)   )
#define IS_FRAME_TAIL(b)               (  IS_BIT_CLEAR((b), FRAME_MARKER_BIT_POS)   )
#define IS_FRAME_CRC_PRESENT(b)        (  IS_BIT_SET((b),   FRAME_CRC_BIT_POS)      )
#define IS_FRAME_ESCAPED(b)            (  IS_BIT_SET((b),   FRAME_ESCAPE_BIT_POS)   )
#define GET_FRAME_ENDIANNESS(b)        ( (IS_BIT_SET((b),   FRAME_ENDIAN_BIT_POS) != false) ?  MELO_BIG_ENDIAN : MELO_LITTLE_ENDIAN  )

#define NUM_ESCAPE_BYTES               5u
#define ESCAPE_BYTE_MASK               ( BIT_MASK(NUM_ESCAPE_BYTES) - 1u)
#define ESCAPE_BYTE                    ( BIT_MASK(FRAME_ESCAPE_BIT_POS) | BIT_MASK(FRAME_RESERVED_BIT_POS) )

#define RX_RECEIVED                    0u
#define TX_CONFIRMATION                1u
#define TX_REQUEST                     2u
#define EVENT_DO_NOTHING               3u
#define NUM_EVENTS                     4u

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
