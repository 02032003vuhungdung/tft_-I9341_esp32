Note 
- Sử dụng các cảm biến BH1750, PIR, Quang trở
- Sử dụng esp32 và màn ili9341 tft 2.8 inch
- Sơ đồ chân
  #define TFT_MISO 19
  #define TFT_MOSI 23
  #define TFT_SCLK 18
  #define TFT_CS   15  // Chip select control pin
  #define TFT_DC    2  // Data Command control pin
  #define TFT_RST   4  // Reset pin (could connect to RST pin)
  #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
-Phản hồi qua email: vuhungdung0203@gmail.com
