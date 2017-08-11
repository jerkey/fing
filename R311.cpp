#ifdef R311 // don't include this file more than once in compilation

void R311_isr() // interrupt service routing
{
}

void R311::Open(HardwareSerial *serial)
{
  _r311Serial = serial;
  _r311Serial.begin(R311BAUDRATE);
  //attachInterrupt(R311_INTERRUPT,R311_isr,RISING);
}

uint8_t R311::ReadSysPara()
{
  uint8_t confirmationCode;

  return confirmationCode;
}
  uint8_t R311::SetSysPara(byte paramNum, byte contents); // returns confirmation code. Set module system’s basic parameter.
  uint16_t R311::TemplateNum(); // returns template number. Reads the current valid template number of the Module.
  uint8_t R311::GenImg(); // returns confirmation code. Detect finger and store the detected finger image in ImageBuffer; otherwise returns CODE_NOFINGER
  uint8_t R311::Img2Tz(uint8_t BufferID); // returns confirmation code. Generate character file from the original finger image in ImageBuffer and store the file in CharBuffer1 or CharBuffer2
  uint8_t R311::RegModel(); // returns confirmation code. Combine information of character files from CharBuffer1 and CharBuffer2 and generate a template which is stored into both CharBuffer1 and CharBuffer2
  uint8_t R311::Store(uint8_t BufferID, uint16_t PageID); // returns confirmation code. Store the template of specified buffer (Buffer1/Buffer2) at the designated location in Flash library
  uint8_t R311::DeletChar(uint16_t PageID, uint16_t N); // returns confirmation code. Delete a segment (N) of templates of Flash library started from the specified location (or PageID)
  uint8_t R311::Empty(); // returns confirmation code. Delete all the templates in the Flash library
  uint8_t R311::Match(); // returns confirmation code. Carry out precise matching of two finger templates from CharBuffer1 and CharBuffer2, providing matching results
  uint8_t R311::Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum); // returns confirmation code. Search the whole finger library for the template that matches the one in CharBuffer1 or CharBuffer2. If found, PageID and MatchScore are populated
#endif // R311
