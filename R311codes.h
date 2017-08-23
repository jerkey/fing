//put error definitions in the FLASH memory
const char ERROR00[] PROGMEM = "command execution complete";
const char ERROR01[] PROGMEM = "error when receiving data package";
const char ERROR02[] PROGMEM = "no finger on the sensor";
const char ERROR03[] PROGMEM = "failed to enroll the finger";
const char ERROR04[] PROGMEM = "0x04";
const char ERROR05[] PROGMEM = "0x05";
const char ERROR06[] PROGMEM = "failed to generate character file due to the over-disorderly fingerprint image";
const char ERROR07[] PROGMEM = "failed to generate character file due to lackness of character point or over-smallness of fingerprint image";
const char ERROR08[] PROGMEM = "finger doesn’t match";
const char ERROR09[] PROGMEM = "failed to find the matching finger";
const char ERROR0A[] PROGMEM = "failed to combine the character files";
const char ERROR0B[] PROGMEM = "addressing PageID is beyond the finger library";
const char ERROR0C[] PROGMEM = "error when reading template from library or the template is invalid";
const char ERROR0D[] PROGMEM = "error when uploading template";
const char ERROR0E[] PROGMEM = "Module can’t receive the following data packages.";
const char ERROR0F[] PROGMEM = "error when uploading image";
const char ERROR10[] PROGMEM = "failed to delete the template";
const char ERROR11[] PROGMEM = "failed to clear finger library";
const char ERROR12[] PROGMEM = "0x12";
const char ERROR13[] PROGMEM = "wrong password!";
const char ERROR14[] PROGMEM = "0x14";
const char ERROR15[] PROGMEM = "failed to generate the image for the lackness of valid primary image";
const char ERROR16[] PROGMEM = "0x16";
const char ERROR17[] PROGMEM = "0x17";
const char ERROR18[] PROGMEM = "error when writing flash";
const char ERROR19[] PROGMEM = "No definition error";
const char ERROR1A[] PROGMEM = "invalid register number";
const char ERROR1B[] PROGMEM = "incorrect configuration of register";
const char ERROR1C[] PROGMEM = "wrong notepad page number";
const char ERROR1D[] PROGMEM = "failed to operate the communication port";

PGM_P const ERRORTEXT[] PROGMEM = { ERROR00, ERROR01, ERROR02, ERROR03, ERROR04, ERROR05, ERROR06, ERROR07, ERROR08, ERROR09, ERROR0A, ERROR0B, ERROR0C, ERROR0D, ERROR0E, ERROR0F, ERROR10, ERROR11, ERROR12, ERROR13, ERROR14, ERROR15, ERROR16, ERROR17, ERROR18, ERROR19, ERROR1A, ERROR1B, ERROR1C, ERROR1D };
#define ERRORTEXTBUFFERLENGTH 107
const char errorTextBuffer[ERRORTEXTBUFFERLENGTH];

void printErrorTextString(uint8_t errorNum) {
  if (errorNum <= 0x1D) {
    memcpy_P(errorTextBuffer, (char*)pgm_read_word(&(ERRORTEXT[errorNum])), ERRORTEXTBUFFERLENGTH);
    Serial.print(errorTextBuffer);
  } else {
    Serial.print("0x");
    if (errorNum < 16) Serial.print("0");
    Serial.print(errorNum,HEX);
  }
}
