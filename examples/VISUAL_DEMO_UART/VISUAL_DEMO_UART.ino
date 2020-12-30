/*------------------------------------------------------------------------------------------------------------------
 |  Author      : Timothius Hermawan                                                                                |
 |  Github      : https://github.com/TimothiusHermawan/ARL_LCD_BACKPACK                                             |
 |  Description : Visualizing ADL LCD BACKPACK features, Creating custom character                                  |
 |  Pin         : -(Software Serial)  Rx(10), Tx(11), Interrupt(2)                                                  |
 |                -(Hardware Serial)  Rx(0), Tx(1), Interrupt(2)                                                    |
 |  Notes       : Create custom character array at https://maxpromer.github.io/LCD-Character-Creator/               |
 ------------------------------------------------------------------------------------------------------------------*/

#include <ARL_LCD_BACKPACK.h>

//Communication using Software Serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);
const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(mySerial,interruptPin);

//Communication using Hardware Serial
/*const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(Serial,interruptPin);*/

uint8_t colorCounter=0;
uint8_t redValue=200;
uint8_t greenValue=200;
uint8_t blueValue=200;

uint8_t contractCounter;
uint8_t contrastValue=70;

//custom character array
byte locked[] = {0x00,0x0E,0x11,0x11,0x1F,0x1B,0x1B,0x1F};
byte unlocked[] = {0x0E,0x11,0x10,0x10,0x1F,0x1B,0x1B,0x1F};
byte heart[] = {0x00,0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00};
byte emptybattery[] = {0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x1F};
byte halffullbattery[] = {0x0E,0x11,0x11,0x11,0x1F,0x1F,0x1F,0x1F};
byte fullbattery[] = {0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
byte music[] = {0x00,0x00,0x0F,0x09,0x09,0x0B,0x1B,0x18};
byte thermometer[] = {0x04,0x0E,0x0A,0x0A,0x0E,0x1F,0x1F,0x0E};
byte ohm[] = {0x00,0x0E,0x11,0x11,0x0A,0x1B,0x00,0x00};
byte eyes1[] = {0x00,0x00,0x04,0x0E,0x0E,0x04,0x00,0x00};
byte eyes2[] = {0x00,0x00,0x00,0x0C,0x12,0x00,0x00,0x00};
byte eyes3[] = {0x00,0x00,0x00,0x06,0x09,0x00,0x00,0x00};
byte mouth1[] = {0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00};
byte mouth2[] = {0x00,0x08,0x18,0x04,0x03,0x00,0x00,0x00};
byte mouth3[] = {0x00,0x02,0x03,0x04,0x18,0x00,0x00,0x00};
byte smileemoji[] = {0x00,0x00,0x0A,0x00,0x11,0x0E,0x00,0x00};

void setup() {
lcd.begin(9600);             //uart baudrate is 9600
  
lcd.init();
lcd.cursorOff();
lcd.setBacklightColor(redValue,greenValue,blueValue);
lcd.setContrast(contrastValue);

}

void loop() {
//---------------------------------------------------Printing smiling face-----------------------------------------------
lcd.createChar(0,eyes1);//create character "eyes1" at memory address 0 (memory location(0-7), character array)
lcd.createChar(1,mouth1);
lcd.createChar(2,eyes2);
lcd.createChar(3,eyes3);
lcd.createChar(4,mouth2);
lcd.createChar(5,mouth3);
lcd.createChar(6,emptybattery);
lcd.createChar(7,halffullbattery);
lcd.returnHome();      //return home after creating custom char (MUST!)
  
//display straight face
lcd.setCursor(8,1);
lcd.printChar(0);      //printing custom character at memory location 0
lcd.printChar(0);
lcd.setCursor(8,2);
lcd.printChar(1);      //printing custom character at memory location 1
lcd.printChar(1);
delay(1000);

//dislay smiling face
lcd.setCursor(8,1);
lcd.printChar(2);
lcd.printChar(3);
delay(1000);
lcd.setCursor(8,2);
lcd.printChar(4);
lcd.printChar(5);
delay(1000);

lcd.clear();
//-----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------Print Welcome-----------------------------------------------------
lcd.setCursor(1,1);
lcd.print("Welcome to LCD");
lcd.setCursor(1,2);
lcd.print("Backpack Guide!");
delay(2000);
//-----------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------rgb control------------------------------------------------------
lcd.setCursor(1,1);
lcd.print("You can control");
lcd.setCursor(1,2);
lcd.print("backlight color,");
delay(1000);

colorCounter=10;
while(colorCounter>0){
  redValue-=20;
  lcd.setBacklightColor(redValue,greenValue,blueValue);
  colorCounter--;    
  delay(100);
}
colorCounter=10;
while(colorCounter>0){
  redValue+=20;
  greenValue-=20;
  lcd.setBacklightColor(redValue,greenValue,blueValue); 
  colorCounter--;   
  delay(100);
}
colorCounter=10;
while(colorCounter>0){
  greenValue+=20;
  blueValue-=20;
  lcd.setBacklightColor(redValue,greenValue,blueValue);  
  colorCounter--;  
  delay(100);
}
colorCounter=10;
while(colorCounter>0){
  blueValue+=20;
  lcd.setBacklightColor(redValue,greenValue,blueValue); 
  colorCounter--;   
  delay(100);
}
lcd.clear();
//-----------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------Contrast Control---------------------------------------------------
lcd.setCursor(1,1);
lcd.print("Digital control");
lcd.setCursor(1,2);
lcd.print("text Contrast,");
delay(1000);
while(contrastValue>0){
  contrastValue-=10;
  lcd.setContrast(contrastValue);
  delay(300);
  }
while(contrastValue<70){
  contrastValue+=10;
  lcd.setContrast(contrastValue);
  delay(100);
  }
delay(1000);
lcd.clear();

//-----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------Playing Ringtone----------------------------------------------------
lcd.setCursor(1,1);
lcd.print("Play our 8");
lcd.setCursor(1,2);
lcd.print("presets ringtone,");
delay(500);
lcd.playTone(6);
lcd.clear();

//-----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------Create Custom Character-----------------------------------------------
lcd.createChar(0,locked);//create character "locked" at memory address 0 (memory location(0-7), character array)
lcd.createChar(1,unlocked);
lcd.createChar(2,heart);
lcd.createChar(3,emptybattery);
lcd.createChar(4,fullbattery);
lcd.createChar(5,ohm);
lcd.createChar(6,music);
lcd.createChar(7,thermometer);
lcd.returnHome();      //return home after creating custom char (MUST!)


//lcd.setCursor(1,1);
lcd.print("Create new char,");
lcd.setCursor(1,2);
lcd.printChar(0);      //printing custom character at memory location 0
lcd.printChar(' ');
lcd.printChar(1);      //printing custom character at memory location 1
lcd.printChar(' ');
lcd.printChar(2);
lcd.printChar(' ');
lcd.printChar(3);
lcd.printChar(' ');
lcd.printChar(4);
lcd.printChar(' ');
lcd.printChar(5);
lcd.printChar(' ');
lcd.printChar(6);
lcd.printChar(' ');
lcd.printChar(7);
lcd.createChar(5,halffullbattery);
lcd.returnHome();

delay(2000);
lcd.clear();
//-----------------------------------------------------------------------------------------------------------------------

//--------------------------------------------measure ADC (display only)-------------------------------------------------
lcd.setCursor(1,1);
lcd.print("It also has 2");
lcd.setCursor(1,2);
lcd.print("12-bit ADC  ,");


lcd.moveCursorLeft();
lcd.moveCursorLeft();
lcd.printChar(3);
delay(1000);
lcd.moveCursorLeft();
lcd.printChar(5);
delay(1000);
lcd.moveCursorLeft();
lcd.printChar(4);
delay(1000);
lcd.moveCursorLeft();
lcd.printChar(5);
delay(500);
lcd.moveCursorLeft();
lcd.printChar(3);
delay(500);
lcd.clear();
//-----------------------------------------------------------------------------------------------------------------------

//-------------------------------------------button input (display only)------------------------------------------------
lcd.setCursor(1,1);
lcd.print("and finally");
delay(500);
lcd.printChar('.');
delay(500);
lcd.printChar('.');
delay(500);
lcd.printChar('.');
delay(500);

lcd.setCursor(1,2);
lcd.print("4 button input!");
delay(2000);

lcd.createChar(0,smileemoji);
lcd.returnHome();
lcd.clear();
lcd.print("Have fun! ");
lcd.printChar(0);

delay(3000);
lcd.clear();
//-----------------------------------------------------------------------------------------------------------------------
 
}