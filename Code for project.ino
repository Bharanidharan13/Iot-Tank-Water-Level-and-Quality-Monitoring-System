#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PH_PIN A0 

const float offset_voltage = 2.5;  
const float voltage_to_pH_slope = -5.0; 

float voltage, pH_value;
#define TRIG_PIN 9
#define ECHO_PIN 10
#define PUMP_PIN 8

const int MIN_DISTANCE = 2;
const int MAX_DISTANCE = 10;



void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Batch 4 Project");
  delay(2000);
  lcd.clear();  
  pinMode(PH_PIN, INPUT); 
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  int maxLevel = 12;
  int waterHeight = max(0, maxLevel - distance);  
  float waterLevelPercentage = (waterHeight / (float)maxLevel) * 100;

  Serial.print("Water level: ");
  Serial.print(waterLevelPercentage);
  Serial.print(" %");

  lcd.setCursor(0, 0);
  lcd.print("WaterLevel:");
  lcd.print(waterLevelPercentage, 1);
  lcd.print("%   ");
  if (distance > MAX_DISTANCE)
  {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Pump ON");
    lcd.setCursor(0, 1);
    lcd.print("Pump Status:ON  ");
  }
  else if (distance <= MIN_DISTANCE)
  {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Pump OFF");
    lcd.setCursor(0, 1);
    lcd.print("Pump Status:OFF ");
  }

  delay(3000);


  const int numReadings = 10;
    float total = 0.0;
    for (int i = 0; i < numReadings; i++) {
        total += analogRead(PH_PIN);
        delay(10); // Short delay between readings
    }

    float averageReading = total / numReadings;

    voltage = (averageReading * 5.0) / 1023.0;

    pH_value = (voltage - offset_voltage) * voltage_to_pH_slope + 7.0;

    Serial.print("pH Value: ");
    Serial.println(pH_value, 2); 

    // Display pH value on LCD
    lcd.setCursor(0, 0);
    if (pH_value >= 6.5 && pH_value <= 8.5) {
        Serial.println("Water is safe");
        lcd.print("Water Safe     ");
    }
    else
    {
        Serial.println("Water Unsafe");
        lcd.print("Water Unsafe     ");
    }

    delay(2000); 
}
