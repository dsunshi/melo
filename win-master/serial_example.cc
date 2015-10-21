
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

#define LINE_MAX 100

static char line[LINE_MAX];
serial::Serial my_serial;//("COM4", 9600, serial::Timeout::simpleTimeout(1000));

void print_header(void);
unsigned int get_value(void);
void comm_port(void);

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

void print_header(void)
{
    printf("-- Options --\n");
    printf("0 - Read  Memory by Address\n");
    printf("1 - Write Memory by Address\n");
    printf("c - Comm Port Configuration\n");
    printf("? - Help\n");
    printf("x - eXit\n");
    printf("-------------\n");
}

void comm_port(void)
{
    vector<serial::PortInfo> devices_found  = serial::list_ports();
    vector<serial::PortInfo>::iterator iter = devices_found.begin();
    uint8_t index = 0;
    unsigned int value;
    
    printf("Available Devices: \n");
    printf("-------------------\n");
    
    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "%d:\t %s, %s\n", index, device.port.c_str(), device.description.c_str() );
        index++;
    }
    
    printf("-------------------\n");
    
    printf("Select device (0 to %d): ", index - 1);
    
    if ( fgets(line, LINE_MAX, stdin) != NULL )
    {
        value = (unsigned int) atoi(line);
        
        for (iter = devices_found.begin(), index = 0; iter != devices_found.end();  index++)
        {
            serial::PortInfo device = *iter++;
            
            if ( index == value )
            {
                printf("Initializing %s ...\n", device.port.c_str());
                
                my_serial.setPort(device.port.c_str());
                my_serial.setBaudrate(9600);
                my_serial.setTimeout( serial::Timeout::simpleTimeout(1000) );
                my_serial.open();
                
                if(my_serial.isOpen())
                {
                    printf("Serial port opened\n");
                }
                else
                {
                    printf("Failed to open port!\n");
                }
            }
        }
    }
    else
    {
        
    }
}

unsigned int get_value(void)
{
    unsigned int value;
    
    if ( fgets(line, LINE_MAX, stdin) != NULL )
    {
        value = (unsigned int) strtol(line, NULL, 16);
        //printf("String value = %s, Int value = %d\n", line, value);
    }
    else
    {
        print_header();
    }

    return value;
}

int main(int argc, char** argv)
{
    char cmd;

    unsigned int address;
    unsigned int value;

    uint8_t tx_frame_buffer[30];
    uint8_t rx_frame_buffer[30];
    uint8_t frame_length;

    MeloInit();

    print_header();

    printf("> ");

    while ( fgets(line, LINE_MAX, stdin) != NULL )
    {
        cmd = line[0];

        if (cmd == '0')
        {
            printf("0 - Read  Memory by Address\n");
            printf("---------------------------\n");
            printf("Memory address (HEX):  ");
            address = get_value();

            frame_length = MeloReadMemoryByAddress( &(tx_frame_buffer[0]), (uint32_t) address, false);
            MeloTransmitBytes(tx_frame_buffer, frame_length);
            my_serial.read(rx_frame_buffer, frame_length + 6);
            MeloReceiveBytes(rx_frame_buffer, frame_length + 6);

            MeloBackground();
            MeloBackground();
            MeloBackground();
            MeloBackground();
        }
        else if (cmd == '1')
        {
            printf("1 - Write Memory by Address\n");
            printf("Memory address (HEX):  ");
            address = get_value();
            printf("Value to write (HEX):  ");
            value   = get_value();

            frame_length = MeloWriteMemoryByAddress( &(tx_frame_buffer[0]), (uint32_t) address, (uint32_t) value, false );
            MeloTransmitBytes(tx_frame_buffer, frame_length);
            my_serial.read(rx_frame_buffer, frame_length + 6);
            MeloReceiveBytes(rx_frame_buffer, frame_length + 6);

            MeloBackground();
            MeloBackground();
            MeloBackground();
            MeloBackground();
        }
        else if ( (cmd == '?') || (cmd == '\n') )
        {
            print_header();
        }
        else if (cmd == 'c')
        {
            comm_port();
        }
        else if (cmd == 'x')
        {
            break;
        }
        else
        {
            printf("Unkown command: %c", cmd);
        }

        printf("> ");
    }

    return 0;
}
