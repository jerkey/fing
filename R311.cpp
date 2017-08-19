#include "R311.h"
#include <HardwareSerial.h>
#include <Arduino.h>
void R311_isr() {}// interrupt service routing

void R311::Open(HardwareSerial *serial) {// https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino
  _r311Serial = serial;
  _r311Serial->begin(R311BAUDRATE);
  //attachInterrupt(R311_INTERRUPT,R311_isr,RISING);
}

uint8_t R311::ReadSysPara() { // return confirmation code or 0xF0 for receive data fail
  pid = 0x01; // Command packet
  length = 3; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x0F; // ReadSysPara
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x1C); // we should have got 28 bytes total
  if (bytesReceived != 0x1C) {
    Serial.print("ReadSysPara bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0;
  }
  // 00 0000000001000003 FFFFFFFF00020006
  //  0  1 2 3 4 5 6 7 8  9 A B C D E F G
  uint8_t returnCode = data[0]; // confirmation code is followed by 16 bytes of data, manual pp10-11
  system_status_register = data[1] * 256 + data[2]; // TODO: is this the right order?
  finger_library_size    = data[5] * 256 + data[6]; // TODO: is this the right order?
  security_level         = data[8];  // security_level is less than a byte so this works...
  data_package_length    = data[14]; // this seems right
  baud_rate_control      = data[16]; // this seems right
  return returnCode; // return confirmation code
}

uint8_t  R311::SetSysPara(uint8_t paramNum, uint8_t contents); // returns confirmation code. Set module system’s basic parameter.

uint16_t R311::TemplateNum() { // returns template number. Reads the current valid template number of the Module.
  pid = 0x01; // Command packet
  length = 3; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x1D; // TemplateNum
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x0E); // we should have got 14 bytes total
  if (bytesReceived != 0x0E) {
    Serial.print("TemplateNum bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0;
  }
  uint16_t template_number = data[1] * 256 + data[2]; // TODO: is this the right order?
  return template_number; // return template number
}

#define DEBUG
uint8_t  R311::GenImg() { // returns confirmation code. Detect finger and store the detected finger image in ImageBuffer; otherwise returns CODE_NOFINGER
  pid = 0x01; // Command packet
  length = 3; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x01; // GenImg
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x0C); // we should have got 12 bytes total
  if (bytesReceived != 0x0C) {
    Serial.print("GenImg bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0; // we did not get the expected number of bytes
  }
  Serial.print("GI:");
  if (data[0] == CODE_OK) Serial.print("OK");
  if (data[0] == CODE_NOFINGER) Serial.print("NOFINGER");
  if (data[0] == CODE_NOMATCH) Serial.print("NOMATCH");
  return data[0]; // confirmation code is the only data, manual pp11-12
}

uint8_t  R311::Img2Tz(uint8_t BufferID) { // returns confirmation code. Generate character file from the original finger image in ImageBuffer and store the file in CharBuffer1 or CharBuffer2
  pid = 0x01; // Command packet
  length = 4; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x02; // Img2Tz
  data[1] = BufferID; // choose which charbuffer to store the character file into
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x0C); // we should have got 12 bytes total
  if (bytesReceived != 0x0C) {
    Serial.print("Img2Tz bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0; // we did not get the expected number of bytes
  }
  return data[0]; // confirmation code is the only data, manual p13
}

uint8_t  R311::RegModel() { // returns confirmation code. Combine information of character files from CharBuffer1 and CharBuffer2 and generate a template which is stored into both CharBuffer1 and CharBuffer2
  pid = 0x01; // Command packet
  length = 3; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x05; // RegModel
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x0C); // we should have got 12 bytes total
  if (bytesReceived != 0x0C) {
    Serial.print("RegModel bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0; // we did not get the expected number of bytes
  }
  return data[0]; // confirmation code is the only data, manual p13
}

uint8_t  R311::Store(uint8_t BufferID, uint16_t PageID) { // returns confirmation code. Store the template of specified buffer (Buffer1/Buffer2) at the designated location in Flash library
  pid = 0x01; // Command packet
  length = 6; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x06; // Store
  data[1] = BufferID; // choose which charbuffer to store in the library
  data[2] = PageID >> 8; // high byte first
  data[3] = PageID & 0xFF;
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x0C); // we should have got 12 bytes total
  if (bytesReceived != 0x0C) {
    Serial.print("Store bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0; // we did not get the expected number of bytes
  }
  return data[0]; // confirmation code is the only data, manual p15
}

