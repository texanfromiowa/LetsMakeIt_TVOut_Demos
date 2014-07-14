/*
Project: ScoreBoard with TVOut Library and 12 Key KeyPad
 Start Date: 2014-04-13
 Date Updated: 2014-04-21
 Prototyped By: Jim F, Calgary AB, Canada
 */
/**************************************************************
  Name          TVOut_Scoreboard2                              
  Modified By   Bob Powell 
                texanfromiowa@gmail.com
          
  Date      Jul 1, 2014    
  Modified  Jul 7, 2014                                
  Version   ?     
  Arduino   1.5.6-r2
  
  Notes     This is Jim's code, with updates from Bob.
            Most of the updates have to do with using a 
            different keypad since he had a 12 key, and
            I have a 25 key.
            
            The only functional change is to stop the 
            timer when the maxPeriad is reach (ie end of game).
           
            Much of Jim's code that was not needed is 
            commented out. 

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

*/

//=== TVOut Library and Golbal Variables ===
#include <TVout.h>
#include <fontALL.h>
#include <Keypad.h>

TVout TV; // Assign TVOut class to TV
int frame=0; // Counts number of times through the loop() function
// and when it reaches 0 then Refresh the Display

char TEMPBUFFER[63];
byte KEYVAL = 0;
byte LASTKEYVAL = 0;

const byte ROWS = 5; //five rows
const byte COLS = 5; //five columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {132,'0','1','2','3'},    // Back arrow
  {133,'4','5','6','7'},    // DEL (BS)
  {134,'8','9','A','B'},    // Arrow 
  {135,'C','D','E','F'},    // Enter
  {' ',128,129,130,131}     // F0 to F3
};
//////////////////////////////////////////////////////////////
// Use an Arduino Mega 2560 because it has more I/O pins
//
byte rowPins[ROWS] = {43, 41, 39, 37, 35}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {53, 51, 49, 47, 45}; //connect to the column pinouts of the keypad
//
// or use a Cross Roads ATmega1284P board which has 10 I/O pins in a row
//
//byte rowPins[ROWS] = {26, 25, 24, 23, 22}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {31, 30, 29, 28, 27}; //connect to the column pinouts of the keypad
//
// comment out the unused board
//////////////////////////////////////////////////////////////
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


unsigned long keypadPressed=0; // Integer value of the keypad buttons pressed
unsigned long lastButtons=0; // last keypad buttons pressed used for debounce
unsigned long extKey=0; // extended key value
unsigned long shiftKey=0; // shift key value toggle '#' key pressed
unsigned long fnKey=0; // function key value toggle '*' key pressed

//=== Global Variables used for Score Board ===
char tmp[20]="000000000000000"; // Temporary buffer used for formatted output
int teamA=0; // Team A Score
int teamB=0; // Team B Score
int period=1; // Game Period
int maxPeriod=4; // Maximum Number of Periods in the game
int maxTimer=10*60; // Maximum Number of seconds to countdown for timer
int timer1=maxTimer; // Countdown timer for periods
int timerOn=0; // Toggle for start/stop timer

//=== Standard Program Setup Function ======================================
void setup()  {
  
  TV.begin(NTSC,128,96);
  TV.select_font(font8x8);// Select the 8x8 font (This font is Large,Clear,Easy to Read)
  TV.delay(2000); // small delay to let TVOut get setup in memory
}

//=== Standard Program Infinite Loop Function ================================
void loop() {
  getInputs();// Get all kinds of inputs from the I/O pins
  processInputs();// Process the inputs
  TV.delay_frame(1);// Helpful for overall timing when using TVOut functions
}

//==========================================================================
// getInputs calls Keypad input routine for now, other inputs can be added
// for example: IRRemote, momentary buttons, sensors, real-time clock, etc.
void getInputs() {
  KEYVAL = customKeypad.getKey();
}



