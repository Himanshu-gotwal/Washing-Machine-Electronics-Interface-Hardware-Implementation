🧺  # Washing Machine Electronics Interface & Hardware Implementation #
A DIY embedded project to simulate a washing machine's electronic control interface using Raspberry Pi Pico, 74HC595 shift registers, rotary encoder, temperature sensor, and 7-segment display, developed using Arduino IDE and Thonny (MicroPython and cpp). 

# 🔧 Features #
Rotary encoder to select wash programs (8 in total)

74HC595-based LEDs to indicate selected program

7-segment display to show timer countdown or temperature

Pause/Play functionality

System lock for safety

Real-time temperature display using thermistor

Status LEDs (Green: Running, Yellow: Temp, Red: Lock)

Buzzer alert at end of program

# 🖥️ Software Stack #
Arduino IDE – for writing and uploading embedded C code

Thonny IDE – for MicroPython code testing (if applicable)

Serial Monitor – for debugging and output

Git – for version control

💡 Hardware Used
Raspberry Pi Pico

2x 74HC595 Shift Registers

Rotary Encoder with Push Button

NTC Temperature Sensor

3x LEDs (Red, Green, Yellow)

Buzzer

7-Segment Display (3-digit, common cathode)

Buttons for temperature and pause

10k Resistor (for thermistor divider)

Breadboard & jumper wires

5V Power Supply

🚀 Getting Started
1. Clone the Repository
2. Upload Code
Open main.ino in Arduino IDE

Select board: Raspberry Pi Pico

Connect and upload

3. Hardware Connections
Refer to the schematic image or schematics in the docs/ folder for full wiring details.

📁 Additional Images & Files - Google Drive

📈 Future Scope
🌐 Add IoT-based control using a web interface or smartphone app

📲 Access washing data and logs remotely

💧 Integrate water level sensors

🧠 Add motor control for drum movement

📟 Replace 7-segment display with LCD or OLED

🔗 Bluetooth/WiFi-based remote control

📚 References
Raspberry Pi Pico Datasheet

74HC595 Shift Register Datasheet

Arduino IDE Docs

MicroPython on Pico

Various tutorials from YouTube & blogs
