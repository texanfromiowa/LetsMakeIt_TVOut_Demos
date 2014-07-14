/*

TVout-Oscilloscope, v1.0

Lets you display the voltage on analog pin A0 on your TV using the TVout Library.

It displays the average, the minimal and the maximal voltage, and the sampling
frequency. 100 sample points are displayed.

The maximal sampling frequency is about 8kHz (on an Arduino Uno). 

Add a button to digital pin 2 to scale the voltage range, and to digital pin 3
to scale the sample range.

(c) Juerg Wullschleger 2011.

http://creativecommons.org/licenses/by/3.0/
*/

/**************************************************************
  Name      TVoutOscilloscope                              
  Modified By   Bob Powell 
                texanfromiowa@gmail.com
          
  Date      Jul 5, 2014    
  Modified  Jul 5, 2014                                
  Version   ?     
  Arduino   1.5.6-r2
  
  Notes     This code was downloaded from http://forum.arduino.cc/index.php?topic=64327.0
            and modified only slightly (added a tone for an onboard signal and keypress
            simulation code) for the Lets Make It Show demonstration.
            www.letsmakeit.tv
            
            The file was save with an .ino extension rather than the .pde extension 
            used on pre-1.0 Arduino IDE files.
            
            
Personal note:

If you do something interesting with this code, expand it, or just
have a question, please email it to me at the address above.  

I hope you find this example helpful.  Enjoy.

Bob

*/


#include <TVout.h>
#include <font4x6.h>

//input pins
const int inputPin = 0;
const int scalePin = 2;
const int samplePin = 3;

// define the graph area
const int graphOffsetX = 18;
const int graphOffsetY = 94;
const int graphWidth = 100;
const int graphHeight = 86;

const int upperGraphLimit = graphOffsetY + 1;
const int lowerGraphLimit = graphOffsetY - graphHeight;

int scaleFactor = 1024/graphHeight+1;
int offset = 0;
int sampleSize = 1;

int minValue, maxValue, avgValue;
long sampleTime = 0;

//used if sampleSize <=3 to draw faster
int data[graphWidth];
uint8_t dispY[graphWidth];

TVout TV;

// Used to simulate keypress later
int TEST = 0;

void setup(){                                         
  pinMode(scalePin, INPUT);     
  pinMode(samplePin, INPUT);     

  //init TV
  TV.begin(_NTSC,120,96);
  TV.select_font(font4x6);
  
  clearGraph();
  drawScale();
  
  // Connected Audio out pin (digital 11 on Uno, 10 on Mega)
  // to analog 0 (input pin) with a 100 ohm resistor for testing.
  TV.tone(200);
  
}

void drawPixel(int x, int y, char c){
  if((y > lowerGraphLimit) && (y < upperGraphLimit)){
    TV.set_pixel(x,y,c);
  }
}

int val2y(int val){
  return graphOffsetY - val/scaleFactor - offset;
}

void drawScale(){
  //delete old scale
  TV.draw_rect(0,lowerGraphLimit, graphOffsetX-2,graphHeight+1,0,0);
  
  for(int i = 0; i<=20;i++){
    int y = val2y(1024 * i / 20);
    drawPixel(graphOffsetX-2,y,1);
    if((i % 2 != 0) && (scaleFactor < 8) && (y > lowerGraphLimit + 3) && (y < upperGraphLimit - 3)){
      TV.print(graphOffsetX - 18,y-3,i/4.0,2);
    }
    if(i % 2 == 0){
      drawPixel(graphOffsetX-3,y,1);  
      if((i % 4 != 0) && (scaleFactor <=8) && (y > lowerGraphLimit + 3) && (y < upperGraphLimit - 3)){
        TV.print(graphOffsetX - 15,y-3,i/4.0,1);
      }
    }
    if(i % 4 == 0){
      drawPixel(graphOffsetX-4,y,1);
      drawPixel(graphOffsetX-5,y,1);
      if((y > lowerGraphLimit + 3) && (y < upperGraphLimit - 3)){
        TV.print(graphOffsetX - 9,y-3,i/4);
      }
    }
  }
}

void clearGraph(){
  TV.draw_rect(graphOffsetX - 1,graphOffsetY - graphHeight, graphWidth+1,graphHeight+1,1,0);
}

void drawData(int i, int val){
  // we use dispY[i] to store the displayed pixel. so we only need to
  // erase that pixel, and not the whole column.
  int y = val2y(val);
  int x = i + graphOffsetX;
  drawPixel(x,dispY[i],0);
  drawPixel(x,y,1);
  dispY[i] = y;
}

