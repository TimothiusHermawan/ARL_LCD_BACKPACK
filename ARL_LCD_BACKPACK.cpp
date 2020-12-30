/*
  ARL_LCD_BACKPACK.cpp - Library for driving ARL LCD Backpack.
  Created by Timothius H, November 27, 2020.
  Released into the public domain.
*/

#include "ARL_LCD_BACKPACK.h"


ARL_LCD_BACKPACK::ARL_LCD_BACKPACK(HardwareSerial &com, uint8_t interruptPin){
  HWUART = &com;
  communication = 2;
  _interruptPin = interruptPin;
  attachInterrupt(digitalPinToInterrupt(_interruptPin), comInt2, FALLING);
}
ARL_LCD_BACKPACK::ARL_LCD_BACKPACK(SoftwareSerial &com){
  UART = &com;
  communication = 1;
  attachInterrupt(digitalPinToInterrupt(_interruptPin), comInt2, FALLING);
}

ARL_LCD_BACKPACK::ARL_LCD_BACKPACK(SoftwareSerial &com, uint8_t interruptPin){
  UART = &com;
  communication = 1;
  _interruptPin = interruptPin;
  attachInterrupt(digitalPinToInterrupt(_interruptPin), comInt2, FALLING);
}

ARL_LCD_BACKPACK::ARL_LCD_BACKPACK(uint8_t address,uint8_t interruptPin){
    communication = 0;
    ADDRESS = address;
    _interruptPin = interruptPin;
  attachInterrupt(digitalPinToInterrupt(_interruptPin), comInt2, FALLING);
}

void ARL_LCD_BACKPACK::begin(unsigned long speed){
  if(communication ==1){
    _UARTBAUD =speed ;
    UART->begin(_UARTBAUD);
  }
  else if(communication ==2){
    _UARTBAUD =speed ;
    HWUART->begin(_UARTBAUD);
  }
  else{
    _speed = speed;
    Wire.setClock(_speed);
    Wire.begin();
  }
  while(!digitalRead(_interruptPin)){}
}

void ARL_LCD_BACKPACK::begin(){
  Wire.begin();
  while(!digitalRead(_interruptPin)){}
}


uint8_t ARL_LCD_BACKPACK::interrupt(void){
  if(notif==1){
   notif=0;
    return 1;
  }
  else{
    return 0;
  }
}

void comInt2(void){
notif=1;
}

void ARL_LCD_BACKPACK::write(char dat){
  if(communication ==1){
    UART->write(dat);
  }
  else if(communication ==2){
    HWUART->write(dat);
  }
}

void ARL_LCD_BACKPACK::write(char memLoc, char dat){
  Wire.beginTransmission(ADDRESS);
  Wire.write(memLoc);
  Wire.write(dat);
  Wire.endTransmission();
}

char ARL_LCD_BACKPACK::read(){
  if(communication ==1){
    return UART->read();
  }
  else if(communication ==2){
    return HWUART->read();
  }
}

char ARL_LCD_BACKPACK::read(uint8_t memLoc){
  uint8_t buffer =0;

  Wire.beginTransmission(ADDRESS);
  Wire.write(memLoc);
  Wire.endTransmission();
  buffer = Wire.requestFrom(ADDRESS, (uint8_t)1);
  while (!Wire.available()); 
   return Wire.read(); 
  
}


char ARL_LCD_BACKPACK::available(){
  if(communication ==1){
    return UART->available();
  }
  else if(communication ==2){
    return HWUART->available();
  }
}

void ARL_LCD_BACKPACK::command (unsigned char cmd)
{
  if(communication ==1){  
    UART->write(0x5A);  
    UART->write(0xFE);
    UART->write(0x01); 
    UART->write(cmd);
    UART->write(0xF0);
    UART->write(0x0F);
  }
  else if(communication ==2){  
    HWUART->write(0x5A);  
    HWUART->write(0xFE);
    HWUART->write(0x01); 
    HWUART->write(cmd);
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }
  else if(communication == 0){
   write(0x01, cmd); 
  }
}

