/**************************************************************
  Name      TVOut_Timers                              
  Author    Bob Powell 
            texanfromiowa@gmail.com
            Copyright (C) 2012-2014, Parallelus Automation, Inc.
          
  Date      Jun 24, 2014    
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

char TEMPBUFFER[63];
TVout TV;
// For an UNO 120x96 is as large as possible (at least that is what it seems)
int WIDTH = 136;  // must be divisable by 8
int HEIGHT = 96;

/**************************************************************
Function: setup
Purpose:  set up Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino
***************************************************************/
void setup() {

  ////Serial.begin(115200);
  ////Serial.println("\n\nStarting...\n");
  
  TV.begin(NTSC,WIDTH,HEIGHT);
  //TV.select_font(font6x8);
  TV.clear_screen();
  TV.set_font(6);

  letsmakeit_intro();  

  TV.set_font(6);

  randomSeed(analogRead(0));
  
}

/**************************************************************
Function: loop
Purpose:  loop funtion for Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
          
          Runs several examples of putting text on the
          TV screen.
***************************************************************/
void loop() {

  int minutes, seconds, x, y, x_char, y_char, charNum;

  // 
  simple_matrix_effect(6,150,250);

  // Size 6 font, 10millisecond delay, 2000 cycles
  random_chars(6,10,2000);
  
  
  // Count from 0 to 200, and back to 0
  TV.clear_screen();
  TV.set_font(6);
  TV.print("\nCounting");
  for(int i = 0; i <= 200; i++){
    sprintf(TEMPBUFFER,"%d", i);
    x = 30 - strlen(TEMPBUFFER)*TV.get_font_x();
    TV.print(x,25,TEMPBUFFER);
    TV.delay(15);
  }  
  TV.delay(1000);
  for(int i = 199; i >= 0; i--){
    sprintf(TEMPBUFFER,"%d", i);
    x = 30 - strlen(TEMPBUFFER)*TV.get_font_x();
    TV.print(0,25,"          ");
    TV.print(x,25,TEMPBUFFER);
    TV.delay(15);
  }
  TV.delay(2000);
  TV.clear_screen();
  TV.delay(2000);
   
     
  // Scroll lines
  TV.clear_screen();
  TV.set_font(6);  
  sprintf(TEMPBUFFER, "Scroll line # 1");
  TV.print(TEMPBUFFER);
  TV.delay(500);
  for(int i = 2; i < 13; i++){
    sprintf(TEMPBUFFER, "\nScroll line # %d", i);
    TV.print(TEMPBUFFER);
    TV.delay(500);
  }
  TV.delay(500);
  TV.print("\nOops, lost top line");
  TV.delay(5000);
  TV.clear_screen();
  
  // Scroll lines fast
  sprintf(TEMPBUFFER, "Fast scrolling # 1");
  TV.print(TEMPBUFFER);
  TV.delay(500);
  for(int i = 2; i <= 25; i++){
    sprintf(TEMPBUFFER, "\nFast scrolling # %d", i);
    TV.print(TEMPBUFFER);
    TV.delay(250);
  }
  TV.delay(5000);
  TV.clear_screen();
  
    
  // Count down from 70sec to 0
  TV.print("\nTime coundown:\n");
  TV.set_font(4);
  TV.print("\n (70sec to 0)");
  TV.set_font(6);
  for(int i = 70; i >= 0; i--){
    minutes = i / 60; 
    seconds = i % 60;
    sprintf(TEMPBUFFER, "%02d:%02d", minutes, seconds);
    TV.print(10,35,TEMPBUFFER);
    TV.delay(1000);
  }  
  TV.delay(2000);
  TV.clear_screen();
  TV.delay(2000);


  // Count up from 0 to 70sec
  TV.print("\nTime count up:\n");
  TV.set_font(4);
  TV.print("\n (0 to 70sec)");
  TV.set_font(6);
  for(int i = 0; i < 70; i++){
    minutes = i / 60; 
    seconds = i % 60;
    sprintf(TEMPBUFFER, "%02d:%02d", minutes, seconds);
    TV.print(10,35,TEMPBUFFER);
    TV.delay(1000);
  }  
  TV.delay(2000);
  TV.clear_screen();
  TV.delay(2000);
 
}


