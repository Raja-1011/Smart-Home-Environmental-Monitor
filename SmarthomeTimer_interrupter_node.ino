/*
  Nano 33 BLE Sense Rev2 - Interrupt-Driven Sensor Node
  ----------------------------------------------------
  Purpose:
  - Demonstrate interrupt based sensor sampling 
  - Acquire analogue sensor data:
      * LDR (ambient light) via ADC
      * MQ-2 gas sensor via ADC
  - Acquire digital temperature and humidity data from HS300x via I2C
  - Output sensor values to USB Serial for evidence
  - Transmit structured CSV telemetry over UART (Serial1) to MKR receiver

*/

#include <Arduino_HS300x.h>
#include <mbed.h>   // Required for mbed::Ticker on Nano 33 BLE Sense Rev2

// ---------------- Pin definitions ----------------
// No changes to hardware wiring
const int LDR_PIN = A0;   // Light-dependent resistor (ADC input)
const int GAS_PIN = A1;   // MQ-2 gas sensor (ADC input)

// ---------------- Sampling configuration ----------------
// Fixed sampling period (500 ms)
const float SAMPLE_PERIOD_S = 0.5f;

// ---------------- Timer-driven sampling flag ----------------
// Volatile because it is modified inside a timer callback
volatile bool sampleDue = false;

// mbed ticker used for interrupt-style scheduling
mbed::Ticker sampler;

// Timer callback function
// This function is intentionally kept short and only sets a flag
void onSampleTick() {
  sampleDue = true;
}

void setup() {
  // USB serial communication to laptop (debugging and assessment evidence)
  Serial.begin(115200);

  // UART communication to MKR1010 receiver node
  Serial1.begin(9600);

  // Initialise HS300x temperature and humidity sensor (I2C)
  HS300x.begin();

  // Start periodic timer interrupt using mbed::Ticker
  sampler.attach(&onSampleTick, SAMPLE_PERIOD_S);

  Serial.println("Nano 33 BLE Sense Rev2 - Ticker Sampling + UART CSV");
  Serial.println("CSV Format: ts,lightPct,gasPct,tempC,humRH");
}

void loop() {
  // Only execute sampling logic when triggered by timer
  if (!sampleDue) return;

  // Clear sampling flag before processing
  sampleDue = false;

  // ---------------- Analogue sensor acquisition (ADC) ----------------

  // Read raw ADC values
  int ldrRaw = analogRead(LDR_PIN);
  int gasRaw = analogRead(GAS_PIN);

  // Convert ADC readings to percentage values (0–100%)
  float lightPct = (ldrRaw / 1023.0f) * 100.0f;
  float gasPct   = (gasRaw / 1023.0f) * 100.0f;

  // Convert ADC readings to voltages using 3.3 V reference
  float ldrV = (ldrRaw * 3.3f) / 1023.0f;
  float gasV = (gasRaw * 3.3f) / 1023.0f;

  // ---------------- Digital sensor acquisition (I2C) ----------------

  // Read temperature and humidity from HS300x sensor
  float tempC = HS300x.readTemperature();
  float humRH = HS300x.readHumidity();

  // Timestamp for current sampling cycle
  unsigned long ts = millis();

  // ---------------- Human-readable output (USB Serial) ----------------

  Serial.println("---- SENSOR READINGS ----");

  Serial.print("LDR   raw: ");
  Serial.print(ldrRaw);
  Serial.print(" | V: ");
  Serial.print(ldrV, 3);
  Serial.print(" V | Brightness: ");
  Serial.print(lightPct, 1);
  Serial.println(" %");

  Serial.print("GAS   raw: ");
  Serial.print(gasRaw);
  Serial.print(" | V: ");
  Serial.print(gasV, 3);
  Serial.print(" V | Gas Level: ");
  Serial.print(gasPct, 1);
  Serial.println(" %");

  Serial.print("TEMP: ");
  Serial.print(tempC, 2);
  Serial.print(" C | HUM: ");
  Serial.print(humRH, 2);
  Serial.println(" %RH");
  Serial.println();

  // ---------------- UART CSV transmission to MKR1010 ----------------

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
}