void ARL_LCD_BACKPACK::setBacklightColor(char red, char green, char blue){
  if(communication ==1){
    UART->write(0x5A);    
    UART->write(0xFE);
    UART->write(0x02); 
    UART->write(red);
    UART->write(green);
    UART->write(blue); 
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){
    HWUART->write(0x5A);    
    HWUART->write(0xFE);
    HWUART->write(0x02); 
    HWUART->write(red);
    HWUART->write(green);
    HWUART->write(blue); 
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else if(communication == 0){
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x02);            
    Wire.write(red);         
    Wire.write(green);
    Wire.write(blue);
    Wire.endTransmission();
  }

  _redValue=red;
  _greenValue=green;
  _blueValue=blue;
}

void ARL_LCD_BACKPACK::setContrast(char val){
  if(communication ==1){
    UART->write(0x5A);    
    UART->write(0xFE);
    UART->write(0x03); 
    UART->write(val);  
    UART->write(0xF0);
    UART->write(0x0F);
  }
  if(communication ==2){
    HWUART->write(0x5A);    
    HWUART->write(0xFE);
    HWUART->write(0x03); 
    HWUART->write(val);  
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }
  else if(communication == 0){
    write(0x05,val);
  }
}

void ARL_LCD_BACKPACK::backlightOff(){
 if(communication ==1){
    UART->write(0x5A);    
    UART->write(0xFE);
    UART->write(0x02); 
    UART->write((uint8_t)0);
    UART->write((uint8_t)0);
    UART->write((uint8_t)0); 
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){
    HWUART->write(0x5A);    
    HWUART->write(0xFE);
    HWUART->write(0x02); 
    HWUART->write((uint8_t)0);
    HWUART->write((uint8_t)0);
    HWUART->write((uint8_t)0); 
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else if(communication == 0){
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x02);            
    Wire.write((uint8_t)0);         
    Wire.write((uint8_t)0);
    Wire.write((uint8_t)0);
    Wire.endTransmission();
  }
}

void ARL_LCD_BACKPACK::backlightOn(){
 if(communication ==1){
    UART->write(0x5A);    
    UART->write(0xFE);
    UART->write(0x02); 
    UART->write(_redValue);
    UART->write(_greenValue);
    UART->write(_blueValue); 
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){
    HWUART->write(0x5A);    
    HWUART->write(0xFE);
    HWUART->write(0x02); 
    HWUART->write(_redValue);
    HWUART->write(_greenValue);
    HWUART->write(_blueValue); 
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else if(communication == 0){
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x02);            
    Wire.write(_redValue);         
    Wire.write(_greenValue);
    Wire.write(_blueValue);
    Wire.endTransmission();
  }
}

void ARL_LCD_BACKPACK::printChar (char char_data)
{
  if(communication ==1){  
  UART->write(0x5A);  
  UART->write(char_data);
  UART->write(0xF0);
  UART->write(0x0F);
  }

  else if(communication ==2){  
  HWUART->write(0x5A);  
  HWUART->write(char_data);
  HWUART->write(0xF0);
  HWUART->write(0x0F);
  }
  
  else if(communication ==0){  
  write(20,char_data);
  }
}
  
void ARL_LCD_BACKPACK::print (char *str)  // Send string to LCD function
{
  if(communication ==1){ 
    UART->write(0x5A); 
    UART->print(str);
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){ 
    HWUART->write(0x5A); 
    HWUART->print(str);
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else{
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x15);
    while (*str){
      Wire.write(*str++);
    }
    Wire.endTransmission();
  }
}

void ARL_LCD_BACKPACK::createChar (uint8_t location, byte *icon){
  uint8_t counter=8;
  location &=0x07;
  command(0x40+(location<<3));
  if(communication ==1){ 
    UART->write(0x5A); 
    while (counter>0){
      UART->write(*icon++);
      counter--;
    }
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){ 
    HWUART->write(0x5A); 
    while (counter>0){
      HWUART->write(*icon++);
      counter--;
    }
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else{
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x15);
    while (counter>0){
      Wire.write(*icon++);
      counter--;
    }
    Wire.endTransmission();
  }
}

