#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD I2C address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); // (address, columns, rows)

// Pin assignments
#define ONE_WIRE_BUS 8
#define RELAY_PIN 9

// Temperature limits in Fahrenheit
#define TEMP_OFF 30.0  // Turn OFF at 30°F
#define TEMP_ON 36.0   // Turn ON at 36°F

bool switchOn = false;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);           // Optional: Serial Monitor
  sensors.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start relay OFF

  lcd.init();        // Initialize LCD
  lcd.backlight();   // Turn on backlight

  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(1000);
}

void loop() {
  sensors.requestTemperatures();
  float tempF = sensors.getTempFByIndex(0);

  Serial.print("Temperature: ");
  Serial.println(tempF);

  // Relay control with hysteresis
  if (!switchOn && tempF >= TEMP_ON) {
    digitalWrite(RELAY_PIN, HIGH); // Turn relay ON
    switchOn = true;
    Serial.println("Relay: ON");
  } else if (switchOn && tempF <= TEMP_OFF) {
    digitalWrite(RELAY_PIN, LOW); // Turn relay OFF
    switchOn = false;
    Serial.println("Relay: OFF");
  }

  // Update LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempF, 1);
  lcd.print((char)223); // Degree symbol
  lcd.print("F");

  lcd.setCursor(0, 1);
  lcd.print("Switch: ");
  lcd.print(switchOn ? "ON " : "OFF");

  delay(1000); // Update every second
}