uint8_t  R311::DeletChar(uint16_t PageID, uint16_t N); // returns confirmation code. Delete a segment (N) of templates of Flash library started from the specified location (or PageID)
uint8_t  R311::Empty(); // returns confirmation code. Delete all the templates in the Flash library
uint8_t  R311::Match(); // returns confirmation code. Carry out precise matching of two finger templates from CharBuffer1 and CharBuffer2, providing matching results
uint8_t  R311::Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum) { // returns confirmation code. Search the whole finger library for the template that matches the one in CharBuffer1 or CharBuffer2. If found, PageID and MatchScore are populated
  pid = 0x01; // Command packet
  length = 8; // length of package content (command packets and data packets) plus the length of Checksum (2 bytes). Unit is byte. Max length is 256 bytes. And high byte is transferred first.
  data[0] = 0x04; // Search
  data[1] = BufferID; // choose which charbuffer to store in the library
  data[2] = StartPage >> 8; // high byte first
  data[3] = StartPage & 0xFF;
  data[4] = PageNum >> 8; // high byte first
  data[5] = PageNum & 0xFF;
  sendPackage();
  uint16_t bytesReceived = receivePackage(0x10); // we should have got 16 bytes total
  if (bytesReceived != 0x10) {
    Serial.print("Store bytesReceived:");
    Serial.println(bytesReceived);
    return 0xF0; // we did not get the expected number of bytes
  }
  PageID = data[1] * 256 + data[2];
  MatchScore = data[3] * 256 + data[4];
  return data[0]; // confirmation code, manual p17
}

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

uint8_t R311::waitForReadiness(bool serialToo) { // wait for not Busy() and (optionally) serial buffer to clear
  uint32_t startTime = millis(); // https://playground.arduino.cc/Code/TimingRollover
  while((millis() - startTime < BUSYTIMEOUTTIME && R311::Busy()) || (_r311Serial->available() && serialToo)) {
    if (_r311Serial->available() && serialToo) _r311Serial->read(); // clear serial buffer if desired
  }
  if (_r311Serial->available() && serialToo)    return 0xFD; // we timed out waiting for serial buffer to clear
  if (system_status_register & 1)              return 0xFE; // we timed out waiting for not Busy()
  return 0; // success, device is not busy and (optionally) serial buffer is clear
}

uint16_t R311::receivePackage(uint16_t bytesNeeded) { // returns # of bytes received OR 0xF000
  uint16_t packageProgess = 0; // how many bytes have we received
  length = 32 << data_package_length; // maximum possible package length
  uint16_t loopCount = 0; // for counting how many receive failures we get
  while (packageProgess < bytesNeeded) { // exit when we have enough, or error 0xF000
    packageProgess = 0; // reset counter for this loop
    while (_r311Serial->available() && packageProgess < (length + 9)) {
      uint8_t inByte = _r311Serial->read();
#ifdef DEBUG
      if (inByte < 16) Serial.print("0"); // i shouldn't have to do this but here we are
      Serial.print(inByte,HEX); // stupidly does not pad with zeroes for single-digit hex
#endif // DEBUG
      if (packageProgess == 6) pid = inByte;
      if (packageProgess == 7) length = (inByte * 256) + 0xFF; // high byte first (conservatively high during this while loop)
      if (packageProgess == 8) length = (length & 0xFF00) + inByte; // replace low byte
      if ((packageProgess > 8) && (packageProgess - 9 < length - 2)) data[packageProgess - 9] = inByte;
      if (packageProgess == length + 9 - 2) sum = inByte * 256; // high byte first
      if (packageProgess == length + 9 - 1) sum += inByte;
      packageProgess++;
      // check for EF01FFFFFFFF etc.. before proceeding             v
      // GenImg() E3D0D6B53D39392C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000A
      if (packageProgess == 1 && inByte != 0xEF) packageProgess = 0; // crude hack to check for header
      if (packageProgess == 2 && inByte != 0x01) packageProgess = 0; // crude hack to check for header
    }
    if (packageProgess < bytesNeeded) {
      Serial.print("/"); // report a short read (for debugging)
      delay(100); // this makes it actually work
    }
    if (loopCount++ > 50) packageProgess = 0xF000; // leave the while loop after 5 seconds of fail
  }
  return packageProgess;
}

uint16_t R311::calcChecksum() { // returns calculated checksum of whatever package is in RAM
  uint16_t actualSum = length + pid; // sum includes these and the data
  for (int i=0; i<length - 2; i++) actualSum += data[i]; // calculate the sum of data too
  return actualSum;
}
