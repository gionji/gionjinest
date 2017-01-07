#include <Wire.h>
#include "rgb_lcd.h"

// Temperature sensor constants
const int B = 4275;                 // B value of the thermistor
const int R0 = 100000;            // R0 = 100k

#define POT_MODE  1
#define TIME_MODE 2
#define TEMP_MODE 3
#define TEMPTIME_MODE 4
#define IA_MODE 5

// state variables
int   heating_mode = POT_MODE;
float temperature  = 0;
int   light        = 0;
bool isHeatingOn = false;

//
int turnOnTemp = 10;

// pins
int BUTOON_PIN = A0;
int POT_PIN    = A1;
int TEMP_PIN   = A3;
int LIGHT_PIN  = A2;
int RELAY_PIN  = 2;
int BUZZER_PIN = 3;

rgb_lcd lcd;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY_PIN,  OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);

  // LCD init
  lcdInit();

  // Reset condition
  reset();

  delay(1000);

}

void loop() {

  // leggo eventuali comandi da seriale
  if (Serial.available() > 0) {
     int incomingByte = Serial.read();
    
     // say what you got:
     Serial.print("I received: ");
     Serial.println(incomingByte, DEC);
  }

  // leggo tutti i sensori o bottoni 
  temperature = readTemp();

  // aggiorno lo stato e le variabili
  if(heating_mode == POT_MODE)
    turnOnTemp  = readPot();
  
  decision();
  
  lcd.clear();
  
  lcd.setCursor(0,1);
  lcd.print( temperature ); 
  lcd.print("C - ");
  lcd.print( turnOnTemp );

  if(isHeatingOn){
    digitalWrite(RELAY_PIN, HIGH);
    }
  else{
    digitalWrite(RELAY_PIN, LOW);
  }
  
  delay(200);
}


void decision(){
  if(temperature < turnOnTemp)
    isHeatingOn = true;
  if(temperature > turnOnTemp + 1)
    isHeatingOn = false;
  }

void lcdInit(){
  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    delay(1000);
    lcd.setRGB(255, 0, 0);
    lcd.setCursor(0, 0);
    lcd.print("3");
    delay(1000);
    lcd.setRGB(255, 255, 0);
    lcd.setCursor(0, 0);
    lcd.print("2");
    delay(1000);
    lcd.setRGB(0, 255, 0);
    lcd.setCursor(0, 0);
    lcd.print("1");
    delay(1000);
    // Print a message to the LCD.

    buzzerOn();
    lcd.setCursor(0, 0);
    lcd.print("gionjinest");
    delay(1000);
    buzzerOff();
  }

void buzzerOn(){
  digitalWrite(BUZZER_PIN, HIGH);
  }

void buzzerOff(){
  digitalWrite(BUZZER_PIN, LOW);
  }

void reset(){  
  digitalWrite(RELAY_PIN,  LOW);
  digitalWrite(BUZZER_PIN, LOW);
  }

int readPot(){  
  int x = analogRead(POT_PIN);
  int y = map(x,0,1023,0,40);
  return y; 
  }  

float readTemp(){
  float R = 1023.0/((float)analogRead( TEMP_PIN ))-1.0;
  R = 100000.0*R;
  //convert to temperature via datasheet;
  float tt=1.00/(log(R/100000.0)/B+1/298.15)-273.15;
    
  return tt*0.66;
  }  
