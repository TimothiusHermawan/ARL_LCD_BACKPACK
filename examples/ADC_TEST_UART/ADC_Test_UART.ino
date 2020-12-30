/*------------------------------------------------------------------------------------------------------------------
 |  Author      : Timothius Hermawan                                                                                |
 |  Github      :                                                                                                   |
 |  Description : commanding lcd backpack to measure adc continuously and reading the result                        |                                                  |
 |  Pin         : -(Software Serial)  Rx(10), Tx(11), Interrupt(2)                                                  |
 |                -(Hardware Serial)  Rx(0), Tx(1), Interrupt(2)                                                    |
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

unsigned char printBuffer[80];

uint8_t redVal=200;
uint8_t greenVal=200;
uint8_t blueVal=200;

char contrastVal=70;

uint8_t adcPtr=1;
int i = 0;

uint8_t recvData[8]={0};
uint16_t adcVal1=0;
uint16_t adcVal2=0;

unsigned long adcTimer=0;


void setup() {
lcd.begin(9600);             //uart baudrate is 9600

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

  
  if(lcd.interrupt()){//detecting interrupt from backpack

    //requesting interrupt information
    lcd.write(0x5A);
    lcd.write(0xFE);
    lcd.write(0x05);
    lcd.write(0xF0);
    lcd.write(0x0F);
    }
  else{}

  if(lcd.available()){
    recvData[i] = (uint8_t)lcd.read();
    if(recvData[0] == 0x5A){//finding package header(1 byte)
      i++;
    }
    else{}
    
    if(i>1){
      if(recvData[i-1] == 0x0F && recvData[i-2] == 0xF0){//finding package end (2 bytes)
        if(recvData[1] == 2){                         //receive ADC1 result
          
          //combining 2 character data type into 1 integer (for right aligned result format)
          adcVal1 = ((unsigned int)recvData[2])&0x00FF;
          adcVal1 =  adcVal1<<8;
          adcVal1 |= ((unsigned int)recvData[3])&0x00FF;
            
          adcPtr=2;
        }
        else if(recvData[1] == 4){                         //receive ADC2 result
          
          //combining 2 character data type into 1 integer (for right aligned result format)
          adcVal2 = ((unsigned int)recvData[2])&0x00FF;
          adcVal2 =  adcVal2<<4;
          adcVal2 |= ((unsigned int)recvData[3])&0x00FF;
          
          adcPtr=0;
        }
        recvData[1] = 0;
        recvData[2] = 0;
        recvData[3] = 0;
        recvData[4] = 0;
        recvData[5] = 0;
        recvData[6] = 0;
        recvData[7] = 0;
        recvData[0] = 0; 
        i=0;
      }    
    }
    else{}  
  } 
}