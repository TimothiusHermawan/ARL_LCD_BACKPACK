/*------------------------------------------------------------------------------------------------------------------
 |  Author      : Timothius Hermawan                                                                                |
 |  Github      : https://github.com/TimothiusHermawan/ARL_LCD_BACKPACK                                             |
 |  Description : ARL LCD BACKPACK interractive demo with button input                                              |
 |  Pin         : SDA(A4), SCL(A5), Interrupt(2)                                                                    |
 ------------------------------------------------------------------------------------------------------------------*/

   
#include <Wire.h>
#include <ARL_LCD_BACKPACK.h>

const uint8_t ADDR = 0x3F;          //Default lcdbackpack address
const uint8_t interruptPin = 2;     //interrupt pin for arduino uno: digital pin 2&3
ARL_LCD_BACKPACK lcd(ADDR,interruptPin);

unsigned char printBuffer[80];
const uint8_t adcAvg = 50;

char recvPtr = 0;
uint8_t buttonHold = 0;

uint8_t adcHigh;
uint8_t adcLow;
uint8_t intInfo;
uint8_t buttonState;

char contrastVal=70;

char rgbPtr=0;
uint8_t redVal=200;
uint8_t greenVal=200;
uint8_t blueVal=200;

unsigned int adcVal=0;

int displayCounter=100;

uint8_t tonePtr = 1;

//page:
//0: contrast test
//1: RGB Backlight test
//2: ADC Test
//3: Buzzer Test
//4: Button Test
char page = 0;

unsigned long buttonTimer = 0;

void contrastTest (uint8_t val,char *curVal);
void rgbTest (uint8_t val,uint8_t *rVal,uint8_t *gVal,uint8_t *bVal);
void buttonTest(uint8_t val,int *counterVal, uint8_t buttonHoldInfo);
void playSound(uint8_t val,uint8_t *toneVal);

  /*LCD AXL Backpack Demo Guide
   
  
  Page 0: Contrast
  Button 1 Press  : Next Page
  Button 2 Press  : Decrease Contrast by 10%
  Button 2 Hold   : Decrease Contrast by 10% Every 0.5s
  Button 3 Press  : Increase Contrast by 10%
  Button 3 Hold   : Increase Contrast by 10% Every 0.5s
  
  Page 1: Backlight Color
  Button 1 Press  : Next Page
  Button 2 Press  : Decrease Color Brightness by 5
  Button 2 Hold   : Decrease Color Brightness by 5 Every 0.2s
  Button 3 Press  : Increase Color Brightness by 5
  Button 3 Hold   : Increase Color Brightness by 5 Every 0.2s
  Button 4 Press  : Adjust Next Color
  
  Page 2: ADC Measurement*
  Button 1 Press  : Next Page
  Button 2 Press  : Measure ADC1
  Button 3 Press  : Measure ADC2
  *Display average of 50 measurement result
   
  Page 3: Buzzer
  Button 1 Press  : Next Page
  Button 2 Press  : Previous Ringtone
  Button 3 Press  : Next Ringtone
  Button 4 Press  : Play Ringtone

  Page 4: Button
  Button 1 Press  : Return to Page 0
  Button 2 Press  : Decrease Counter by 1
  Button 2 Hold   : Decrease Counter by 10 Every 0.1s
  Button 3 Press  : Increase Counter by 1
  Button 3 Hold   : Increase Counter by 10 Every 0.1s
  Button 4 Press  : Reset Counter Value to 100
  Button 4 Hold   : Set Counter Value to 0
  Button 2&3 Press: Set Counter Value to 1000
  Button 2&4 Press: Decrease Counter by 10
  Button 3&4 Press: Increase Counter by 10 */ 



void setup() {

lcd.begin();        //I2C running in default speed(100kHz)
//lcd.begin(250000);  //I2C running in custom speed(recomended max speed: 250kHz)

  lcd.init();         //initialize lcd
  lcd.setBacklightColor(redVal,greenVal,blueVal); //Setting lcd backlight color
  lcd.setContrast(contrastVal);    //Setting lcd text contrast
  
  //Printing text on lcd
  lcd.print("Contrast"); 
  lcd.setCursor(1,2);
  lcd.print("Value: ");
  sprintf(printBuffer, "%3d", (unsigned int)contrastVal);
  lcd.setCursor(8,2);
  lcd.print(printBuffer);
  lcd.playTone(8);              //Playing ringtone 8(Startup system)
}

