#ifdef R311 // don't include this file more than once in compilation

void R311_isr() // interrupt service routing
{
}

void R311::Init(HardwareSerial *serial)
{
  _r311Serial = serial;
  _r311Serial.begin(R311BAUDRATE);
  //attachInterrupt(R311_INTERRUPT,R311_isr,RISING);
}

uint8_t R311::SelfTest()
{
  testInProgress = 1;
  testSuccess = 0;
  WDT_RESET();

  m_R311Fault = 0;
  testInProgress = 0;

  return !testSuccess;
}
#endif // R311
