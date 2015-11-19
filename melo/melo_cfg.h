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

#ifndef __MELO_CFG_H_
#define __MELO_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* #include <stdbool.h> */
#include <stdint.h>

#define MELO_CFG_MAX_DATA_LENGTH       100
#define MELO_CFG_MAX_STACK_SIZE        5

#define MELO_CFG_MODE_SLAVE
/* #define MELO_CFG_MODE_MASTER */

/*#define MELO_CFG_BIG_ENDIAN */
/* #define MELO_CFG_LITTLE_ENDIAN */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
