#pragma once

uint16_t BAUD_RATE_CONTROL =    6;      // Parameter Number: 4
uint16_t SECURITY_LEVEL =       3;      // Parameter Number: 5
uint16_t DATA_PACKAGE_LENGTH =  1;      // Parameter Number: 6
uint32_t MODULE_PASSWORD =      0xFFFFFFFF; // default is 0xFFFFFFFF
uint32_t MODULE_ADDRESS =       0xFFFFFFFF; // default is 0xFFFFFFFF

#define R311BAUDRATE    (9600*BAUD_RATE_CONTROL) // R311 manual, page 4, default baud = 9600 * 6
#define CODE_OK                 00h // command execution complete
#define CODE_NOFINGER           02h // no finger on the sensor
#define CODE_NOMATCH            08h // finger doesn’t match

typedef struct {
	uint16_t start; // Header: Fixed value of 0xEF01; High byte transferred first.
	uint32_t adder; // Address: Default value is 0xFFFFFFFF, which can be modified by command. High byte transferred first and at wrong adder value, module will reject to transfer.
	uint8_t pid;    // Package identifier: 01H Command packet; 02H Data packet; Data packet shall not appear alone in executing processs, must follow command packet or acknowledge packet.  07H Acknowledge packet; 08H End of Data packet.
	uint16_t length; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
        uint8_t data[256]; // It can be commands, data, command’s parameters, acknowledge result, etc. (fingerprint character value, template are all deemed as data);
	uint16_t sum; // Checksum: The arithmetic sum of package identifier, package length and all package contens. Overflowing bits are omitted. high byte is transferred first.
} R311Package;

class R311 {
  uint16_t system_status_register;
  uint16_t finger_library_size;
public:
  R311(HardwareSerial *serial) {} // https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino

  HardwareSerial * _r311Serial; // member within class
  R311Package package; // data package returned from reader

  void Init(); // open serial port
  uint8_t ReadSysPara(); // returns confirmation code. Query hardware to update system_status_register, finger_library_size, SECURITY_LEVEL, MODULE_ADDRESS, DATA_PACKAGE_LENGTH, BAUD_RATE_CONTROL
  uint8_t SetSysPara(byte paramNum, byte contents); // returns confirmation code. Set module system’s basic parameter.
  boolean Busy() { ReadSysPara(); return (system_status_register & 1); } // Busy: 1: system is executing commands; 0: system is free
  boolean Pass() { ReadSysPara(); return (system_status_register & 2); } // Pass: 1: found the matching finger; 0: wrong finger
  boolean PWD() { ReadSysPara(); return (system_status_register & 4); } // PWD: 1: Verified device’s handshaking password
  boolean ImgBufStat() { ReadSysPara(); return (system_status_register & 8); } // ImgBufStat: 1: image buffer contains valid image
  uint16_t TempleteNum(); // returns template number. Reads the current valid template number of the Module.
  uint8_t GenImg(); // returns confirmation code. Detect finger and store the detected finger image in ImageBuffer; otherwise returns CODE_NOFINGER
  uint8_t Img2Tz(uint8_t BufferID); // returns confirmation code. Generate character file from the original finger image in ImageBuffer and store the file in CharBuffer1 or CharBuffer2
  uint8_t RegModel(); // returns confirmation code. Combine information of character files from CharBuffer1 and CharBuffer2 and generate a template which is stored into both CharBuffer1 and CharBuffer2
  uint8_t Store(uint8_t BufferID, uint16_t PageID); // returns confirmation code. Store the template of specified buffer (Buffer1/Buffer2) at the designated location in Flash library
  uint8_t DeletChar(uint16_t PageID, uint16_t N); // returns confirmation code. Delete a segment (N) of templates of Flash library started from the specified location (or PageID)
  uint8_t Empty(); // returns confirmation code. Delete all the templates in the Flash library
  uint8_t Match(); // returns confirmation code. Carry out precise matching of two finger templates from CharBuffer1 and CharBuffer2, providing matching results
};
