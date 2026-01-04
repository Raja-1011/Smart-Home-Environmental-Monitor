/*
  Nano 33 BLE Sense Rev2 - Sensor Node Firmware
  --------------------------------------------
  Purpose:
  - Read environmental sensors:
      * LDR (ambient brightness) via ADC
      * MQ-2 gas sensor via ADC
      * HS300x temperature and humidity via I2C
  - Display  output on USB Serial Monitor
  - Transmit structured CSV telemetry over UART (Serial1)
    to the MKR1010 receiver node

  CSV Format Sent Over UART:
    timestamp_ms, light_percent, gas_percent, temperature_C, humidity_percent
*/

#include <Arduino_HS300x.h>

// ---------------- Pin definitions ----------------
const int LDR_PIN = A0;   // LDR connected to analog input A0
const int GAS_PIN = A1;   // MQ-2 gas sensor connected to analog input A1

void setup() {
  // USB serial communication to laptop (debugging & evidence)
  Serial.begin(115200);

  // UART communication to MKR1010 receiver node
  Serial1.begin(9600);

  // Initialise HS300x temperature & humidity sensor (I2C)
  HS300x.begin();

  Serial.println("Nano Sensor Node started");
}

void loop() {
  // ----------- Read analogue sensors (ADC) -----------

  // Read raw ADC value from LDR
  int ldrRaw = analogRead(LDR_PIN);
  // Convert ADC value to voltage using 3.3 V reference
  float ldrV = (ldrRaw * 3.3f) / 1023.0f;

  // Read raw ADC value from gas sensor
  int gasRaw = analogRead(GAS_PIN);
  // Convert ADC value to voltage
  float gasV = (gasRaw * 3.3f) / 1023.0f;

  // Convert ADC readings into percentage values
  float lightPct = (ldrRaw / 1023.0f) * 100.0f;
  float gasPct   = (gasRaw / 1023.0f) * 100.0f;

  // ----------- Read digital sensors (I2C) -----------

  // Read temperature and humidity from HS300x sensor
  float tempC = HS300x.readTemperature();
  float humRH = HS300x.readHumidity();

  // Generate timestamp in milliseconds
  unsigned long ts = millis();

  // -----------  output (USB Serial) -----------

  Serial.println("---- SENSOR READINGS ----");

  Serial.print("LDR   raw: ");
  Serial.print(ldrRaw);
  Serial.print(" | V: ");
  Serial.print(ldrV, 3);
  Serial.print(" V | Brightness: ");
  Serial.print(lightPct, 1);
  Serial.println(" %");

  Serial.print("GAS  raw: ");
  Serial.print(gasRaw);
  Serial.print(" | V: ");
  Serial.print(gasV, 3);
  Serial.print(" V | Gas Level: ");
  Serial.print(gasPct, 1);
  Serial.println(" %");

  Serial.print("TEMP: ");
  Serial.print(tempC, 2);
  Serial.print(" °C | HUM: ");
  Serial.print(humRH, 2);
  Serial.println(" %RH");
  Serial.println();

  // ----------- UART CSV transmission to MKR1010 -----------

  // CSV format: timestamp,lightPct,gasPct,tempC,humRH
  Serial1.print(ts);
  Serial1.print(",");
  Serial1.print(lightPct, 2);
  Serial1.print(",");
  Serial1.print(gasPct, 2);
  Serial1.print(",");
  Serial1.print(tempC, 2);
  Serial1.print(",");
  Serial1.println(humRH, 2);

  // ----------- Sampling interval -----------

  // Delay used here to maintain ~500 ms sampling rate
  delay(500);
}
