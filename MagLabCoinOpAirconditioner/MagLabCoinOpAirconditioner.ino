// Fubarino SD, Arduino IDE 1.6.7, chipKIT core 1.1.0

// Coin Counter (CH-924 from Adafruit) Wiring:
// Red: +12VDC
// Black: Ground
// White: No Connect
// Gray: 10k pullup to 3.3VDC + Pin 0 of Fubarino SD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define COIN_COUNTER 0 //PIN_INT1

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

volatile uint32_t pennies = 0;
uint32_t pennies_current = 0;
uint32_t pennies_last = 0;

int32_t time_s_per_cent = 2;
int32_t time_remaining  = 0;

// On Peek $0.20/kWh 12pm - 6pm
// Mid Peek $0.16/kWh 8am - 12pm, 6pm - 11pm
// Off Peek $0.14/kWh 11pm - 8am

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  //while(!Serial);
  delay(3000);
  Serial.println("Mag Lab Coin-Op Air Conditioner...");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  pinMode(PIN_LED1, OUTPUT);

  pinMode(COIN_COUNTER, INPUT);
  attachInterrupt(COIN_COUNTER, intHandle, LOW);
}

void delay_counter(uint32_t ms)
{
  uint32_t start_ms = millis();
  uint32_t end_ms = start_ms + ms;
  /*
    Serial.print(start_ms ,DEC);
    Serial.print(" ");
    Serial.print(end_ms,DEC);
    Serial.print(" ");
  */
  while ( millis() < end_ms )
  {
    if ( digitalRead(COIN_COUNTER) == LOW )
    {
      pennies++;
      while (digitalRead(COIN_COUNTER) == LOW);
    }
  }
}

// the loop function runs over and over again forever
void loop()
{

  // This must be a single cycle (or we have to disable intnerrupts
  pennies_current = pennies;

  if ( pennies_current != pennies_last )
  {
    time_remaining += (pennies_current - pennies_last) * time_s_per_cent;
    pennies_last = pennies_current;
  }

  delay_counter(1000);


  //delay(1000);

  if ( time_remaining > 0)
  {
    uint32_t cents = pennies_current % 100;
    uint32_t dollars = pennies_current / 100;
    Serial.print("$");
    Serial.print(dollars, DEC);
    Serial.print(".");
    if ( cents <= 9 ) Serial.print("0");
    Serial.print(cents, DEC);
    Serial.print(" ");
    Serial.print(time_remaining, DEC);
    Serial.print(" sec");

    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Funds    ");
    lcd.print("$");
    lcd.print(dollars, DEC);
    lcd.print(".");
    if ( cents <= 9 ) lcd.print("0");
    lcd.print(cents, DEC);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print("Time    ");
    lcd.print(time_remaining, DEC);
    lcd.print(" sec");
    Serial.println("");
    digitalWrite(PIN_LED1, HIGH);
    time_remaining--;
  }
    else if ( time_remaining <= 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mag Lab Coin-Op");
      lcd.setCursor(0, 2);
      lcd.print("Air Conditioner");
      digitalWrite(PIN_LED1, LOW);
      Serial.println(pennies);
    }
}
  void intHandle() {
    pennies++;
  }


