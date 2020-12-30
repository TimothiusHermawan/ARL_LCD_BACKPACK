/*------------------------------------------------------------------------------------------------------------------
 |  Author      : Timothius Hermawan                                                                                |
 |  Github      : https://github.com/TimothiusHermawan/ARL_LCD_BACKPACK                                             |
 |  Description : commanding lcd backpack to measure adc continuously and reading the result                        |
 |  Pin         : SDA(A4), SCL(A5), Interrupt(2)                                                                    |
 ------------------------------------------------------------------------------------------------------------------*/
 

#include <Wire.h>
#include <ARL_LCD_BACKPACK.h>

const uint8_t ADDR = 0x3F;          //Default lcdbackpack address
const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(ADDR,interruptPin);

unsigned char printBuffer[80];

uint8_t redVal=200;
uint8_t greenVal=200;
uint8_t blueVal=200;

char contrastVal=70;

uint8_t adcHigh;
uint8_t adcLow;
uint8_t intInfo;
uint8_t buttonState;

uint8_t adcPtr=1;
uint16_t adcVal1=0;
uint16_t adcVal2=0;

unsigned long adcTimer=0;


void setup() {
lcd.begin();        //I2C running in default speed(100kHz)
//lcd.begin(400000);  //I2C running in custom speed(Arduino Uno max speed: 400kHz)

lcd.init();         //initialize lcd
lcd.setBacklightColor(redVal,greenVal,blueVal); //Setting lcd backlight color
lcd.setContrast(contrastVal);    //Setting lcd text contrast

//Printing text on lcd
lcd.setCursor(1,1);
lcd.print("ADC1:      /4096");
lcd.setCursor(1,2);
lcd.print("ADC2:      /4096");
adcTimer=millis();
}

void loop() {
  if(millis()-adcTimer>200){    //printing ADC value to lcd every 200ms
    lcd.setCursor(7,1);
    sprintf(printBuffer, "%04u",adcVal1);
    lcd.print(printBuffer);
    lcd.setCursor(7,2);
    sprintf(printBuffer, "%04u",adcVal2);
    lcd.print(printBuffer);
    lcd.setCursor(16,2);
    adcVal1=0;
    adcVal2=0;
    adcPtr=1;
    adcTimer=millis();
   }
  else{   
   if(adcPtr==1){
    lcd.adcRead(1,15,1);  //Sending command for ADC1 measurement(channel(1-2), speed(15(slowest)-0(fastest)), result format(0(left aligned),1(right aligned)))
    adcPtr=0;
    }
   else if(adcPtr==2){
    lcd.adcRead(2,0,0);   //Sending command for ADC2 measurement(channel(1-2), speed(15(slowest)-0(fastest)), result format(0(left aligned),1(right aligned)))
    adcPtr=0;
    } 
  }
  
  if(lcd.interrupt()){ //detecting interrupt from backpack
    intInfo = lcd.read(7);          //reading interrupt info from register address 7

    if(intInfo == 2){               //receive ADC1 result
      adcHigh = lcd.read(9);        //reading adc higher byte from register address 9
      adcLow = lcd.read(10);        //reading adc lower byte from register address 10
      
      //combining 2 character data type into 1 integer (for right aligned result format)
      adcVal1 = (unsigned char)adcHigh;
      adcVal1 =  adcVal1<<8; 
      adcVal1 |= (unsigned char)adcLow;
      
      adcPtr=2;
    }
    if(intInfo == 4){                 //receive ADC2 result
      adcHigh = lcd.read(9);          //reading adc higher byte from register address 9
      adcLow = lcd.read(10);          //reading adc lower byte from register address 10

      //combining 2 character data type into 1 integer (for right aligned result format)
      adcVal2 = (unsigned char)adcHigh;
      adcVal2 =  adcVal2<<4;
      adcVal2 |= (unsigned char)adcLow;
      
      adcPtr=0;
    }
    }
  else{}
        
}