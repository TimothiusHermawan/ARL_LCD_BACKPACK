/*------------------------------------------------------------------------------------------------------------------
 |  Author      : Timothius Hermawan                                                                                |
 |  Github      :                                                                                                   |
 |  Description : library function description                                                                      |                                                  |
 |  Pin         : -(Software Serial)  Rx(10), Tx(11), Interrupt(2)                                                  |
 |                -(Hardware Serial)  Rx(0), Tx(1), Interrupt(2)                                                    |
 ------------------------------------------------------------------------------------------------------------------*/

#include <ARL_LCD_BACKPACK.h>

//Communication using Software Serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);     //(rx,tx)
const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(mySerial,interruptPin);

//Communication using Hardware Serial
/*const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(Serial,interruptPin);*/

void setup() {
lcd.begin(9600);             //uart baudrate is 9600

lcd.init();                  // initialize character LCD (2 or 4 row)
lcd.setContrast(70);         // Set character LCD display contrast (0-100)
lcd.setBacklightColor(255,255,255);// Set character LCD display backlight color (red(0-255), green(0-255), blue(0-255))     

lcd.setCursor(1,1);          //set cursor position (x(1-16),y(1-2))
lcd.print("a");              //print sentence 
lcd.setCursor(1,2);
lcd.print("ARL LCD BACKPACK");

//----------------------------------------------------------------------------other Function----------------------------------------------------------------------------

//character lcd function

//lcd.init1Row();             //initialize 1 row character lcd
//lcd.init();                 // initialize character LCD (2 or 4 row)
//lcd.printChar('x');         //print a character 
//lcd.print("x");             //print sentence 
//lcd.command(0xXX);          //send command to lcd
//lcd.setCursor(x,y);         //set cursor position (x(1-16),y(1-2))
//lcd.clear();                //clear display
//lcd.shiftDisplayLeft();     //move character lcd Display 1 box left
//lcd.shiftDisplayRight();    //move character lcd Display 1 box right
//lcd.returnHome();           //return cursor to first line first box and all shifted display back to place
//lcd.displayOff();           //make character lcd blank (content will not lost)
//lcd.displayOn();            //make character lcd show its content
//lcd.cursorOff();            //hide cursor
//lcd.cursorOn();             //show cursor
//lcd.blinkOff();             //hide blink
//lcd.blinkOn();              //show blink
//lcd.moveCursorLeft();       //move character LCD cursor left
//lcd.moveCursorRight();      //move character LCD cursor right
//lcd.writingLeftToRight();   //text moving right (left align)
//lcd.writingRightToLeft();   //text moving left (right align)
//lcd.autoCursorShift();      //cursor move to right after writing character
//lcd.noCursorShift();        //cursor doesnt move after writing character
//lcd.createChar(x[]);        //create custom character

//Backpack features

//lcd.setContrast(value);     // Set character LCD display contrast value(0-100)
//lcd.setBacklightColor(red,green,blue);    // Set character LCD display backlight color (red(0-255), green(0-255), blue(0-255))     
//lcd.backlightOff();         //turn lcd backlight off
//lcd.backlightOn();          //turn lcd backlight on
//lcd.adcRead(pin,speed,format);            //command lcd backpack to read ADC (pin(1-2), speed(0-15), format(0(left aligned)-1(left aligned))
//lcd.playTone(tone);         //command backpack to play a ringtone tone(1-8)
//lcd.interrupt();            //reading interrupt from lcd backpack

}

void loop() {
}