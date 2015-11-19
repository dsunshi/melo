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

#ifndef __MELO_H_
#define __MELO_H_

/******************************************************************************
*                                   Includes                                  *
******************************************************************************/
#include "melo_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
*                             Exported Data Types                             *
******************************************************************************/
#if !defined(UINT8_MAX) && !defined(__UINT8_TYPE__)
    typedef unsigned char  uint8_t;
    #define UINT8_MAX      0xFFu
#endif

#if !defined(UINT16_MAX) && !defined(__UINT16_TYPE__)
    typedef unsigned short uint16_t;
    #define UINT16_MAX     0xFFFFu
#endif

#if !defined(UINT32_MAX) && !defined(__UINT32_TYPE__)
    typedef unsigned int   uint32_t;
    #define UINT32_MAX     0xFFFFFFFFul
#endif

#if (__bool_true_false_are_defined == 1)
    /* Use C99 built-in bool*/
#else
  #if defined(_WIN32) && !defined(__MINGW32__)
    /* MS VS only supports C89 C, so define our own bool */
    #pragma once

    #define false   0u
    #define true    1u
    #define bool    int
  #else
    #ifdef __cplusplus
        /* Use built-in C++ bool */
    #else
        typedef uint8_t        bool;
        #define false          ( (bool) 0u     )
        #define true           ( (bool) !false )
    #endif /* C++ */
  #endif   /* C89 */
#endif     /* C99 */

typedef struct
{
    uint8_t *data;
    uint8_t  length;
    uint8_t  size;
} MeloList;

#define MELO_LITTLE_ENDIAN             0u
#define MELO_BIG_ENDIAN                1u

#ifdef MELO_CFG_LITTLE_ENDIAN
    #define MELO_CFG_PE_ENDIANESS      MELO_LITTLE_ENDIAN
#endif

#ifdef MELO_CFG_BIG_ENDIAN
    #ifndef MELO_CFG_PE_ENDIANESS
        #define MELO_CFG_PE_ENDIANESS  MELO_BIG_ENDIAN
    #else
        #error "Duplicate processor endianess!"
    #endif
#endif

#ifndef MELO_CFG_PE_ENDIANESS
    #define MELO_CFG_PE_ENDIANESS MeloGetEndianess()
#else
    #define MELO_COMPILE_TIME_ENDIAN
#endif

/******************************************************************************
*                       Exported Function Prototypes                          *
******************************************************************************/
void    MeloBackground(void);
void    MeloInit(void);
void    MeloTransmitComplete(void);
void    MeloReceiveByte( const uint8_t byte );
void    MeloReceiveBytes( const uint8_t * const bytes, const uint8_t num );

#ifndef MELO_COMPILE_TIME_ENDIAN
uint8_t MeloGetEndianess(void);
#endif

#ifdef MELO_CFG_MODE_MASTER
uint8_t MeloServiceRequestBuilder(uint8_t * buffer, const uint8_t service, const uint8_t subfunction, const MeloList * const request_data, const bool use_crc );
#endif

/******************************************************************************
*                       Application Function Prototypes                       *
******************************************************************************/
uint8_t * MeloCreatePointer( const uint32_t address );
void      MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length );

#ifdef MELO_CFG_MODE_MASTER
void      MeloRequestBytes( const uint8_t num );
void      MeloReceiveResponse( const uint8_t service, const uint8_t subfunction, const uint8_t * const bytes, const uint8_t length, bool postive );
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
