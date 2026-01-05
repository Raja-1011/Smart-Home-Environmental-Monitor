#include <Arduino_HS300x.h>  // Library for HS3003 temp/humidity sensor

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial Monitor (Nano 33 BLE behavior)

  Serial.println("HS3003 Temperature & Humidity Test");

  // Initialize the HS3003 sensor
  if (!HS300x.begin()) {
    Serial.println("Failed to initialize HS3003 sensor!");
    while (1) {
      // Stay here if sensor is not found
      delay(1000);
    }
  }

  Serial.println("HS3003 sensor initialized successfully.");
}

void loop() {
  // Read temperature in Celsius
  float temperature = HS300x.readTemperature();

  // Read relative humidity in %
  float humidity = HS300x.readHumidity();

  // Print results
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.print(" °C");

  Serial.print("   |   Humidity: ");
  Serial.print(humidity, 2);
  Serial.println(" %RH");

  delay(1000);  // 1 reading per second
}
