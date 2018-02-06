#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Servo.h>
#include "Wire.h"
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define DS1307_I2C_ADDRESS 0x68 // the I2C address of Tiny RTC
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
// Convert normal decimal numbers to binary coded decimal
Servo myservo;
int mode = 0;
int mode_s = 0;
int mode_s2 = 0;
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}
// Function to set the currnt time, change the second&minute&hour to the right time
void setDateDs1307(int hour, int minute, int second )
{

  dayOfWeek = 0;
  dayOfMonth = 0;
  month = 0;
  year = 0;
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(decToBcd(0));
  Wire.write(decToBcd(second)); // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour)); // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
// Function to gets the date and time from the ds1307 and prints result
void getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(decToBcd(0));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  second = bcdToDec(Wire.read() & 0x7f);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3f); // Need to change this if 12 hour am/pm
  dayOfWeek = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.println();
  Serial.print("MODE");
  Serial.print("mode ");
  Serial.print(mode);
  Serial.print(" mode_s ");
  Serial.print(mode_s);
  Serial.print(" mode_s2 ");
  Serial.print(mode_s2);
  Serial.println();
  //Serial.print("Day of week:");
}
void setup() {
  lcd.begin(16, 2);
  myservo.attach(11);
  myservo.write(90);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  //setDateDs1307(16,0,0); //Set current time;
  //getDateDs1307();//get the time data from tiny RTC
  //EEPROM.read(address);
  //EEPROM.write(addr, val);
}
unsigned long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  if (mode == 0) {


    if (currentMillis - previousMillis >= 1000) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      getDateDs1307();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Now ");
      lcd.print(hour, DEC);
      lcd.print(":");
      lcd.print(minute, DEC);
      lcd.print(":");
      lcd.print(second, DEC);
      lcd.setCursor(0, 1);
      lcd.print("Set ");
      lcd.print(EEPROM.read(5));
      lcd.print(",");
      lcd.print(EEPROM.read(6));
      lcd.setCursor(9, 1);
      lcd.print(" Lev ");
      lcd.print(EEPROM.read(1));
      if (EEPROM.read(5) == hour) {
        if(mode_s == 0){
        mode_s = 1;
        myservo.write(45);
        digitalWrite(LED_BUILTIN, HIGH);
        if (EEPROM.read(1) == 1)
          delay(1000*EEPROM.read(2));
        if (EEPROM.read(1) == 2)
          delay(1000*EEPROM.read(3));
        if (EEPROM.read(1) == 3)
          delay(1000*EEPROM.read(4));
        }
        myservo.write(90);
        digitalWrite(LED_BUILTIN, LOW);
      }else{
        mode_s = 0;
      }
      if (EEPROM.read(6) == hour) {
        if(mode_s2 == 0){
        mode_s2 = 1;
        myservo.write(45);
        digitalWrite(LED_BUILTIN, HIGH);
        if (EEPROM.read(1) == 1)
          delay(1000*EEPROM.read(2));
        if (EEPROM.read(1) == 2)
          delay(1000*EEPROM.read(3));
        if (EEPROM.read(1) == 3)
          delay(1000*EEPROM.read(4));
        }
        myservo.write(90);
        digitalWrite(LED_BUILTIN, LOW);
      }else{
        mode_s2 = 0;
      }
      
    }
    if (digitalRead(8) == 0) {
      mode = 1;
    }
    if (digitalRead(9) == 0) {
      mode = 2;
    }
    if (digitalRead(10) == 0) {
      mode = 3;
    }

  }
  if (mode == 1) {  ///set เวลานาฬิกา
    if (currentMillis - previousMillis >= 500) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      getDateDs1307();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Timenow ");
      lcd.setCursor(0, 1);
      lcd.print("H ");
      lcd.print(EEPROM.read(7));
      lcd.print(" M ");
      lcd.print(EEPROM.read(8));
      lcd.print(" S ");
      lcd.print(EEPROM.read(9));
    }
    if (digitalRead(8) == 0) {
      delay(200);
      int Level = EEPROM.read(7) + 1;
      if (Level > 23)
        Level = 0;
      EEPROM.write(7, Level);
    }
    if (digitalRead(9) == 0) {
      delay(200);
      int Level = EEPROM.read(8) + 1;
      if (Level > 59)
        Level = 0;
      EEPROM.write(8, Level);
    }
    if (digitalRead(10) == 0) {
      delay(200);
      int Level = EEPROM.read(9) + 1;
      if (Level > 59)
        Level = 0;
      EEPROM.write(9, Level);
    }
    if (digitalRead(12) == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OK");
      setDateDs1307(EEPROM.read(7), EEPROM.read(8), EEPROM.read(9));
      mode = 0;
      delay(1000);
    }
  }
  if (mode == 2) {  ///set เวลาให้อาหาร
    if (currentMillis - previousMillis >= 1000) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      getDateDs1307();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Times ");
      lcd.setCursor(0, 1);
      lcd.print("Set ");
      lcd.print(EEPROM.read(5));
      lcd.print(",");
      lcd.print(EEPROM.read(6));
    }
    if (digitalRead(8) == 0) {
      delay(200);
      int Level = EEPROM.read(5) + 1;
      if (Level > 23)
        Level = 0;
      EEPROM.write(5, Level);
    }
    if (digitalRead(9) == 0) {
      delay(200);
      int Level = EEPROM.read(6) + 1;
      if (Level > 23)
        Level = 0;
      EEPROM.write(6, Level);
    }
    if (digitalRead(12) == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OK");
      mode = 0;
      delay(1000);
    }
  }
  if (mode == 3) {    ///set ปริมานให้อาหาร
    if (currentMillis - previousMillis >= 1000) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      getDateDs1307();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Level ");
      lcd.setCursor(0, 1);
      lcd.print("Level ");
      lcd.print(EEPROM.read(1));
      lcd.print(" ");
      if (EEPROM.read(1) == 1)
        lcd.print(EEPROM.read(2));
      if (EEPROM.read(1) == 2)
        lcd.print(EEPROM.read(3));
      if (EEPROM.read(1) == 3)
        lcd.print(EEPROM.read(4));
    }
    if (digitalRead(8) == 0) {
      delay(200);
      int Level = EEPROM.read(1) + 1;
      if (Level > 3)
        Level = 1;
      EEPROM.write(1, Level);
    }
    if (digitalRead(9) == 0 && EEPROM.read(1) == 1) {
      delay(200);
      int Level = EEPROM.read(2) + 1;
      if (Level > 20)
        Level = 1;
      EEPROM.write(2, Level);
    }
    if (digitalRead(9) == 0 && EEPROM.read(1) == 2) {
      delay(200);
      int Level = EEPROM.read(3) + 1;
      if (Level > 20)
        Level = 1;
      EEPROM.write(3, Level);
    }
    if (digitalRead(9) == 0 && EEPROM.read(1) == 3) {
      delay(200);
      int Level = EEPROM.read(4) + 1;
      if (Level > 20)
        Level = 1;
      EEPROM.write(4, Level);
    }
    if (digitalRead(12) == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OK");
      mode = 0;
      delay(1000);
    }

  }
}
