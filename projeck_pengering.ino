//include library SD
#include <SPI.h>
#include <SD.h>

//include library DHT22
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//include library rtc ds3231
#include <DS3231.h>

//include library I2C
#include <Wire.h>

//include library LCD 20x4
#include <LiquidCrystal_I2C.h>

//pin SD
int sdPin = 8;
byte derajat = B11011111;

//init sensor DHT22
#define DHTTYPE DHT22
DHT dht1(3, DHT22);

//init SD module
File myFile;

//init sd
//SD sd;

//init sensor DS3231
DS3231 rtc;
RTCDateTime dt;

//init LCD 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

//boolean condition = false;
int counter = 1;
int lastTime = -1;

void setup() {

  //start serial using port 9600
  Serial.begin(9600);

  //start sensor rtc DS3231
  rtc.begin();

  //set start time rtc DS3231
  rtc.setDateTime(2022, 7, 2, 4, 59, 55);

  //start LCD 20x4
  lcd.init();
  lcd.backlight();

  pinMode (2, OUTPUT);
  digitalWrite (2, HIGH);
  //start sensor DHT1
  dht1.begin();

  //connecting to SD card
  lcd.setCursor(1, 2);
  lcd.print("Init SD card...");
  Serial.println("Init SD card...");
  if (!SD.begin(sdPin)) {
    lcd.clear();
    lcd.setCursor(1, 2);
    lcd.print("init failed!");
    Serial.println("init failed!");
  }

  delay(1000);
  lcd.println("init done.");
  Serial.println("init done.");
  digitalWrite(2, LOW);
  delay(500);
  lcd.clear();

  float suhu1 = dht1.readTemperature();
  dt = rtc.getDateTime();
  delay(200);
  lcd.setCursor(1, 0);
  lcd.print(dt.hour); lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.print(" ");
  lcd.setCursor(1, 1);
  lcd.print("Suhu: ");
  lcd.print(suhu1);
  lcd.print(" ");
  lcd.write(derajat);
  lcd.print("C");
}
void loop() {

  myFile = SD.open("myfile.txt", FILE_WRITE);
  if (myFile) {
    lcd.clear();
    lcd.setCursor(1, 2);
    lcd.print("SD ok");
    Serial.println("SD Ok");
    myFile.close();
  } else {
    lcd.clear();
    lcd.setCursor(1, 2);
    lcd.print("SD Bermasalah");
    Serial.println("SD bermasalah");
    SD.begin(sdPin);
  }

  float suhu1 = dht1.readTemperature();
  dt = rtc.getDateTime();

  if (suhu1 > 65) {
    digitalWrite(2, LOW);
    delay(500);
  } else {
    digitalWrite(2, HIGH);
    delay(500);
  }

  lcd.setCursor(1, 0);
  lcd.print(dt.hour); lcd.print(":");
  lcd.print(dt.minute); lcd.print(":");
  lcd.print(dt.second); lcd.print(" ");
  lcd.setCursor(1, 1);
  lcd.print("Suhu: ");
  lcd.print(suhu1);
  lcd.print(" ");
  lcd.write(derajat);
  lcd.print("C");

  Serial.print(dt.year); Serial.print("-");
  Serial.print(dt.month); Serial.print("-");
  Serial.print(dt.day); Serial.print(" ");
  Serial.print(dt.hour); Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.print(" ");
  Serial.print("---> ");
  Serial.println(suhu1);

  if (dt.hour - lastTime != 0 && !isnan(suhu1)) {
    Serial.println(dt.hour);
    Serial.println(lastTime);
    lastTime = dt.hour;
    Serial.println(dt.hour);
    Serial.println(lastTime);

    //opening SD card
    myFile = SD.open("myfile.txt", FILE_WRITE);
    if (myFile) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(dt.hour); lcd.print(":");
      lcd.print(dt.minute); lcd.print(":");
      lcd.print(dt.second); lcd.print(" ");
      lcd.setCursor(1, 1);
      lcd.print("Suhu: ");
      lcd.print(suhu1);
      lcd.print(" ");
      lcd.write(derajat);
      lcd.print("C");

      myFile.print(dt.year); myFile.print("-");
      myFile.print(dt.month); myFile.print("-");
      myFile.print(dt.day); myFile.print(" ");
      myFile.print(dt.hour); myFile.print(":");
      myFile.print(dt.minute); myFile.print(":");
      myFile.print(dt.second); myFile.print(" ");
      myFile.println(suhu1);

      // close the file:
      myFile.close();
    } else {
      Serial.println("error opening myfile.txt");
      lcd.clear();
      lcd.setCursor(1, 2);
      lcd.print("SD card bermasalah");
      SD.begin(sdPin);
    }
  }

  delay(1000);
}