//========================================================
//== Here is where the heavy-duty processing logic happens
void processInputs() {
  //keypadPressed=keyMap[keyVal];// ASCII value assigned when keypad button pressed
  //keypadPressed=(int)hexaKeys[KEYVAL];// ASCII value assigned when keypad button pressed
  keypadPressed=KEYVAL;// ASCII value assigned when keypad button pressed
  displayKeysPressed();// Display the ASCII values (Note: used for debugging purposes)

  switch (KEYVAL) {
    
    case(48):    // 0
      teamA-=1;  //  Decrement Team A
      break;
    case(49):    // 1
      teamA+=1;  //  Decrement Team A
      break;
    case(50):    // 2
      teamB-=1;  //  Decrement Team B
      break;
    case(51):    // 3
      teamB+=1;  //  Increment Team B
      break;
    case(52):    // 4
      period-=1; //  Decrement Period
      break;
    case(53):    // 5
      period+=1; //  Increment Period
      break;
    case(54):   // 6

      break;
    case(55):   // 7

      break;
    case(56):   // 8

      break;
    case(57):   // 9

      break;
    case(65):   // A

      break;
    case(66):   // B

      break;
    case(67):   // C

      break;
    case(68):   // D

      break;
    case(69):   // E

      break;
    case(70):   // F

      break;
      
    case(32):   // Space

      break;
    case (128): // F0 function key
      timerOn=1-timerOn;  // Turn Timer on/off
      break;
    case (129): // F1 function key

      break;
    case (130): // F2 function key

      break;
    case (131): // F3 function key
      teamA=0; // Reset Team A Score
      teamB=0; // Reset Team B Score
      TV.clear_screen(); // Clear the TVOut screen
      displayScores();// Display the Team A & Team B Scores
      timer1=maxTimer;// Reset the countdown timer
      timerOn=0;
      break;
    case (132):   // <==== arrow

      break;
    case (133):       // DEL

      break;
    case (134):   // ====> arrow

      break;
    case (135):              // Enter

      break;
    default:  // This should never happen
      break;
  }    
  
  
  period=constrain(period,0,4);// Make sure Period value is in the range of 0 to 4
  teamA=constrain(teamA,0,199);// Make sure Team A Score is in the range of 0 to 199
  teamB=constrain(teamB,0,199);// Make sure Team B Score is in the range of 0 to 199
   
/*  
  // When keypadPressed is not equal to lastButtons then process the input values
  if(!(keypadPressed==lastButtons)) {
    if(extKey==176) {// Extended keypressed = "*1" (asterisk then 1)
      timer1=10;// reset countdown timer to 10 seconds
      resetButtonsPressed();
    }
    if(extKey>175) {// Reset extended keypressed and function key pressed
      fnKey=0;
      extKey=0;
      resetButtonsPressed();
    }
    if(extKey=='A') { // Extended keypressed = "#1" (pound then 1)
      timer1=maxTimer; // reset countdown timer
      timerOn=0;// pause the countdown timer
      resetButtonsPressed();
    }
    if(extKey=='B') {// Extended keypressed = "#2"
      timerOn=1-timerOn;
      resetButtonsPressed();
    }
    if(extKey=='C') {// Extended keypressed = "#3"
      teamA=0; // Reset Team A Score
      teamB=0; // Reset Team B Score
      TV.clear_screen(); // Clear the TVOut screen
      displayScores();// Display the Team A & Team B Scores
      timer1=maxTimer;// Reset the countdown timer
      resetButtonsPressed();
    }
    if(extKey=='F') {// Extended keypressed ="#6"
      teamA=0;// Reset Team A Score
      resetButtonsPressed();
    }
    if(extKey=='I') {// Extended keypressed = "#9"
      teamB=0;// Reset Team B Score
      resetButtonsPressed();
    }
    shiftKey+=(lastButtons==35);// If the '#' (pound key) pressed, then shiftKey activated
    fnKey+=(lastButtons==42);// If the '*' (asterisk key) pressed, then the functionKey activated
    extKey=(fnKey>0)*127+(shiftKey>0)*16+keypadPressed;// Assign the extended key value based on fnKey,shiftKey & keypadPressed values
    if( (shiftKey==0) & (fnKey==0) ) {// If the shiftKey and the fnKey are not assigned, then process normal keypad keys pressed
      period+=(keypadPressed=='1')*-1; // Keyapad 1 pressed, decrease period value by 1
      period+=(keypadPressed=='3');// Keypad 3 pressed, increase period value by 1
      teamA+=(keypadPressed=='4')*-1;// Keypad 4 pressed, decrease Team A Score by 1
      teamA+=(keypadPressed=='6');// Keypad 6 pressed, increase Team A Score by 1
      teamB+=(keypadPressed=='7')*-1;// Keypad 7 pressed, decrease Team B Score by 1
      teamB+=(keypadPressed=='9');// Keypad 9 pressed, increase Team B Score by 1
      period=constrain(period,0,4);// Make sure Period value is in the range of 0 to 4
      teamA=constrain(teamA,0,199);// Make sure Team A Score is in the range of 0 to 199
      teamB=constrain(teamB,0,199);// Make sure Team B Score is in the range of 0 to 199
      displayTimer();// Display the Countdown Timer
      displayScores();// Display the Team A & Team B Scores
    }
  }
  lastButtons=keypadPressed;// Used for keypad debounce and shiftKey/functionKey pressed
  resetFnShiftKeys();// if shiftKey or fnKey pressed then Reset them
*/
  if(frame==0) {// If frame counter equals 0 then Refresh and Display the Items
    displayTimer();// Display Countdown Timer
    displayScores();// Display Team A & Team B Scores
    displayLine(4,'-');// Display a row of '-' characters on line 4
    displayLine(7,'-');// Display a row of '-' characters on line 7
  }
  if(timer1==0) {// When the Countdown Timer reaches 0
    if(period != maxPeriod){
      timer1=maxTimer;// Reset the Countdown Timer
      period++;// and Increase the Period Counter by 1
    }
    // stops the timer when maxPeriod is reached
    else
      timerOn=0;
  }
  // When everything is processed,
  // Increase the frame counter by 1 
  // and reset to 0 when frame counter reaches 60
  if(++frame == 60) frame = 0;
}

