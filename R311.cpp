#include "R311.h"
#include <HardwareSerial.h>
#include <Arduino.h>
void R311_isr() // interrupt service routing
{
}

void R311::Open(HardwareSerial *serial) {// https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino
  _r311Serial = serial;
  _r311Serial->begin(R311BAUDRATE);
  //attachInterrupt(R311_INTERRUPT,R311_isr,RISING);
}

uint8_t R311::ReadSysPara()
{
  uint8_t confirmationCode;

  return confirmationCode;
}

uint8_t  R311::SetSysPara(byte paramNum, byte contents); // returns confirmation code. Set module system’s basic parameter.
uint16_t R311::TemplateNum(); // returns template number. Reads the current valid template number of the Module.
uint8_t  R311::GenImg(); // returns confirmation code. Detect finger and store the detected finger image in ImageBuffer; otherwise returns CODE_NOFINGER
uint8_t  R311::Img2Tz(uint8_t BufferID); // returns confirmation code. Generate character file from the original finger image in ImageBuffer and store the file in CharBuffer1 or CharBuffer2
uint8_t  R311::RegModel(); // returns confirmation code. Combine information of character files from CharBuffer1 and CharBuffer2 and generate a template which is stored into both CharBuffer1 and CharBuffer2
uint8_t  R311::Store(uint8_t BufferID, uint16_t PageID); // returns confirmation code. Store the template of specified buffer (Buffer1/Buffer2) at the designated location in Flash library
uint8_t  R311::DeletChar(uint16_t PageID, uint16_t N); // returns confirmation code. Delete a segment (N) of templates of Flash library started from the specified location (or PageID)
uint8_t  R311::Empty(); // returns confirmation code. Delete all the templates in the Flash library
uint8_t  R311::Match(); // returns confirmation code. Carry out precise matching of two finger templates from CharBuffer1 and CharBuffer2, providing matching results
uint8_t  R311::Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum); // returns confirmation code. Search the whole finger library for the template that matches the one in CharBuffer1 or CharBuffer2. If found, PageID and MatchScore are populated

uint8_t R311::sendPackage() { // returns 0
  _r311Serial->write(0xEF); // Header: Fixed value of 0xEF01; High byte transferred first.
  _r311Serial->write(0x01); // Header: Fixed value of 0xEF01; High byte transferred first.
  _r311Serial->write(module_address >> 24);
  _r311Serial->write(module_address >> 16);
  _r311Serial->write(module_address >> 8 );
  _r311Serial->write(module_address      );
  _r311Serial->write(pid);
  _r311Serial->write(length >> 8);
  _r311Serial->write(length);
  sum = pid + length; // arithmetic sum of package identifier, package length and all package contents. Overflowing bits are omitted. high byte is transferred first
  for (int i=0; i<length - 2; i++) { // send the data (length = data plus 2 bytes of checksum)
    _r311Serial->write(data[i]);
    sum += data[i]; // adding to checksum, overflowing is fine
  }
  _r311Serial->write(sum >> 8); // write checksum, high byte first
  _r311Serial->write(sum);
  return 0;
}

uint8_t waitForReadiness(boolean serialToo) { // wait for not Busy() and (optionally) serial buffer to clear
  uint32_t startTime = millis(); // https://playground.arduino.cc/Code/TimingRollover
  while((millis() - startTime < BUSYTIMEOUTTIME && R311::Busy()) || (_r311Serial->available() && serialToo)) {
    if (_r311Serial->available() && serialToo) _r311Serial->read(); // clear serial buffer if desired
  }
  if (_r311Serial->available() && serialToo)    return 0xFD; // we timed out waiting for serial buffer to clear
  if (system_status_register & 1)              return 0xFE; // we timed out waiting for not Busy()
  return 0; // success, device is not busy and (optionally) serial buffer is clear
}
