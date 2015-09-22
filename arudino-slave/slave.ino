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

#include "melo.h"

const uint32_t test_data = 0xCAFEBEEF;

uint8_t * MeloCreatePointer( const uint32_t address )
{
    //return ( (uint8_t *) address );
    return ( (uint8_t *) &test_data );
}

void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length )
{
  uint8_t bytesSent = 0;
  
  do
  {
    bytesSent += (uint8_t) Serial.write(bytes, length);
  } while(bytesSent < length);
  
  MeloTransmitComplete();
}

void MeloRequestBytes( const uint8_t num )
{
  /* TBD - Needed for Master Only */
}

void MeloReceiveResponse( const uint8_t service, const uint8_t subfunction, const uint8_t * const bytes, const uint8_t length, bool postive )
{
  /* TBD - Needed for Master Only */
}

void setup()
{
  Serial.begin(9600);
  MeloInit();
}

void loop()
{
  uint8_t input_byte;
    
  if ( Serial.available() )
  {
    /* get the new byte */
    input_byte = (uint8_t) Serial.read();
    MeloReceiveByte( input_byte );
    MeloBackground();
  }
}