//=====================================================
//== Resets the keypadPressed and lastButtons pressed
void resetButtonsPressed() {
  lastButtons=0; // reset lastButtons
  keypadPressed=0; // reset keypadPressed
}

//== Resets the fnKey,shiftKey,extKey ===
void resetFnShiftKeys() {
  if(extKey>16) {// Toggle Shift Key off
    shiftKey=0;
  }
  if(extKey>127) {// Toggle Function Key off
    fnKey=0;
  }
}

//==============================================================================
// Displays the integer values of the keypressed and extended keypressed values
void displayKeysPressed() {
  if(KEYVAL != 0)
    LASTKEYVAL = KEYVAL;
    
  //sprintf(tmp,"%04d",keypadPressed);
  sprintf(tmp,"%03d %c",LASTKEYVAL, LASTKEYVAL);
  TV.print(0*10,8*10,tmp);
  //sprintf(tmp,"%04d",extKey);
  //TV.print(7*10,8*10,tmp);
  //delay(1000);
  
  switch (LASTKEYVAL) {
    
    case(48):    // 0
    case(49):    // 1
    case(50):    // 2
    case(51):    // 3
    case(52):    // 4
    case(53):    // 5
    case(54):   // 6
    case(55):   // 7
    case(56):   // 8
    case(57):   // 9
    case(65):   // A
    case(66):   // B
    case(67):   // C
    case(68):   // D
    case(69):   // E
    case(70):   // F
      sprintf(tmp,"     ");
      break;
      
    case (32): // F0 function key
      sprintf(tmp,"Space");
      break;
    case (128): // F0 function key
      sprintf(tmp,"F0   ");
      break;
    case (129): // F1 function key
      sprintf(tmp,"F1   ");
      break;
    case (130): // F2 function key
      sprintf(tmp,"F2   ");
      break;
    case (131): // F3 function key
      sprintf(tmp,"F3   ");
      break;
    case (132):   // <==== arrow
      sprintf(tmp,"<==  ");
      break;
    case (133):       // DEL
      sprintf(tmp,"DEL  ");
      break;
    case (134):   // ====> arrow
      sprintf(tmp,"==>  ");
      break;
    case (135):              // Enter
      sprintf(tmp,"Enter");
      break;
    default:  // This should never happen
      break;
  } 
  
  TV.print(8*10,8*10,tmp);

}

//================================
//== Displays the Countdown Timer
void displayTimer() {
  if(frame==0) {
    if(timerOn==1) {
      timer1--;
      timer1=constrain(timer1,0,999);
    }
  }
  int mins=int(timer1 / 60);
  int secs=int(timer1 % 60);
  sprintf(tmp,"TIMER  -> %02d:%02d",mins,secs);
  TV.print(0*10,0*10,tmp);
  period=constrain(period,0,4);
  sprintf(tmp,"PERIOD -> %02d",period);
  TV.print(0*10,1*10,tmp);
}

//================================
//== Displays a row of characters
void displayLine(int lineNum, char chr) {
  for(int i=0; i<16; i++) {
    TV.print_char(i*8,lineNum*10,chr);
  }  
}

//=========================================
//== Displays Team A Score & Team B Score
void displayScores() {
  sprintf(tmp,"TEAM A -> %03d",teamA);
  TV.print(0*10,2*10,tmp);
  sprintf(tmp,"TEAM B -> %03d",teamB);
  TV.print(0*10,3*10,tmp);
}

