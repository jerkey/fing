#pragma once

#define R311BAUDRATE    57600 // R311 manual, page 4, default baud = 9600 * 6

class R311 {
  uint8_t m_R311Fault;
  uint8_t testInProgress;
public:
  uint8_t testSuccess;

// https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino
  R311(HardwareSerial *serial) {}

  HardwareSerial * _r311Serial; // member within class
  void Init();
  void Reset();
  void SetFault() { m_R311Fault = 1; }
  uint8_t Fault() { return m_R311Fault; }
  uint8_t SelfTest();
  void SetTestSuccess() { testSuccess = 1; }
  uint8_t SelfTestSuccess() { return testSuccess; }
  uint8_t SelfTestInProgress() { return testInProgress; }
};
