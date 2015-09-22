/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

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
#include "melo.h"

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

void print_usage()
{
	cerr << "Usage: test_serial {-e|<serial port address>} ";
    cerr << "<baudrate> [test string]" << endl;
}

serial::Serial my_serial("COM4", 9600, serial::Timeout::simpleTimeout(1000));

int run(int argc, char **argv)
{
  if(argc < 2) {
	  print_usage();
    return 0;
  }

  // Argument 1 is the serial port or enumerate flag
  string port(argv[1]);

  if( port == "-e" ) {
	  enumerate_ports();
	  return 0;
  }
  else if( argc < 3 ) {
	  print_usage();
	  return 1;
  }

  // Argument 2 is the baudrate
  unsigned long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
  sscanf_s(argv[2], "%lu", &baud);
#else
  sscanf(argv[2], "%lu", &baud);
#endif

  // port, baudrate, timeout in milliseconds
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  cout << "Is the serial port open?";
  if(my_serial.isOpen())
    cout << " Yes." << endl;
  else
    cout << " No." << endl;

  // Get the Test string
  int count = 0;
  string test_string;
  if (argc == 4) {
    test_string = argv[3];
  } else {
    test_string = "Testing.";
  }

  // Test the timeout, there should be 1 second between prints
  cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    uint8_t data[8] = {0x28, 0x04, 0x01, 0x18, 0x01, 0x80, 0x00, 0x20};
    //size_t bytes_wrote = my_serial.write(test_string);
    size_t bytes_wrote = my_serial.write(data, 8);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms
  my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  count = 0;
  cout << "Timeout == 250ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking exactly for what was written
  count = 0;
  cout << "Timeout == 250ms, asking for exactly what was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length());

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking for 1 less than what was written
  count = 0;
  cout << "Timeout == 250ms, asking for 1 less than was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()-1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  return 0;
}

uint8_t * MeloCreatePointer( const uint32_t address )
{
    return ( (uint8_t *) address );
}

void MeloTransmitBytes( const uint8_t * const bytes, const uint8_t length )
{
  size_t bytes_wrote;
  try {
    do {
        bytes_wrote = my_serial.write(bytes, length);
    } while (bytes_wrote < length);
  } catch (exception &e) {
    cerr << "Unhandled Exception: " << e.what() << endl;
  }
  MeloTransmitComplete();
}

void MeloRequestBytes( const uint8_t num )
{
  printf("Requesting %d bytes from channel.\n", num);
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

int main(int argc, char **argv) {
    uint8_t tx_frame_buffer[30];
    uint8_t rx_frame_buffer[30];
    uint8_t frame_length;
    uint32_t magic_addr = (uint32_t) 0x00800118uL;
    
    MeloInit();
    
    frame_length = MeloReadMemoryByAddress( &(tx_frame_buffer[0]), magic_addr, false);
    
    MeloTransmitBytes(tx_frame_buffer, frame_length);
    
    printf("Frame length: %d\n", frame_length);
    
    my_serial.read(rx_frame_buffer, 5 + 8);
    
    MeloReceiveBytes(rx_frame_buffer, 5 + 8);
    
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    MeloBackground();
    
    
  //try {
  //  return run(argc, argv);
  //} catch (exception &e) {
  //  cerr << "Unhandled Exception: " << e.what() << endl;
  //}
  
  return 0;
}
