/*
  ARL_LCD_BACKPACK.cpp - Library for driving ARL LCD Backpack.
  Created by Timothius H, November 27, 2020.
  Released into the public domain.
*/

#ifndef ARL_LCD_BACKPACK_H
#define ARL_LCD_BACKPACK_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

volatile static uint8_t notif;

//static void intAvail0();
void comInt2(void);

class ARL_LCD_BACKPACK {
  private:
    char communication;
    unsigned long _UARTBAUD;   
    SoftwareSerial* UART;
    HardwareSerial* HWUART;
    uint8_t ADDRESS;
    uint8_t _interruptPin;
    uint8_t _serialPin;
    unsigned long _speed;
    uint8_t _entryMode = 0x04;
    uint8_t movementConfig = 0x02;
    uint8_t displayConfig = 0x06;
    uint8_t _displaySetting = 0x08;
    uint8_t _redValue = 0xFF;
    uint8_t _greenValue = 0xFF;
    uint8_t _blueValue = 0xFF;
    uint8_t x = 1;
    uint8_t y = 1;
  public:
    ARL_LCD_BACKPACK(HardwareSerial &com,uint8_t interruptPin);
    ARL_LCD_BACKPACK(SoftwareSerial &com);
    ARL_LCD_BACKPACK(SoftwareSerial &com,uint8_t interruptPin);
    ARL_LCD_BACKPACK(uint8_t,uint8_t);
    void begin();
    void begin(unsigned long UARTBAUD);
    uint8_t interrupt(void);
    void write(char dat);
    void write(char memloc, char dat);
    char read();
    char read(uint8_t memLoc);
    char available();
    void init(void);
    void init1Row(void);
    void setCursor (char row, char pos);
    void clear(void);
    void command (unsigned char cmd);
    void printChar (char char_data);
    void print (char *str);
    void shiftDisplayLeft(void);
    void shiftDisplayRight(void);
    void moveCursorLeft(void);
    void moveCursorRight(void);
    void displayOff(void);
    void displayOn(void);
    void cursorOn(void);
    void cursorOff(void);
    void blinkOn(void);
    void blinkOff(void);
    void backlightOn(void);
    void backlightOff(void);
    void writingLeftToRight(void);
    void writingRightToLeft(void);
    void autoCursorShift(void);
    void noCursorShift(void);
    void returnHome(void);
    void createChar(uint8_t location, byte *icon);
    void setBacklightColor(char,char,char);
    void setContrast(char);
    void adcRead(char, char, char);
    void playTone(unsigned char);
};

#endif