#pragma once

#define BAUD_RATE_CONTROL       6       // Parameter Number: 4
byte SECURITY_LEVEL =           3;      // Parameter Number: 5
byte DATA_PACKAGE_LENGTH =      1;      // Parameter Number: 6
#define MODULE_PASSWORD         0xFFFFFFFF // default is 0xFFFFFFFF
#define MODULE_ADDRESS          0xFFFFFFFF // default is 0xFFFFFFFF

#define R311BAUDRATE    (9600*BAUD_RATE_CONTROL) // R311 manual, page 4, default baud = 9600 * 6

class R311 {
  uint8_t m_R311Fault;
  uint16_t system_status_register;
  uint8_t testInProgress;
public:
  uint8_t testSuccess;

// https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino
  R311(HardwareSerial *serial) {}

  HardwareSerial * _r311Serial; // member within class
  void Init(); // open serial port
  uint16_t ReadSysPara(); // query hardware to update system_status_register
  boolean Busy() { return (system_status_register & 1); } // Busy: 1: system is executing commands; 0: system is free
  boolean Pass() { return (system_status_register & 2); } // Pass: 1: found the matching finger; 0: wrong finger
  boolean PWD() { return (system_status_register & 4); } // PWD: 1: Verified device’s handshaking password
  boolean ImgBufStat() { return (system_status_register & 8); } // ImgBufStat: 1: image buffer contains valid image
  uint8_t Fault() { return m_R311Fault; }
  uint8_t SelfTest();
  void SetTestSuccess() { testSuccess = 1; }
  uint8_t SelfTestSuccess() { return testSuccess; }
  uint8_t SelfTestInProgress() { return testInProgress; }
};