void loop() {
if(lcd.interrupt()){ //detecting innterrupt from backpack
intInfo = lcd.read(7); //reading interrupt info from register address 7

if(intInfo & 1){ //button pressed Interrupt
  lcd.playTone(1);
  buttonState = lcd.read(8);
    if(buttonState == 1){
      page++;
      if (page>4){
        page = 0;
        }
      else{}

      if(page == 0){                                        //Displaying Contrast Test
        displayCounter=100;
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Contrast"); 
        lcd.setCursor(1,2);
        lcd.print("Value: ");
        sprintf(printBuffer, "%3d", (unsigned int)contrastVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);
        }  
        
      else if(page == 1){                                   //Displaying RGB Backlight Test
        contrastVal = 70;
        lcd.clear();
        lcd.setContrast(contrastVal);
        lcd.setCursor(1,1);
        lcd.print("Backlight: Red   ");
        lcd.setCursor(1,2);
        lcd.print("R");
        sprintf(printBuffer, "%3d", (unsigned int)redVal);
        lcd.print(printBuffer);
        lcd.setCursor(6,2);
        lcd.print("G");
        sprintf(printBuffer, "%3d", (unsigned int)greenVal);
        lcd.print(printBuffer);

        lcd.setCursor(11,2);
        lcd.print("B");
        sprintf(printBuffer, "%3d", (unsigned int)blueVal);
        lcd.print(printBuffer);
        lcd.setCursor(1,2);
        } 
        
       else if(page == 2){                                //Displaying ADC Measurement test                            
        rgbPtr = 0;
        redVal=200;
        greenVal=200;
        blueVal=200;
        lcd.setBacklightColor(redVal, greenVal, blueVal);
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("ADC  Measurement ");
        lcd.setCursor(1,2);
        lcd.print("ADC_:");
        } 

        else if(page == 3){                                  //Displaying  Buzzer Test
        adcVal = 0;  
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Buzzer   ");
        lcd.setCursor(1,2);
        lcd.print("Sound:");
        sprintf(printBuffer, "%d", (unsigned int)tonePtr);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);
        }
        else if(page == 4){                                   //Displaying button test
        tonePtr =1;
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Button:");
        lcd.setCursor(1,2);
        lcd.print("Counter:  0100");
        }
       }
       else{                                            
        
        if (page ==0){
          contrastTest (buttonState, &contrastVal);//Contrast test 
          }
        else if(page ==1){
          rgbTest (buttonState,&redVal,&greenVal,&blueVal);//RGB backlight test 
          }

         else if(page == 2){ 
            if(buttonState== 2){
               lcd.adcRead(1,15,1);//Sending command for ADC2 measurement(channel(1-2), speed(15(slowest)-0(fastest)), result format(0(left aligned),1(right aligned)))
              }
             else if(buttonState ==4){
               lcd.adcRead(2,0,0);//Sending command for ADC2 measurement(channel(1-2), speed(15(slowest)-0(fastest)), result format(0(left aligned),1(right aligned)))
           }
         }  
        else if(page ==3){
          playSound(buttonState,&tonePtr);//buzzer test
          }
        else if(page ==4){                          
          buttonTest(buttonState,&displayCounter,buttonHold);//button test
          } 
         }
        }

    if(intInfo & 2){        //ADC1 interrupt
          adcHigh = lcd.read(9);        //reading adc higher byte from register address 9
          adcLow = lcd.read(10);        //reading adc lower byte from register address 10

          //combining 2 character data type into 1 integer (for right aligned result format)
          adcVal = adcHigh;
          adcVal =  adcVal<<8;
          adcVal |= adcLow;
        lcd.setCursor(1,2);
        lcd.print("ADC1:");
        sprintf(printBuffer, "%04u /4096",adcVal);
        lcd.print(printBuffer);
        }
    if(intInfo & 4){        //ADC2 interrupt
          adcHigh = lcd.read(9);          //reading adc higher byte from register address 9
          adcLow = lcd.read(10);          //reading adc lower byte from register address 10

          //combining 2 character data type into 1 integer (for left aligned result format)
          adcVal = adcHigh;
          adcVal =  adcVal<<4;
          adcVal |= adcLow;

        lcd.setCursor(1,2);
        lcd.print("ADC2:");
        sprintf(printBuffer, "%04u /4096",adcVal);
        lcd.print(printBuffer);
      }    

      intInfo = 0;
      buttonState = 0;
      adcHigh = 0;
      adcLow = 0;
  }

  if(buttonHold !=0){   //handling button hold

  if(page==0){
    if (millis()-buttonTimer>500){
    if(buttonHold == 1){
      contrastTest (2, &contrastVal);
      buttonTimer = millis();
      }
    else if(buttonHold == 2){
      contrastTest (4, &contrastVal);
      buttonTimer = millis();
      }
    }     
   }
  else if(page==1){
    if (millis()-buttonTimer>200){
    if(buttonHold == 1){
      rgbTest(2,&redVal,&greenVal,&blueVal);
      buttonTimer = millis();
      }
    else if(buttonHold == 2){
      rgbTest(4,&redVal,&greenVal,&blueVal);
      buttonTimer = millis();
      }
    }     
   }
  else if(page==4){
  if (millis()-buttonTimer>100){  
  if(buttonHold ==1){
    buttonTest(10,&displayCounter,buttonHold);
    buttonTimer = millis();
    }
  else if(buttonHold ==2){
    buttonTest(12,&displayCounter,buttonHold);
    buttonTimer = millis();
    } 
  } 
 } 
        
}
  
}

