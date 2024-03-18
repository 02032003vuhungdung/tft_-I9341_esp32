//<App !Start!>
// FILE: [bai_6.ino]
// Created by GUIslice Builder version: [0.17.b27]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "bai_6_GSLC.h"
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

#include <TFT_eWidget.h>               // Widget library
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter; 

GraphWidget gr = GraphWidget(&tft);    // Graph widget gr instance with pointer to tft
TraceWidget tr = TraceWidget(&gr);     // Graph trace tr with pointer to gr

const float gxLow  = 0.0;
const float gxHigh = 100.0;
const float gyLow  = -512.0;
const float gyHigh = 512.0;
int i = 0;
int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;
const int freq = 5000;     //tần số 5000
const int ledChannel = 0;  //kênh PWM - kênh 0
const int resolution = 8;  // độ phân giải của PWM , 8bit ~ 254 mức
const int ledPin = 5;
// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemProgress2_4= NULL;
gslc_tsElemRef* m_pElemVal1       = NULL;
gslc_tsElemRef* m_pElemVal1_2     = NULL;
gslc_tsElemRef* m_pElemXRingGauge1= NULL;
gslc_tsElemRef* m_pElemXRingGauge2= NULL;
gslc_tsElemRef* m_pElemXRingGauge3= NULL;
gslc_tsElemRef* m_pElemKeyPadNum  = NULL;
//<Save_References !End!>
#define QTRO 12
#define BH1750 14
#define PIR 27 
#define LED 33
int a=0;
// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_NUMINPUT1:
        // Clicked on edit field, so show popup box and associate with this text field
        gslc_ElemXKeyPadInputAsk(&m_gui, m_pElemKeyPadNum, E_POP_KEYPAD_NUM, m_pElemVal1);
        break;
      case E_ELEM_NUMINPUT2:
        // Clicked on edit field, so show popup box and associate with this text field
        gslc_ElemXKeyPadInputAsk(&m_gui, m_pElemKeyPadNum, E_POP_KEYPAD_NUM, m_pElemVal1_2);
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}
//<Checkbox Callback !Start!>
//<Checkbox Callback !End!>
// KeyPad Input Ready callback
bool CbKeypad(void* pvGui, void *pvElemRef, int16_t nState, void* pvData)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)pvGui;
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  // From the pvData we can get the ID element that is ready.
  int16_t nTargetElemId = gslc_ElemXKeyPadDataTargetIdGet(pGui, pvData);
  if (nState == XKEYPAD_CB_STATE_DONE) {
    // User clicked on Enter to leave popup
    // - If we have a popup active, pass the return value directly to
    //   the corresponding value field
    switch (nTargetElemId) {
//<Keypad Enums !Start!>
      case E_ELEM_NUMINPUT1:
        gslc_ElemXKeyPadInputGet(pGui, m_pElemVal1, pvData);
        gslc_PopupHide(&m_gui);
        break;
      case E_ELEM_NUMINPUT2:
        gslc_ElemXKeyPadInputGet(pGui, m_pElemVal1_2, pvData);
        gslc_PopupHide(&m_gui);
        break;

//<Keypad Enums !End!>
      default:
        break;
    }
  } else if (nState == XKEYPAD_CB_STATE_CANCEL) {
    // User escaped from popup, so don't update values
    gslc_PopupHide(&m_gui);
  }
  return true;
}
//<Spinner Callback !Start!>
//<Spinner Callback !End!>
//<Listbox Callback !Start!>
//<Listbox Callback !End!>
//<Draw Callback !Start!>
//<Draw Callback !End!>
//<Slider Callback !Start!>
//<Slider Callback !End!>
//<Tick Callback !Start!>
//<Tick Callback !End!>

