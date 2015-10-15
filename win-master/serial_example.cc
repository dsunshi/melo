
#include <string>
#include <iostream>
#include <cstdio>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"
#include "./../melo/melo.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

void enumerate_ports()
{
	vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();

	while( iter != devices_found.end() )
	{
		serial::PortInfo device = *iter++;

		printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
     device.hardware_id.c_str() );
	}
}

serial::Serial my_serial("COM4", 9600, serial::Timeout::simpleTimeout(1000));

uint8_t * MeloCreatePointer( const uint32_t address )
{
    return ( (uint8_t *) address );
}

void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length )
{
  size_t bytes_wrote = 0;
  try {
    do {
        bytes_wrote = my_serial.write(bytes, length);
    } while (bytes_wrote < length);
  } catch (exception &e) {
    cerr << "Unhandled Exception: " << e.what() << endl;
  }
    if (bytes_wrote >= 0)
    {
    size_t index = 0;
    printf("Wrote: ");
    for (index = 0; index < length; index++)
    {
        printf("0x%X ", bytes[index]);
    }
    printf("\n");
        MeloTransmitComplete();
    }
}

void MeloRequestBytes( const uint8_t num )
{
  printf("Requesting %d bytes from channel.\n", num);
    MeloTransmitComplete();
}

void MeloReceiveResponse( const uint8_t service, const uint8_t subfunction, const uint8_t * const bytes, const uint8_t length, bool postive )
{
    uint8_t index;
    
    printf("Received Response: \n");
    printf("Positive Response: %d\n", postive);
    printf("Service:           %d\n", service);
    printf("Subfunction:       %d\n", subfunction);
    printf("Bytes:             ");
    for (index = 0; index < length; index++)
    {
        printf("0x%X ", bytes[index]);
    }
    printf("\n");
}

uint8_t MeloReadMemoryByAddress(uint8_t * buffer, const uint32_t address, const bool use_crc)
{
    MeloList request;
    uint8_t  data[4];
    
    request.data   = data;
    request.length = 4;
    
    /* Always this format - endianess/byte order is automatically accounted for! */
    data[0] = (address      ) & 0xFF;
    data[1] = (address >> 8 ) & 0xFF;
    data[2] = (address >> 16) & 0xFF;
    data[3] = (address >> 24) & 0xFF;
   
    
    return MeloServiceRequestBuilder(buffer, 0, 1, &request, use_crc);
}

uint8_t MeloWriteMemoryByAddress(uint8_t * buffer, const uint32_t address, const uint32_t value, const bool use_crc)
{
    MeloList request;
    uint8_t  data[8];
    
    request.data   = data;
    request.length = 8;
    
    /* Always this format - endianess/byte order is automatically accounted for! */
    data[0] = (address      ) & 0xFF;
    data[1] = (address >> 8 ) & 0xFF;
    data[2] = (address >> 16) & 0xFF;
    data[3] = (address >> 24) & 0xFF;
    
    data[4] = (value        ) & 0xFF;
    data[5] = (value   >> 8 ) & 0xFF;
    data[6] = (value   >> 16) & 0xFF;
    data[7] = (value   >> 24) & 0xFF;
   
    
    return MeloServiceRequestBuilder(buffer, 0, 2, &request, use_crc);
}

int main(int argc, char **argv) {
    uint8_t tx_frame_buffer[30];
    uint8_t rx_frame_buffer[30];
    uint8_t frame_length;
    uint32_t magic_addr = (uint32_t) 0x00800118uL;
    
    MeloInit();
    
    printf("REad\n");
    frame_length = MeloReadMemoryByAddress( &(tx_frame_buffer[0]), magic_addr, false);
    
    MeloTransmitBytes(tx_frame_buffer, frame_length);
    
    printf("Frame length: %d\n", frame_length);

    my_serial.read(rx_frame_buffer, frame_length + 6);
    
    MeloReceiveBytes(rx_frame_buffer, frame_length + 6);
    
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    
    my_sleep(1000);

    printf("Write\n");
    frame_length = MeloWriteMemoryByAddress( &(tx_frame_buffer[0]), magic_addr, 0xFFFFFFFF, false );
    
    printf("Frame length: %d\n", frame_length);
    
    MeloTransmitBytes(tx_frame_buffer, frame_length);
    
    my_serial.read(rx_frame_buffer, frame_length + 6);
    
    MeloReceiveBytes(rx_frame_buffer, frame_length + 6);
    
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();

  return 0;
}
