/*
  Software-Only Alert & Decision-Logic Module
  -------------------------------------------
  Purpose:
  - Process environmental readings in the same format as the main system
  - Input data format:
      timestamp, light%, gas%, temperature, humidity
  - Classify system state into:
      SAFE, WARNING, or ALERT
  - Output:
      1) Status messages
      2) Compact CSV status log for validation
*/

// ---------------- input data ----------------
// These values would normally come from logged CSV files
unsigned long timestamp = 1000;   // milliseconds
float lightPct = 5.0;             // low ambient light
float gasPct   = 60.4;            // high gas level
float tempC    = 19.0;            // temperature (°C)
float humPct   = 34.0;            // humidity (%RH)

// ---------------- Threshold definitions ----------------
// Thresholds chosen based on observed sensor behaviour
const float GAS_ALERT_THRESHOLD   = 60.0;  // gas safety limit
const float LIGHT_WARNING_LEVEL   = 10.0;  // low light warning

void setup() {
  // Serial used only for displaying results
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Software Only Alert & Decision Logic");
  Serial.println("-----------------------------------");

  // ---------------- Decision logic ----------------
  String state = "SAFE";

  // Gas concentration has highest priority
  if (gasPct >= GAS_ALERT_THRESHOLD) {
    state = "ALERT";
  }
  // Low light produces a warning only
  else if (lightPct < LIGHT_WARNING_LEVEL) {
    state = "WARNING";
  }

  // ---------------- Human-readable output ----------------
  Serial.print("[");
  Serial.print(timestamp);
  Serial.print(" ms] State: ");
  Serial.println(state);

  Serial.print("Light: "); Serial.print(lightPct); Serial.println(" %");
  Serial.print("Gas: ");   Serial.print(gasPct);   Serial.println(" %");
  Serial.print("Temp: ");  Serial.print(tempC);    Serial.println(" C");
  Serial.print("Hum: ");   Serial.print(humPct);   Serial.println(" %RH");

  // ---------------- CSV status log ----------------
  // Format: timestamp,state,light%,gas%,tempC,hum%
  Serial.println();
  Serial.println("CSV Status Log:");
  Serial.print(timestamp);
  Serial.print(",");
  Serial.print(state);
  Serial.print(",");
  Serial.print(lightPct);
  Serial.print(",");
  Serial.print(gasPct);
  Serial.print(",");
  Serial.print(tempC);
  Serial.print(",");
  Serial.println(humPct);
}

void loop() {

}
