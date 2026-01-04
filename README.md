Smart Home Environmental Monitor
1. Project Overview
This project presents the design and implementation of a Smart Home Environmental Monitoring System based on embedded microcontrollers. The system continuously monitors environmental parameters including light intensity, gas concentration, temperature, and humidity, and evaluates environmental safety conditions using rule-based decision logic.
The system integrates hardware sensing, embedded software, serial communication, and offline data analysis.

2. System Architecture
The system follows a distributed two-node architecture:
A Sensor Node, responsible for data acquisition and structured telemetry generation.
A Receiver Node, responsible for data reception, alert display, and user feedback.
Communication between nodes is achieved using UART, while peripheral devices (LCD display) are interfaced using I²C.

3. System Diagram

    SN[Sensor Node\nArduino Nano 33 BLE Sense Rev2\nLDR | MQ-2 | HS300x]
    RN[Receiver Node\nArduino MKR / Uno\nLCD Display]

    SN -- UART --> RN
    RN -- I²C --> LCD[LCD Module]


4. Software Design
   
4.1 Sensor Node Software

The sensor node software:

Uses interrupt driven timing for periodic sampling
Reads analogue sensor values via ADC
Reads temperature and humidity using the on-board HS300x sensor
Formats readings into structured CSV-style telemetry
Transmits sensor data via UART

4.2 Alert and Decision Logic
The alert and decision logic module analyses structured environmental data containing:
Timestamp
Light level
Gas level
Temperature
Humidity
Based on predefined thresholds, the system classifies conditions into:
SAFE
WARNING
ALERT

The output includes:
A status message
A CSV formatted message for validation and logging
This module satisfies requirements for self-contained embedded software engineering and automated decision-making.

5. Hardware Design
5.1 Sensor Node Hardware
Arduino Nano 33 BLE Sense Rev2
LDR with voltage divider connected to analogue input
MQ-2 gas sensor with RC low-pass filtering and op-amp signal conditioning
Shared ground reference for stable ADC operation

5.2 Receiver Node Hardware
Arduino-based receiver (Nano / MKR / Uno)
UART interface for sensor data reception
I²C interface for 16×2 LCD display
Correct voltage-level management (3.3 V / 5 V) to prevent hardware conflicts

6. Data Logging and Analysis
Sensor readings are logged to text files and analysed offline using Python. The analysis includes:
Combined sensor plots over time
Threshold-based low-light detection
Validation of sensor stability and independence

7. Repository Structure
7.1 Arduino Source Code
smarthomesensornode.ino – Sensor acquisition and UART transmission
smarthomeReceivernode.ino – UART reception and LCD output
smarthomeAlertDecisionLogic.ino – Alert classification logic
SmarthomeTimer_interrupter_node.ino – Interrupt-driven timing
7.2 Data Logs
Sensor reading(TI).txt – Normal operating conditions
low-light.txt – Captured low-light event
7.3 Python Analysis
Combined sensor readings over time.ipynb
Low-light detection.ipynb

8. Tools and Technologies
Arduino Nano 33 BLE Sense Rev2
Arduino IDE (C/C++)
Python (NumPy, Matplotlib, Jupyter Notebook)
UART and I²C communication protocols

10. Learning Outcomes
Through this project, practical experience was gained in:
Interrupt driven embedded system design
Analogue signal acquisition and conditioning
Structured telemetry generation
Hardware software integration
Data driven decision logic and validation

13. Appendices
Appendix A – Communication Protocols

UART used for inter node communication with shared ground reference
I²C used for LCD display interfacing

Appendix B – Sensor Calibration and Thresholds
Brightness threshold: 10% (low-light detection)
Gas level monitored for abnormal deviations
Temperature and humidity monitored for environmental stability

Appendix C – Offline Analysis
Python-based post-processing validates real-time system behaviour and confirms event detection observed during live operation.