/**************************************************************
Function: random_chars
Purpose:  place random chars on the screen
Args:     int font_size - 4, 6, or 8 from the font library
          int duration - how long to delay between cycles
          int steps - number of cycles
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
***************************************************************/
void random_chars(int font_size, int duration, int steps)
{
  TV.clear_screen();
  TV.set_font(font_size);
  
  int x, y, x_char, y_char, charNum, numCharsX, numCharsY;
 
  numCharsX = (WIDTH - 1) / TV.get_font_x();    
  numCharsY = HEIGHT / TV.get_font_y();  
    
  // Random chars on screen
  sprintf(TEMPBUFFER, "Randon characters");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)-(TV.get_font_y() * 1.5), TEMPBUFFER);
  sprintf(TEMPBUFFER, "placed on screen");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)-(TV.get_font_y() * 0.5), TEMPBUFFER);
  sprintf(TEMPBUFFER, "in a grid pattern");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)+(TV.get_font_y() * 0.5), TEMPBUFFER);
  TV.delay(3000);
  
  for(int i = 0; i < steps; i++){
    x_char = random(numCharsX);  
    y_char = random(numCharsY); 
    charNum = random(32,129); 
    x = x_char * font_size;
    y = y_char * 8;
    sprintf(TEMPBUFFER, "%c", charNum);
    TV.print(x,y,TEMPBUFFER);   
    TV.delay(duration); 
  }  
  
  TV.delay(3000);
  TV.clear_screen();
  //TV.set_font(8);
  sprintf(TEMPBUFFER, "That was fun!");
  x = (WIDTH/2) - (strlen(TEMPBUFFER)*TV.get_font_x())/2;
  y = (HEIGHT/2);
  TV.print(x, y, TEMPBUFFER);
  //TV.set_font(6);
  TV.delay(5000);
  TV.clear_screen();
  TV.delay(1000);

}

/**************************************************************
Function: simple_matrix_effect
Purpose:  place random chars on the screen
Args:     int font_size - 4 or 6 from the font library
                          (font 8 is not setup yet)
          int duration - how long to delay between cycles
          int steps - number of cycles
Returns:  nothing
Notes:    A simple version of the Matrix movie effect to 
          demo how it could be done.  
***************************************************************/
void simple_matrix_effect(int font_size, int duration, int cycles)
{
  
  TV.clear_screen();
  TV.set_font(font_size);
  
  int x, y, x_char, y_char, charNum, numCharsX, numCharsY;
 
  // Set the number of chars in X and Y  
  numCharsX = (WIDTH - 1) / TV.get_font_x();    
  numCharsY = HEIGHT / TV.get_font_y();  
    
  // create variables for each column 
  int cursor_y[numCharsX];     // number of rows
  int cursor_speed[numCharsX]; // speed for each columns
  int cursor_delay[numCharsX]; // speed counter for each column

  // show intro
  sprintf(TEMPBUFFER, "Simple");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)-(TV.get_font_y() * 0.5), TEMPBUFFER);
  sprintf(TEMPBUFFER, "Matrix Effect");
  TV.print(get_centered_x(TEMPBUFFER), get_centered_y(TEMPBUFFER)+(TV.get_font_y() * 0.5), TEMPBUFFER);
  TV.delay(3000);
  TV.clear_screen();

  
  // Set an initial, random speed multiplier for each column
  for(int i = 0; i < numCharsX; i++){
    cursor_speed[i] = random(10); 
    cursor_delay[i] = cursor_speed[i];
    cursor_y[i] = 0;
  }


  int i, j;
  
  for(i = 0; i < cycles; i++){
    for(j = 0; j < numCharsX; j++){
        
      // time to print
      if(cursor_delay[j] == 0){
        
        // reset cursor delay
        cursor_delay[j] =  cursor_speed[j];    
        
        // calculate position  
        x = j * TV.get_font_x();
        y = cursor_y[j] * TV.get_font_y();
        sprintf(TEMPBUFFER, "%c", 128);
        TV.print(x, y, TEMPBUFFER);
        
        if(cursor_y[j] == 0){
          charNum = 128;
        }
        else{
          charNum = random(33,127); 
        }  
        

       if(cursor_y[j] > 0){  
         y = (cursor_y[j]-1) * TV.get_font_y();
         sprintf(TEMPBUFFER, "%c", charNum);
         TV.print(x, y, TEMPBUFFER);
       }
       
       cursor_y[j] = cursor_y[j] + 1;        
        
       // Reached the last row in a column -  reset cursor 
       // position, speed for next column drop, and make
       // the last char a random char 
       if(cursor_y[j] == numCharsY){
         charNum = random(33,127);
         y = (numCharsY-1) * TV.get_font_y();
         sprintf(TEMPBUFFER, "%c", charNum);
         TV.print(x, y, TEMPBUFFER);
         cursor_y[j] = 0;
         cursor_speed[j] = random(10); 
         cursor_delay[j] = cursor_speed[j];
       }  
        
     }
      
      // cursor_delay[j] != 0)
      else{
        // decrement the delay counter
        cursor_delay[j] = cursor_delay[j] - 1;
      }   


    }

    TV.delay(duration);
  }

  TV.delay(3000);
  TV.clear_screen();
  sprintf(TEMPBUFFER, "That was fun!");
  x = (WIDTH/2) - (strlen(TEMPBUFFER)*TV.get_font_x())/2;
  y = (HEIGHT/2);
  TV.print(x, y, TEMPBUFFER);
  //TV.set_font(6);
  TV.delay(5000);
  TV.clear_screen();
  TV.delay(1000);

}
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



