#include "R311.h"

R311 fingReader;

void setup() {
  Serial.begin(57600); // USB virtual comm port, talking to computer
  while (!Serial) {} // wait for serial port to connect. Needed for native USB port only
  Serial.println("native USB port reporting for communications");
  fingReader.Open(&Serial1); // hardware serial port, talking to fingerprint reader
}

void loop() {
  if (Serial.available() > 0) {
    byte inByte = Serial.read();
  }
  byte returnCode = fingReader.ReadSysPara();
  printPackageRaw(returnCode); // for debugging
  printSSR();
  delay(100);
}

void printPackageRaw(uint8_t returnCode) {
  uint16_t actualSum = fingReader.calcChecksum();
  if (actualSum != fingReader.sum) return; // how about don't display garbage data
  // EF01 FFFFFFFF 07 0003 20 002A  ReadSysPara()=0xC  Length:3  PID:0x7  Data:20  sum:42
  //  0 1  2 3 4 5  6  7 8  9  A B
  Serial.print("  returned 0x");
  Serial.print(returnCode,HEX);
  Serial.print("  Length:");
  Serial.print((uint16_t)fingReader.length);
  Serial.print("  PID:0x");
  Serial.print((byte)fingReader.pid,HEX);
  Serial.print("  Data:");
  for (int i=0; i<fingReader.length - 2; i++) {
    if (fingReader.data[i] < 16) Serial.print("0"); // i shouldn't have to do this but here we are
    Serial.print(fingReader.data[i],HEX); // stupidly does not pad with zeroes for single-digit hex
  }
  Serial.print("  reported sum:");
  Serial.print(fingReader.sum);
  Serial.print("  actual sum:");
  Serial.println(actualSum);
}

void printSSR() { // print status bits from system_status_register
  Serial.print("SSR:");
  Serial.print(fingReader.system_status_register);
  Serial.print("  finger_library_size:");
  Serial.print(fingReader.finger_library_size);
  Serial.print("  security_level:");
  Serial.print(fingReader.security_level);
  Serial.print("  data_package_length:");
  Serial.print(32 << fingReader.data_package_length);
  Serial.print("  status: ");
  if (fingReader.Busy()) Serial.print("Busy ");
  if (fingReader.Pass()) Serial.print("Pass ");
  if (fingReader.PWD()) Serial.print("PWD ");
  if (fingReader.ImgBufStat()) Serial.print("ImgBufStat ");
  Serial.println(";");
}