void setup()
{
  // ------------------------------------------------
  // Initialize
  // ------------------------------------------------
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  //Gán chân led vào kênh PWM đã chọn
  ledcAttachPin(ledPin, ledChannel);
  //Khởi tạo Serial với baurate 9600
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(35, INPUT_PULLDOWN);

  // Wait for USB Serial 
  //delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used
  Wire.begin();
  lightMeter.begin();
  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{
  int qtro = digitalRead(QTRO);
  int bh1750 = digitalRead(BH1750);
  int pir = digitalRead(PIR);
  int led = digitalRead(LED);
  int a3= digitalRead(35); 

  if((i==0)&&(qtro==HIGH)){
    i=1;
    i1=0;
    i2=0; 
    i3 = 0;
    i4=0;
    gslc_SetPageCur(&m_gui,E_PG_MAIN);   
  }
  if((i==1)&&(qtro==LOW)){
  gslc_SetPageCur(&m_gui,E_PG_MAIN);  
  tft.begin();
  tft.setRotation(1);

  // Graph area is 200 pixels wide, 150 pixels high, dark grey background
  gr.createGraph(200, 150, tft.color565(5, 5, 5));

  // x scale units is from 0 to 100, y scale units is -512 to 512
  gr.setGraphScale(gxLow, gxHigh, gyLow, gyHigh);

  // X grid starts at 0 with lines every 20 x-scale units
  // Y grid starts at -512 with lines every 64 y-scale units
  // blue grid
  gr.setGraphGrid(gxLow, 20.0, gyLow, 20.0, TFT_BLUE);

  // Draw empty graph, top left corner at pixel coordinate 40,10 on TFT
  gr.drawGraph(20, 150);

  // Start a trace with using red, trace points are in x and y scale units
  // In this example a horizontal line is drawn
  tr.startTrace(TFT_RED);
  // Add a trace point at 0.0,0.0 on graph
  tr.addPoint(0.0, 0.0);
  // Add another point at 100.0, 0.0 this will be joined via line to the last point added
  tr.addPoint(100.0, 0.0);

  // Start a new trace with using white
  tr.startTrace(TFT_GREEN);  
  i=2;    
  }
  if((i==2)&&(qtro==HIGH)){
    i=3;     
  }
  if((i==3)&&(qtro==LOW)){
  int a=analogRead(13);
  tft.setCursor(135, 91, 2);
  tft.println(a);           // Print floating point number

  static uint32_t plotTime = millis();
  static float gx = 0.0, gy = 0.0;
  static float delta = 10.0;

  // Create a new plot point every 100ms
  if (millis() - plotTime >= 100) {
    plotTime = millis();

    // Add a plot, first point in a trace will be a single pixel (if within graph area)
    tr.addPoint(gx, gy);
    gx += 1;
    gy =a/10;

    // If the end of the graph x ais is reached start a new trace at 0.0,0.0
    if (gx > gxHigh) {
      gx = 0.0;
      gy = a/10;

      // Draw empty graph at 40,10 on display to clear old one
      gr.drawGraph(20, 150);
      // Start new trace
      tr.startTrace(TFT_GREEN);
    }}

  i = 3;      
  }
  int lux = lightMeter.readLightLevel();
  if((i1==0)&&(bh1750==HIGH)){
    i1=1;
    i=0;
    i2=0;
    i3 = 0;
    i4=0;
    gslc_SetPageCur(&m_gui,E_PG2);
    gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK1);
  }
  if((i1==1)&&(bh1750==LOW)){ 
    gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK1);
    gslc_SetPageCur(&m_gui,E_PG2);
    gslc_ElemXRingGaugeSetVal(&m_gui, m_pElemXRingGauge1, (lux)); 
    gslc_SetPageCur(&m_gui,E_PG2); 
    tft.setCursor(135, 121, 2);
    tft.println(lux);
    i1=1;

  }
  if((i2==0)&&(pir==HIGH)){
    i2=1;
    i=0;
    i1=0;
    i3 = 0;
    i4=0;
    gslc_SetPageCur(&m_gui,E_PG3);
  }  
  if((i2==1)&&(pir==LOW)){ 
  gslc_SetPageCur(&m_gui,E_PG3);
  tft.begin();
  tft.setRotation(1);

  // Graph area is 200 pixels wide, 150 pixels high, dark grey background
  gr.createGraph(200, 150, tft.color565(5, 5, 5));

  // x scale units is from 0 to 100, y scale units is -512 to 512
  gr.setGraphScale(gxLow, gxHigh, gyLow, gyHigh);

  // X grid starts at 0 with lines every 20 x-scale units
  // Y grid starts at -512 with lines every 64 y-scale units
  // blue grid
  gr.setGraphGrid(gxLow, 20.0, gyLow, 20.0, TFT_BLUE);

  // Draw empty graph, top left corner at pixel coordinate 40,10 on TFT
  gr.drawGraph(20, 150);

  // Start a trace with using red, trace points are in x and y scale units
  // In this example a horizontal line is drawn
  tr.startTrace(TFT_RED);
  // Add a trace point at 0.0,0.0 on graph
  tr.addPoint(0.0, 0.0);
  // Add another point at 100.0, 0.0 this will be joined via line to the last point added
  tr.addPoint(100.0, 0.0);

  // Start a new trace with using white
  tr.startTrace(TFT_GREEN);  
  i2=2;    
  }    
  if((i2==2)&&(pir==HIGH)){
    i2=3;
  }
  if((i2==3)&&(pir==LOW)){
  bool a2=digitalRead(32);
  static uint32_t plotTime = millis();
  static float gx = 0.0, gy = 0.0;
  static float delta = 10.0;

  // Create a new plot point every 100ms
  if (millis() - plotTime >= 100) {
    plotTime = millis();

    // Add a plot, first point in a trace will be a single pixel (if within graph area)
    tr.addPoint(gx, gy);
    gx += 0.5;
    gy =a2*100;

    // If the end of the graph x ais is reached start a new trace at 0.0,0.0
    if (gx > gxHigh) {
      gx = 0.0;
      gy = a2*200;

      // Draw empty graph at 40,10 on display to clear old one
      gr.drawGraph(20, 150);
      // Start new trace
      tr.startTrace(TFT_GREEN);
    }}
    if(a2==HIGH){
      tft.setCursor(30, 50, 2);
      tft.print("Xuat   hien    chuyen   dong");    
    i2=3;           
    }
    else{
      tft.setCursor(30, 50, 2);
      tft.print("Khong xuat hien chuyen dong"); 
    }
  }
  if((i3==0)&&(led==HIGH)){
    i3=1;
    i=0;
    i1=0;
    i2 = 0;
    i4=0;
    gslc_SetPageCur(&m_gui,E_PG4);
    gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK1);
  }  
  if((i3==1)&&(led==LOW)){ 
  gslc_SetPageCur(&m_gui,E_PG4);
  gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK1);

  gslc_ElemXRingGaugeSetVal(&m_gui, m_pElemXRingGauge2, analogRead(13));
  
  gslc_ElemXRingGaugeSetVal(&m_gui, m_pElemXRingGauge3, lux);
  i3 = 1;
  }
  
  if((i4==0)&&(a3==HIGH)){
    i4=1;

  }
  if((i4==1)&&(a3==LOW)){
  int level = map(0, 0, 100, 254, 0);
  ledcWrite(ledChannel, level);  //Đưa mức PWM vào kênh PWM đã chọn
  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2_4,0 );
    i4=2;
  }
  if((i4==2)&&(a3==HIGH)){
    i4=3;
  }
  if((i4==3)&&(a3==LOW)){
  int level = map(25, 0, 100, 254, 0);
  ledcWrite(ledChannel, level);  //Đưa mức PWM vào kênh PWM đã chọn
  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2_4,25 );
 
    i4=4;
  }
    if((i4==4)&&(a3==HIGH)){
    i4=5;
  }
  if((i4==5)&&(a3==LOW)){
  int level = map(50, 0, 100, 254, 0);
  ledcWrite(ledChannel, level);  //Đưa mức PWM vào kênh PWM đã chọn
  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2_4,50 );
    i4=6;
  }
    if((i4==6)&&(a3==HIGH)){
    i4=7;
  }
  if((i4==7)&&(a3==LOW)){
  int level = map(75, 0, 100, 254, 0);
  ledcWrite(ledChannel, level);  //Đưa mức PWM vào kênh PWM đã chọn
  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2_4,75 );
  
    i4=8;
  }
  if((i4==8)&&(a3==HIGH)){
    i4=9;
  }
  if((i4==9)&&(a3==LOW)){
  int level = map(100, 0, 100, 254, 0);
  ledcWrite(ledChannel, level);  //Đưa mức PWM vào kênh PWM đã chọn
  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2_4,100 );
 
    i4=0;
  }

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  //TODO - Add update code for any text, gauges, or sliders
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------


  gslc_Update(&m_gui);
  Serial.println(i4);
}

