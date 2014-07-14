/**************************************************************
  Name      TVOutDemo                                
  Author    Bob Powell 
            texanfromiowa@gmail.com
            Copyright (C) 2012-2014, Parallelus Automation, Inc.
          
  Date      Jun 21, 2014    
  Modified  Jul 7, 2014                                
  Version   1.0.0      
  Arduino   1.5.6-r2
  
  Notes     
            
            
Legal Stuff:
============
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option, any later version.
	 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
	 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
          
   
     
Personal note:

If you do something interesting with this code, expand it, or just
have a question, please email it to me at the address above.  

I hope you find this example helpful.  Enjoy.

Bob


****************************************************************/
// Set some variables - Global variables, except for the Arduino
// pins, are CAPITALIZED.

// This code uses the TVOutBeta1 version of the library
#include <TVout.h>
#include <fontALL.h>
#include "schematic.h"
#include "TVOlogo.h"

TVout TV;

// Global variables
char TEMPBUFFER[63];
bool DEBUG = false;
// For an UNO 120x96 is as large as possible (at least that is what it seems)
int WIDTH = 136;  // must be divisable by 8
int HEIGHT = 96;

// Needed for the cube demo originally from the TVOut example
int zOff = 150;
int xOff = 0;
int yOff = 0;
int cSize = 50;
int view_plane = 64;
float angle = PI/60;

float cube3d[8][3] = {
  {xOff - cSize,yOff + cSize,zOff - cSize},
  {xOff + cSize,yOff + cSize,zOff - cSize},
  {xOff - cSize,yOff - cSize,zOff - cSize},
  {xOff + cSize,yOff - cSize,zOff - cSize},
  {xOff - cSize,yOff + cSize,zOff + cSize},
  {xOff + cSize,yOff + cSize,zOff + cSize},
  {xOff - cSize,yOff - cSize,zOff + cSize},
  {xOff + cSize,yOff - cSize,zOff + cSize}
};
unsigned char cube2d[8][2];


/**************************************************************
Function: setup
Purpose:  set up Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino
***************************************************************/
void setup() {
  
  if(DEBUG){
    Serial.begin(9600);
    Serial.println("Starting.");
  }
  
  TV.begin(NTSC,WIDTH,HEIGHT);
  //TV.select_font(font6x8);
  TV.clear_screen();
  TV.set_font(6);

  letsmakeit_intro();  
    
  play_charge();
  TV.set_font(6);
  
  randomSeed(analogRead(0));
} // end of setup()

/**************************************************************
Function: loop
Purpose:  loop funtion for Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
***************************************************************/
void loop() {
  
  run_cube(5000);
  TV.clear_screen();
  delay(5000);
  
  
} // end of loop()  


/**************************************************************
Function: letsmakeit_intro
Purpose:  Lets Make It intro 
Args:     none
Returns:  nothing
Notes:    For Lets Make It, www.letsmakeit.tv, demo..
***************************************************************/
void letsmakeit_intro()
{
  int x, y;

  // Put something on screen for small delay
  TV.print("\nLets Make It\nwww.letsmakeit.tv\nTVOut Demo\n\nStarting.");
  TV.draw_line(0,48,120,48,WHITE);
  for(int i = 0; i < 6; i++){
    TV.delay(500);
    TV.print(".");
  }
  TV.print(".done");
  TV.delay(2000);
  TV.clear_screen();

  // Show the visable area
  TV.draw_rect(0,0,WIDTH-1,HEIGHT-1,WHITE);
  sprintf(TEMPBUFFER, "Visable Area");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)-12, TEMPBUFFER);
  
  sprintf(TEMPBUFFER, "Width = %d", WIDTH);
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)+4, TEMPBUFFER);
  
  sprintf(TEMPBUFFER, "Height = %d", HEIGHT);
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)+12, TEMPBUFFER);
  delay(10000);  
 
}


/**************************************************************
Function: play_charge
Purpose:  plays Charge tune 
Args:     none
Returns:  nothing
Notes:    Used this for the tune: http://en.wikipedia.org/wiki/Charge_(fanfare)
          Frequency numbers from: http://en.wikipedia.org/wiki/Piano_key_frequencies
***************************************************************/
void play_charge()
{  
  
  TV.clear_screen();
  TV.set_font(8);
  sprintf(TEMPBUFFER, "Charge!!");  
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER), TEMPBUFFER);
  int note = 250;
  int noteoff = 50;

  // G4
  TV.tone(392);
  delay(note*2);
  TV.noTone();
  TV.delay(noteoff);

  // C5
  TV.tone(523);
  delay(note*2);
  TV.noTone();
  TV.delay(noteoff);

  // E5
  TV.tone(659);
  delay(note*2);
  TV.noTone();
  TV.delay(noteoff);

  // G5
  TV.tone(784);
  delay(note*3);
  TV.noTone();
  TV.delay(noteoff);

  // E5
  TV.tone(659);
  delay(note);
  TV.noTone();
  TV.delay(noteoff);
  
  // G5
  TV.tone(784);
  delay(note*8);
  TV.noTone();

  TV.noTone();
  TV.clear_screen();
  sprintf(TEMPBUFFER, "All done");  
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER), TEMPBUFFER);
  TV.delay(3000);
  
}


/**************************************************************
Function: get_centered_x
Purpose:  calculate x for centered text 
Args:     char *temp - pointer to string being printed
Returns:  x value
Notes:    
***************************************************************/
int get_centered_x(char *temp)
{
  int x = (WIDTH - strlen(temp)*TV.get_font_x())/2;
  return x;
}