void drawData(int i, int v1, int v2){
  int u = val2y(v1);
  int v = val2y(v2);
  int x = i + graphOffsetX;
  TV.draw_column(x, graphOffsetY, graphOffsetY - graphHeight + 1, 0);
  u = min(max(u,lowerGraphLimit),upperGraphLimit);
  v = min(max(v,lowerGraphLimit),upperGraphLimit);
  TV.draw_column(x, u, v, 1);
}

void changeScaleFactor(){
  int i = 1;
  while(i < scaleFactor)
    i = i*2;
  scaleFactor = i / 2;
  if(scaleFactor == 0){
    scaleFactor = 1024/graphHeight+1;
  }
  offset = graphHeight/2 - avgValue/scaleFactor;
  
  // we need: 0/scaleFactor + offset >= 0 and 1024/scaleFactor + offset <= graphHeight
  offset = max(graphHeight - 1024/scaleFactor,offset);
  offset = min(0,offset);

  drawScale();
  clearGraph();
}

void changeSampleSize(){
  if(sampleSize<5)
    sampleSize = sampleSize + 1;
  else
    sampleSize = sampleSize * 8 / 5;
  if(sampleSize > 300){
    sampleSize = 1;
  }
  clearGraph();
}

boolean scaleButtonPressed = false, sampleButtonPressed = false;
boolean checkButtons(){
  if(digitalRead(scalePin) == HIGH){
    scaleButtonPressed = true;
  }else{
    if(scaleButtonPressed){
      changeScaleFactor();
      scaleButtonPressed = false;
      return true;
    }
  }
  if(digitalRead(samplePin) == HIGH){
    sampleButtonPressed = true;
  }else{
    if(sampleButtonPressed){
      changeSampleSize();
      sampleButtonPressed = false;
      return true;
    }
  }
  return false;
}

float val2volts(int val){
  return val * 5 / 1024.0;
}

void printVolts(){
  TV.print(2,2,val2volts(avgValue),2);
  TV.print("V ("); 
  TV.print(val2volts(minValue),2);
  TV.print("V-"); 
  TV.print(val2volts(maxValue),2);
  TV.print("V)"); 
}

void printSampleRate(){
  long gw = graphWidth;
  TV.print(90,2, gw*1000000/sampleTime);
  TV.print("Hz ");
}

void loop() {
  
  // Simpulate a keypress, if needed
  if(TEST <= 2){
    changeSampleSize();
    TEST++;
  }  
    
  
  checkButtons();
  
  // wait for the next raising edge, but only for max. 400 steps.
  boolean wasLow = false;
  for(int w=0 ; w<400 ; w++){
    int val = analogRead(inputPin);
    if(val < avgValue){
      wasLow = true;
    }else if((val > avgValue) && wasLow){
      break;
    }
  }
  
  minValue = 1024; maxValue = 0;
  long sum = 0;
  int i = 0;  
  long t = micros();

  while(i < graphWidth){
    if(sampleSize <= 3){
      //for small sampleSize, sample first and then draw
      if(sampleSize == 1){
        //read as fast as possible
        i=0;
        while(i < graphWidth){
           data[i] = analogRead(inputPin);
           i++;
        }
      }else{
        i=0;
        while(i < graphWidth){
          int s=0;
          for(int j=0; j<sampleSize;j++)
            s = s + analogRead(inputPin);
          data[i] = s/sampleSize;
          i++;
        }
      }
      sampleTime = micros() - t;
      i=0;
      while(i < graphWidth){
        int val = data[i];
        minValue = min(minValue, val);
        maxValue = max(maxValue, val);
        sum = sum + sampleSize*val;
        drawData(i,val);
        i++;
      }
    }else{
      //for large samleSize, do sample and draw at the same time.
      int minV = 1024, maxV = 0;
      for(int j = 0; j< sampleSize; j++){
        int val = analogRead(inputPin);
        minV = min(minV,val);
        maxV = max(maxV,val);
        sum = sum + val;
      }
      drawData(i,minV,maxV);
      minValue = min(minValue,minV);
      maxValue = max(maxValue,maxV);
      i++;

      if(checkButtons()){
        //button has been pressed. reset values.
        i = 0;
        t  = micros();
        minValue = 1024; maxValue = 0;
        sum = 0;
      }
      if(i==graphWidth)
        sampleTime = micros() - t;
    }
  }
  avgValue =  sum / (graphWidth*sampleSize);

  printVolts();
  printSampleRate(); 
}