void ARL_LCD_BACKPACK::adcRead(char pin, char spd, char format){
  if(communication ==1){  
    UART->write(0x5A);  
    UART->write(0xFE);
    UART->write(0x04); 
    UART->write(pin);
    UART->write(spd);
    UART->write(format); 
    UART->write(0xF0);
    UART->write(0x0F);
  }  

  else if(communication ==2){  
    HWUART->write(0x5A);  
    HWUART->write(0xFE);
    HWUART->write(0x04); 
    HWUART->write(pin);
    HWUART->write(spd);
    HWUART->write(format); 
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  } 

  else if(communication == 0){
    char cfg;

    cfg = pin<<5;
    cfg &= 0x20;
  
    cfg |= (format<<4);
    cfg &= 0x30;
  
    cfg |= (spd);
    cfg &= 0x3F; 
    write(0x06,cfg);
  }
}
void ARL_LCD_BACKPACK::playTone(unsigned char tone){
  if(communication ==1){ 
    UART->write(0x5A);   
    UART->write(0xFE);
    UART->write(0x06);
    UART->write(tone);
    UART->write(0xF0);
    UART->write(0x0F);
  }

  else if(communication ==2){ 
    HWUART->write(0x5A);   
    HWUART->write(0xFE);
    HWUART->write(0x06);
    HWUART->write(tone);
    HWUART->write(0xF0);
    HWUART->write(0x0F);
  }

  else if(communication ==0){
    write(0x0B,tone);
  }

}

void ARL_LCD_BACKPACK::shiftDisplayLeft(){
command(0x18);
}

void ARL_LCD_BACKPACK::shiftDisplayRight(){
command(0x1C);
}

void ARL_LCD_BACKPACK::moveCursorLeft(){
command(0x10);
}

void ARL_LCD_BACKPACK::moveCursorRight(){
command(0x14);
}

void ARL_LCD_BACKPACK::displayOff(){
displayConfig &= ~0x04;  
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::displayOn(){
displayConfig |= 0x04; 
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::cursorOff(){
displayConfig &= ~0x02; 
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::cursorOn(){
displayConfig |= 0x02; 
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::blinkOff(){
displayConfig &= ~0x01; 
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::blinkOn(){
displayConfig |= 0x01; 
command(_displaySetting|displayConfig);
}

void ARL_LCD_BACKPACK::writingLeftToRight(){
movementConfig |= 0x02; 
command(_entryMode|movementConfig);
}

void ARL_LCD_BACKPACK::writingRightToLeft(){
movementConfig &= ~0x02; 
command(_entryMode|movementConfig);
}

void ARL_LCD_BACKPACK::autoCursorShift(){
movementConfig &= ~0x01; 
command(_entryMode|movementConfig);
}

void ARL_LCD_BACKPACK::noCursorShift(){
movementConfig |= 0x01; 
command(_entryMode|movementConfig);
}

void ARL_LCD_BACKPACK::init ()
{ 
  command (0x38); 
  command (0x0E);
  command (0x06);
  clear();
  command (0x80);
}

void ARL_LCD_BACKPACK::init1Row ()
{ 
  command (0x30); 
  command (0x0E);
  command (0x06);
  clear();
  command (0x80);
}

void ARL_LCD_BACKPACK::clear (){
command(0x01);
delay(2);
}


void ARL_LCD_BACKPACK::setCursor (char pos, char row)      //Setting Cursor Position
  { row = row-1;
    pos = pos-1;
    
  if(pos<16){
  if (row == 0){
  command((pos & 0x0F)|0x80);}
  else if (row == 1)
  command((pos & 0x0F)|0xC0);
  else if (row == 2){
  pos = pos+4;
  command((pos & 0x0F)|0x90);}
  else if (row == 3){
  pos = pos+4;  
  command((pos & 0x0F)|0xD0);}
  }
  else{
  pos = pos-16;
  if (row == 0){
  command((pos & 0x0F)|0x90);}
  else if (row == 1)
  command((pos & 0x0F)|0xD0);
  else if (row == 2){
  pos = pos+4;
  command((pos & 0x0F)|0xA0);}
  else if (row == 3){
  pos = pos+4;  
  command((pos & 0x0F)|0xE0);}
  }
delay(2);  
  
}

void ARL_LCD_BACKPACK::returnHome (){
command(0x02);
}