/**************************************************************
Function: get_centered_y
Purpose:  calculate y for centered text 
Args:     char *temp - pointer to string being printed
Returns:  y value
Notes:    
***************************************************************/
int get_centered_y(char *temp)
{
  int x = (HEIGHT + (TV.get_font_y()/2))/2;
  return x;
}


/**************************************************************
Function: run_cube
Purpose:  runs the random cube demo from the TVOut example code
Args:     int duration - in milliseconds          
Returns:  nothing
Notes:    This is the TVOut code in its own function.
***************************************************************/
void run_cube(int duration){
  
  int currentTime = millis();
  int finishTime = currentTime + duration;
  // The cycleNum variable is only needed if DEBUG is true, 
  // however, to prevent compilier warnings it is left here.
  int cycleNum = 0;
  
  TV.clear_screen();
  TV.print(16,40,"Random Cube");
  TV.print(28,48,"Rotation");
  TV.delay(2000);
  
  // NOTE: if DEBUG is true, then the display will flicker!!!
  if(DEBUG){
    sprintf(TEMPBUFFER, "finishTime = %d", finishTime);
    Serial.println(TEMPBUFFER);
  }
    
  do {
    
    if(DEBUG){
      sprintf(TEMPBUFFER, "Starting cycleNum = %d", cycleNum);
      Serial.println(TEMPBUFFER);
      cycleNum++;
    }
    
    // Sets a random number of steps for any rotation direction
    int rsteps = random(10,60);
    
    //Runs a random direction (one of 6) for the number of rsteps set above.
    switch(random(6)) {
      case 0:
        for (int i = 0; i < rsteps; i++) {
          zrotate(angle);
          printcube();
        }
        break;
      case 1:
        for (int i = 0; i < rsteps; i++) {
            zrotate(2*PI - angle);
            printcube();
          }
          break;
      case 2:
        for (int i = 0; i < rsteps; i++) {
            xrotate(angle);
            printcube();
          }
          break;
      case 3:
        for (int i = 0; i < rsteps; i++) {
          xrotate(2*PI - angle);
          printcube();
        }
        break;
      case 4:
        for (int i = 0; i < rsteps; i++) {
          yrotate(angle);
          printcube();
        }
        break;
      case 5:
        for (int i = 0; i < rsteps; i++) {
          yrotate(2*PI - angle);
          printcube();
        }
        break;
    }
    
    currentTime = millis();
    if(DEBUG){
      sprintf(TEMPBUFFER, "currentTime = %d, finishTime = %d", currentTime, finishTime);
      Serial.println(TEMPBUFFER);
    }
  } while(currentTime < finishTime);
  
}

void intro() {
  unsigned char w,l,wb;
  int index;
  w = pgm_read_byte(TVOlogo);
  l = pgm_read_byte(TVOlogo+1);
  if (w&7)
    wb = w/8 + 1;
  else
    wb = w/8;
  index = wb*(l-1) + 2;

  for ( unsigned char i = 1; i < l; i++ ) {
    TV.bitmap((TV.hres() - w)/2,0,TVOlogo,index,w,i);
    index-= wb;
    TV.delay(50);
  }
  for (unsigned char i = 0; i < (TV.vres() - l)/2; i++) {
    TV.bitmap((TV.hres() - w)/2,i,TVOlogo);
    TV.delay(50);
  }
  TV.delay(3000);
  TV.clear_screen();
}

void printcube() {
  //calculate 2d points
  for(byte i = 0; i < 8; i++) {
    cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (TV.hres()/2));
    cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (TV.vres()/2));
  }
  TV.delay_frame(1);
  TV.clear_screen();
  draw_cube();
}

void zrotate(float q) {
  float tx,ty,temp;
  for(byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    ty = cube3d[i][1] - yOff;
    temp = tx * cos(q) - ty * sin(q);
    ty = tx * sin(q) + ty * cos(q);
    tx = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][1] = ty + yOff;
  }
}

void yrotate(float q) {
  float tx,tz,temp;
  for(byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    tz = cube3d[i][2] - zOff;
    temp = tz * cos(q) - tx * sin(q);
    tx = tz * sin(q) + tx * cos(q);
    tz = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][2] = tz + zOff;
  }
}

void xrotate(float q) {
  float ty,tz,temp;
  for(byte i = 0; i < 8; i++) {
    ty = cube3d[i][1] - yOff;
    tz = cube3d[i][2] - zOff;
    temp = ty * cos(q) - tz * sin(q);
    tz = ty * sin(q) + tz * cos(q);
    ty = temp;
    cube3d[i][1] = ty + yOff;
    cube3d[i][2] = tz + zOff;
  }
}

void draw_cube() {
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[1][0],cube2d[1][1],WHITE);
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[2][0],cube2d[2][1],WHITE);
  TV.draw_line(cube2d[0][0],cube2d[0][1],cube2d[4][0],cube2d[4][1],WHITE);
  TV.draw_line(cube2d[1][0],cube2d[1][1],cube2d[5][0],cube2d[5][1],WHITE);
  TV.draw_line(cube2d[1][0],cube2d[1][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[2][0],cube2d[2][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[2][0],cube2d[2][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[4][0],cube2d[4][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[4][0],cube2d[4][1],cube2d[5][0],cube2d[5][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[6][0],cube2d[6][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[3][0],cube2d[3][1],WHITE);
  TV.draw_line(cube2d[7][0],cube2d[7][1],cube2d[5][0],cube2d[5][1],WHITE);
}
