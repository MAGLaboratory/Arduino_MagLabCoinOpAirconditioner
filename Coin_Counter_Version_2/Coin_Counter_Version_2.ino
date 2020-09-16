#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
volatile int INTcount;
int pin = 2;
int result;
int timer;
unsigned long time1;
unsigned long minCounter;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
void setup() 
{ 
Serial.begin(9600);
 lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight  ();
   
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("HI!YourDuino.com");
  delay(1000);  

// Wait and then tell user they can start the Serial Monitor and type in characters to
// Display. (Set Serial Monitor option to "No Line Ending")
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time Left: ");
  lcd.print(timer);
  lcd.print("min");
  lcd.setCursor(0,1);
  lcd.print("1 cent per min");


attachInterrupt(digitalPinToInterrupt(pin), timerControl, FALLING);
pinMode(13, OUTPUT);
}

void timerControl()
{
  INTcount = INTcount + 1;
  timer = timer + 1;
  
}
void loop() {
  lcd.setCursor(0,0);
  lcd.print("Time Left: ");
  lcd.print(timer);
  lcd.print("min");
  lcd.setCursor(0,1);
  lcd.print("1 cent per min");
  time1 = millis();
  Serial.println(minCounter);
  Serial.println(time1);
  if(timer == 1)
  {
    minCounter = millis() + 60000;
  }
  
  if(timer != 0 && minCounter <= millis())
  {
    timer = timer - 1;
    minCounter = millis() + 600000;
  }
 /*while(timer > 0)
 {
   digitalWrite(13,HIGH);
   delay(60000);
   timer = timer - 1;
 }

 if(timer < 0)
 {
  digitalWrite(13,LOW);
  delay(60000);
 }*/
}