void contrastTest (uint8_t val, char *curVal){
  if(val == 2){
      if(*curVal > 0){
      *curVal = *curVal - 5;
      lcd.setContrast(*curVal);
      sprintf(printBuffer, "%3d", (unsigned int)*curVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);}
      else{*curVal = 0;}
      
      }
    else if(val == 4){
      if(*curVal < 100){
      *curVal = *curVal + 5; 
      lcd.setContrast(*curVal);//set contrast value(contrast value: 0-100)
      sprintf(printBuffer, "%3d", (unsigned int)*curVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);
      }
      else{*curVal = 100;}
    }
    else if(val == 32){
      buttonHold=1;
      }
      else if(val == 64){
      buttonHold=2;
      }
      else if(val == 255){
      buttonHold=0;
      }
    else{
      }
  }

void rgbTest (uint8_t val,uint8_t *rVal,uint8_t *gVal,uint8_t *bVal){
    if(val == 2){
      if(rgbPtr == 0){
        if(*rVal >0){
        *rVal = *rVal - 5; 
        lcd.setCursor(2,2);
        sprintf(printBuffer, "%3d", (unsigned int)redVal);
        lcd.print(printBuffer); 
        lcd.setCursor(1,2);
        }
        else{*rVal = 255;
        lcd.setCursor(2,2);
        sprintf(printBuffer, "%3d", (unsigned int)redVal);
        lcd.print(printBuffer); 
        lcd.setCursor(1,2);
        }
        }
      else if(rgbPtr == 1){
        if(*gVal >0){
        *gVal = *gVal - 5; 
        
        lcd.setCursor(7,2);
        sprintf(printBuffer, "%3d", (unsigned int)greenVal);
        lcd.print(printBuffer);

        lcd.setCursor(6,2); 
        }
        else{*gVal = 255;
        lcd.setCursor(7,2);
        sprintf(printBuffer, "%3d", (unsigned int)greenVal);
        lcd.print(printBuffer);
        lcd.setCursor(6,2); }
        }  
       else if(rgbPtr == 2){
        if(*bVal >0){
        *bVal = *bVal - 5; 

        lcd.setCursor(12,2);
        sprintf(printBuffer, "%3d", (unsigned int)blueVal);
        lcd.print(printBuffer); 
        lcd.setCursor(11,2);
        }
        else{*bVal = 255;
        lcd.setCursor(12,2);
        sprintf(printBuffer, "%3d", (unsigned int)blueVal);
        lcd.print(printBuffer); 
        lcd.setCursor(11,2);
        }
        }   
      }
    else if(val == 4){
      if(rgbPtr == 0){
        if(*rVal <255){
        *rVal = *rVal + 5;
        lcd.setCursor(2,2);
        sprintf(printBuffer, "%3d", (unsigned int)redVal);
        lcd.print(printBuffer);
        
        lcd.setCursor(1,2);
        }
        else{*rVal = 0;
        lcd.setCursor(2,2);
        sprintf(printBuffer, "%3d", (unsigned int)redVal);
        lcd.print(printBuffer);
        lcd.setCursor(1,2);
        }
        }  

      else if(rgbPtr == 1){
        if(*gVal <255){
        *gVal = *gVal + 5;
        
        lcd.setCursor(7,2);
        sprintf(printBuffer, "%3d", (unsigned int)greenVal);
        lcd.print(printBuffer);
        lcd.setCursor(6,2);  
        }
        else{*gVal = 0;
        lcd.setCursor(7,2);
        sprintf(printBuffer, "%3d", (unsigned int)greenVal);
        lcd.print(printBuffer);
        lcd.setCursor(6,2);  
        }
        }
      else if(rgbPtr == 2){
        if(*bVal <255){
          *bVal = *bVal + 5; 
          
          lcd.setCursor(12,2);
          sprintf(printBuffer, "%3d", (unsigned int)blueVal);
          lcd.print(printBuffer); 
          lcd.setCursor(11,2);
          }
        else{*bVal = 0;
        lcd.setCursor(12,2);
          sprintf(printBuffer, "%3d", (unsigned int)blueVal);
          lcd.print(printBuffer); 
          lcd.setCursor(11,2);
        }
        }
      }
      else if(val == 8){
      rgbPtr++;
      if(rgbPtr==3){
        rgbPtr = 0;
        }
      else{}

      if(rgbPtr == 0){
        lcd.setCursor(1,1);
        lcd.print("Backlight: Red   ");
        lcd.setCursor(1,2);
        }  
      else if(rgbPtr == 1){
        lcd.setCursor(1,1);
        lcd.print("Backlight: Green ");
        lcd.setCursor(6,2);
        }    
      else if(rgbPtr == 2){
        lcd.setCursor(1,1);
        lcd.print("Backlight: Blue ");
        lcd.setCursor(11,2);
        }      
      }
      else if(val == 32){
      buttonHold=1;
      }
      else if(val == 64){
      buttonHold=2;
      }
      else if(val == 255){
      buttonHold=0;
      }
      lcd.setBacklightColor(*rVal,*gVal,*bVal);//set rgb backlight value(Red value(0-255), Green value(0-255), Blue value(0-255))
  }

