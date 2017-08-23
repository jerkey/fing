#include "R311.h"
#include "R311codes.h" // for translating error numbers to text

R311 fingReader;

void setup() {
  Serial.begin(57600); // USB virtual comm port, talking to computer
  while (!Serial) {} // wait for serial port to connect. Needed for native USB port only
  Serial.println("fingerprint reader R311 test");
  Serial.println("https://github.com/jerkey/fing");
  fingReader.Open(&Serial1); // hardware serial port, talking to fingerprint reader
}

void loop() {
  if (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == '1') { // enroll once
      Serial.print("GenImg() ");
      printPackageRaw(fingReader.GenImg()); // print the returned code and all data
      Serial.print("Img2Tz(1) "); // call with a 1 or 2 to select which serial buffer
      printPackageRaw(fingReader.Img2Tz(1)); // print the returned code and all data
    }
    if (inChar == '2') { // enroll second time and store in first open spot
      Serial.print("GenImg() ");
      printPackageRaw(fingReader.GenImg()); // print the returned code and all data
      Serial.print("Img2Tz(2) "); // call with a 1 or 2 to select which serial buffer
      printPackageRaw(fingReader.Img2Tz(2)); // print the returned code and all data
      Serial.print("RegModel() ");
      printPackageRaw(fingReader.RegModel()); // print the returned code and all data
      Serial.print("Store(1,TemplateNum()) ");
      printPackageRaw(fingReader.Store(1,fingReader.TemplateNum()+1)); // RECORDS START WITH 1 NOT ZERO
      Serial.print("TemplateNum() = ");
      Serial.println(fingReader.TemplateNum()); // print the template number
    }
    if (inChar == '3') { // read finger, search for it and identify it
      Serial.print("GenImg() ");
      printPackageRaw(fingReader.GenImg()); // print the returned code and all data
      Serial.print("Img2Tz(1) "); // call with a 1 or 2 to select which serial buffer
      printPackageRaw(fingReader.Img2Tz(1)); // print the returned code and all data
      Serial.print("Search(1,1,256) ");
      fingReader.PageID = 0;
      fingReader.MatchScore = 0;
      printPackageRaw(fingReader.Search(1,1,256)); // print the returned code and all data
      Serial.print("PageID ");
      Serial.print(fingReader.PageID);
      Serial.print("  MatchScore ");
      Serial.println(fingReader.MatchScore);
    }
    if (inChar == 'g') {
      Serial.print("GenImg() ");
      printPackageRaw(fingReader.GenImg()); // print the returned code and all data
    }
    if (inChar == 'r') {
      Serial.print("ReadSysPara() ");
      printPackageRaw(fingReader.ReadSysPara()); // print the returned code and all data
      printSSR();
    }
    if (inChar == 'i') {
      Serial.print("Img2Tz(1 or 2) "); // call with a 1 or 2 to select which serial buffer
      printPackageRaw(fingReader.Img2Tz(Serial.read() - 48)); // print the returned code and all data
    }
    if (inChar == 't') {
      Serial.print("TemplateNum() = ");
      Serial.println(fingReader.TemplateNum()); // print the template number
    }
    if (inChar == '`') { // translate error number to text
      printErrorTextString(Serial.parseInt());
      Serial.println();
    }
    if (inChar == 's') {
      Serial.print("Store(1,parseInt()) ");
      printPackageRaw(fingReader.Store(1,Serial.parseInt())); // print the returned code and all data
    }
    if (inChar == 'f') {
      Serial.print("Search(1,1,256) ");
      fingReader.PageID = 0;
      fingReader.MatchScore = 0;
      printPackageRaw(fingReader.Search(1,1,256)); // print the returned code and all data
      Serial.print("PageID ");
      Serial.print(fingReader.PageID);
      Serial.print("  MatchScore ");
      Serial.println(fingReader.MatchScore);
    }
    if (inChar == 'm') {
      Serial.print("RegModel() ");
      printPackageRaw(fingReader.RegModel()); // print the returned code and all data
    }
    if (inChar == 'e') {
      Serial.print("Empty() ");
      printPackageRaw(fingReader.Empty()); // print the returned code and all data
    }
    if (inChar == 'p') {
      Serial.print("receivePackage(1) ");
      printPackageRaw(fingReader.receivePackage(1)); // print the returned code and all data
    }
    if (inChar == '!') {
      Serial.print("SetSysPara(5,1)"); // set security level
      printPackageRaw(fingReader.SetSysPara(5,1)); // set security level
    }
    if (inChar == '@') {
      Serial.print("SetSysPara(5,2)"); // set security level
      printPackageRaw(fingReader.SetSysPara(5,2)); // set security level
    }
    if (inChar == '#') {
      Serial.print("SetSysPara(5,3)"); // set security level
      printPackageRaw(fingReader.SetSysPara(5,3)); // set security level
    }
    if (inChar == '$') {
      Serial.print("SetSysPara(5,4)"); // set security level
      printPackageRaw(fingReader.SetSysPara(5,4)); // set security level
    }
    if (inChar == '%') {
      Serial.print("SetSysPara(5,5)"); // set security level
      printPackageRaw(fingReader.SetSysPara(5,5)); // set security level
    }
  } else { // no serial available
    if (fingReader.GenImg() == CODE_OK) { // found a finger
      if (fingReader.Img2Tz(1) == CODE_OK) { // successfully interpreted it
        fingReader.PageID = 0;
        fingReader.MatchScore = 0;
        if (fingReader.Search(1,1,256) == 0) { // print the returned data
          Serial.print("PageID ");
          Serial.print(fingReader.PageID);
          Serial.print("  MatchScore ");
          Serial.println(fingReader.MatchScore);
        }
      }
    } // scan for finger
  } // no serial available
}

void printPackageRaw(uint16_t returnCode) {
  uint16_t actualSum = fingReader.calcChecksum();
  if (actualSum != fingReader.sum) return; // how about don't display garbage data
  // EF01 FFFFFFFF 07 0003 20 002A  ReadSysPara()=0xC  Length:3  PID:0x7  Data:20  sum:42
  //  0 1  2 3 4 5  6  7 8  9  A B
  if (returnCode != 0) {
    Serial.print("ERROR: ");
    printErrorTextString(returnCode);
  }
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
