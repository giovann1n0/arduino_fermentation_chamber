# include "DHT.h"
# include <LiquidCrystal.h>

// LCD information
LiquidCrystal lcd(34, 35, 30, 31, 32, 33);

// Temperature and Humidity sensor information
#define DHTPIN 25     // Defines the pin used to retrieve the data from the sensor
#define DHTTYPE DHT22   // Defines the sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialises  the sensor

// Fan information
const int coldpin =  26;

// Heating bed information
const int hotpin =  28;

// LED information
const int redled = 23;
const int greenled = 24;
const int blueled = 22;

// Temperature thresholds
float min_temp_c = 27.50;
float max_temp_c = 27.60;

int ms_delay = 2000;

void setup() {
  // Initialises the serial bus
  Serial.begin(9600);
  
  // Initialises the sensor
  dht.begin();

  delay(100);  // To give LCD time to power up
  lcd.begin(16,2);
  lcd.clear();

  // Initialises the fan
  pinMode(coldpin, OUTPUT);
  digitalWrite(coldpin, LOW);

  // Initialises the heating bed
  pinMode(hotpin, OUTPUT);
  digitalWrite(hotpin, LOW);

  // Initialises the 3 LEDs
  pinMode(redled, OUTPUT);
  digitalWrite(redled, LOW);
  pinMode(greenled, OUTPUT);
  digitalWrite(greenled, LOW);
  pinMode(blueled, OUTPUT);
  digitalWrite(blueled, LOW);

}

void loop() {
  // Temperature and Humidity sensor initialization
  delay(ms_delay);

  // Reads from the sensor
  float sampled_humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float sampled_temperature = dht.readTemperature();

  // Check if reading was successful
  if (isnan(sampled_temperature) || isnan(sampled_humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Triggers the temperature regulation
  if (sampled_temperature < min_temp_c) {
    digitalWrite(coldpin, LOW);
    digitalWrite(hotpin, HIGH);

    digitalWrite(redled, LOW);
    digitalWrite(greenled, LOW);
    digitalWrite(blueled, HIGH);
  }
  else if (sampled_temperature > max_temp_c) {
    digitalWrite(coldpin, HIGH);
    digitalWrite(hotpin, LOW);

    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
    digitalWrite(blueled, LOW);
  }
  else {
    digitalWrite(coldpin, LOW);
    digitalWrite(hotpin, LOW);

    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
    digitalWrite(blueled, LOW);
  }
  // Print statements
 Serial.println(sampled_temperature);
 lcd.setCursor(0, 0);
 lcd.print("Sampled Temp:");
 lcd.setCursor(0, 1);
 lcd.print(sampled_temperature, 1);
 lcd.print(" [*C]");
}