void buttonTest(uint8_t val,int *counterVal, uint8_t buttonHoldInfo){
  if(val==2){//button 2 pressed
    *counterVal-=1;

    if(*counterVal<0){
      *counterVal=1000;
      }
    lcd.setCursor(9,1);
    lcd.print("2     ");
    lcd.setCursor(9,1);

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }
  else if(val==4){//button 3 pressed
    *counterVal+=1;

    if(*counterVal>1000){
      *counterVal=0;
      }
    lcd.setCursor(9,1);
    lcd.print("3     ");
    lcd.setCursor(9,1);

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }  

    else if(val==8){//button 4 pressed
    *counterVal=100;
    lcd.setCursor(9,1);
    lcd.print("4     ");
    lcd.setCursor(9,1);

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }

    else if(val==128){//button 4 hold
    *counterVal=0;
    lcd.setCursor(9,1);
    lcd.print("4 Hold");
    lcd.setCursor(9,1);

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }

    else if(val==6){//button 2&3 pressed
    *counterVal=1000;
    lcd.setCursor(9,1);
    lcd.print("2 & 3 ");
    lcd.setCursor(9,1);

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }
    
   else if(val==10){//button 2&3 pressed
    *counterVal-=10;

    if(*counterVal<0){
      *counterVal=1000;
      }

    lcd.setCursor(9,1);
    if(buttonHoldInfo ==0 ){
      lcd.print("2 & 4 ");
     }
    else{
      lcd.print("2 Hold");
    }

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    }
   else if(val==12){//button 2&4 pressed
    *counterVal+=10;

    if(*counterVal>1000){
      *counterVal=0;
      }
    lcd.setCursor(9,1);
    if(buttonHoldInfo ==0 ){
      lcd.print("3 & 4 ");
     }
    else{
      lcd.print("3 Hold");
    }
    

    lcd.setCursor(11,2);
    sprintf(printBuffer, "%04d",(unsigned int)*counterVal);
    lcd.print(printBuffer);
    } 
    
    else if(val== 32){//button 2 Hold
    buttonHold=1;
    }
    else if(val== 64){//button 3 Hold
    buttonHold=2;
    }
    else if(val == 255){//Button release (after hold)
    buttonHold=0;
    }
  }

void playSound(uint8_t val,uint8_t *toneVal){
  if(val == 2){
      if(*toneVal>1){
        *toneVal = *toneVal-1;
        sprintf(printBuffer, "%d", (unsigned int)*toneVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);}
      else{
        *toneVal=8;
        sprintf(printBuffer, "%d", (unsigned int)*toneVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);
        }
      }
    else if(val == 4){
      if(*toneVal<8){
        *toneVal = *toneVal+1;
        sprintf(printBuffer, "%d", (unsigned int)*toneVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);}
      else{
        *toneVal=1;
        sprintf(printBuffer, "%d", (unsigned int)*toneVal);
        lcd.setCursor(8,2);
        lcd.print(printBuffer);
        }
      }  
    else if(val == 8){
      lcd.playTone(*toneVal);//playing ringtone(1-8)
      }
    else{}  
    }
