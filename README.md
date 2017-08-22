# using an R311 (maybe R303 maybe R300) capacitive fingerprint reader from an arduino, in this case a Leonardo

may also work with one of these other readers:

this one is a third the price and has higher resolution:
https://www.aliexpress.com/item/R303-Capacitive-Fingerprint-Reader-Module-Sensor-Scanner/32620290283.html

this one has the same resolution:
https://www.aliexpress.com/item/R300-All-in-one-Capacitive-Fingerprint-Reader-Module-sensor-FPC1020/32677023085.html

native USB port reporting for communications

GenImg() ///E3D0D6B53D36342C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000AGI:OK  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
Img2Tz(1 or 2) //////EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
Search(1,1,256) /EF01FFFFFFFF07000709000000000017  returned 0x9  Length:7  PID:0x7  Data:0900000000  reported sum:23  actual sum:23
PageID 0  MatchScore 0
GenImg() ///E3D0D6B53D38342C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000AGI:OK  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
Img2Tz(1 or 2) //////EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
Search(1,1,256) /EF01FFFFFFFF07000709000000000017  returned 0x9  Length:7  PID:0x7  Data:0900000000  reported sum:23  actual sum:23
PageID 0  MatchScore 0
TemplateNum() /EF01FFFFFFFF070005000002000E  returned 0x2  Length:5  PID:0x7  Data:000002  reported sum:14  actual sum:14

  1
  GenImg() ///E3D0D6B53D39382C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000AGI:OK  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  Img2Tz(1 or 2) ///////EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10

  2
  GenImg() ///E3D0D6B53D39312C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000AGI:OK  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  Img2Tz(1 or 2) ///////EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  RegModel() /EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  Store(1,TemplateNum()+1) //EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  TemplateNum() /EF01FFFFFFFF070005000002000E  returned 0x2  Length:5  PID:0x7  Data:000002  reported sum:14  actual sum:14

  3
  GenImg() ///E3D0D6B53D39312C205765743D33322C204472793D3332200AEF01FFFFFFFF07000300000AGI:OK  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  Img2Tz(1) ///////EF01FFFFFFFF07000300000A  returned 0x0  Length:3  PID:0x7  Data:00  reported sum:10  actual sum:10
  Search(1,1,256) /EF01FFFFFFFF070007000011006F008E  returned 0x0  Length:7  PID:0x7  Data:000011006F  reported sum:142  actual sum:142
  PageID 17  MatchScore 111

  e
  Empty() 
