#include <math.h>

// --- Rotary Encoder Pins ---
const int clkPin = 22;
const int dtPin = 20;
const int swPin = 21;

// --- LED 74HC595 Pins (Program Selection LEDs) ---
const int ledDataPin = 17;    // DS
const int ledClockPin = 18;   // SHCP
const int ledLatchPin = 19;   // STCP

// --- 7-Segment 74HC595 Pins ---
const int segDataPin = 6;     // DS
const int segClockPin = 4;    // SHCP
const int segLatchPin = 5;    // STCP

// --- Buzzer Pin ---
const int buzzerPin = 10;

// --- Temperature Sensor Pin ---
const int tempPin = 26;

// --- Temperature Button ---
const int tempButtonPin = 3;

// --- Pause/Play Button ---
const int pauseButtonPin = 2;
// -- OUTPUT LED--- 

const int Yellow = 9;
const int Green = 7;
const int Red = 8;

// --- Program Variables ---
int programTimes[] = {5, 10, 15, 20, 25, 30, 35, 40};  // Seconds
int currentProgram = 0;
int lastClk = digitalRead(clkPin);
bool programRunning = false;
bool isPaused = false;
bool systemLocked = false;
// --- 7-Segment Map (Common Cathode) ---
const byte digitToSegment[] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
};

// --- LED Shift Out ---
void shiftOutLeds(byte value) {
  digitalWrite(ledLatchPin, LOW);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(ledDataPin, (value >> i) & 1);
    digitalWrite(ledClockPin, HIGH);
    digitalWrite(ledClockPin, LOW);
  }
  digitalWrite(ledLatchPin, HIGH);
}

// --- 7-Segment Display Shift Out (3 digits) ---
void displayNumber(int number) {
  String numStr = String(number);
  while (numStr.length() < 3) numStr = "0" + numStr;

  digitalWrite(segLatchPin, LOW);
  for (int i = 0; i < numStr.length(); i++) {
    byte val = digitToSegment[numStr.charAt(i) - '0'];
    for (int j = 7; j >= 0; j--) {
      digitalWrite(segDataPin, (val >> j) & 1);
      digitalWrite(segClockPin, HIGH);
      digitalWrite(segClockPin, LOW);
    }
  }
  digitalWrite(segLatchPin, HIGH);
}

// --- Set LED for selected program ---
void selectProgram(int index) {
  shiftOutLeds(1 << index);
  Serial.print("Selected Program: ");
  Serial.println(index);
}

// --- Run Selected Program ---
void runProgram(int duration) {
  programRunning = true;
 // Serial.print("> Program ");
 // Serial.print(currentProgram);
  Serial.print(" started for ");
  Serial.print(duration);
  Serial.println(" seconds");

  for (int i = duration; i > 0; i--) {
    displayNumber(i);
    digitalWrite(Green, HIGH);

    if (digitalRead(tempButtonPin) == HIGH) {
    float temp = readTemperature();
    int roundedTemp = (int)temp;
    digitalWrite(Green, LOW);
    digitalWrite(Yellow,HIGH);
    displayNumber(roundedTemp);
    delay(2000);  // Display the temperature for a second
    digitalWrite(Yellow,LOW); 
    digitalWrite(Green, HIGH);
    }


        // --- Pause/Resume Check ---
    if (digitalRead(pauseButtonPin) == HIGH) {
      isPaused = !isPaused;
      delay(300); // debounce
    }

    // --- While Paused ---
    while (isPaused) {
      
      displayNumber(i);
      delay(300);
      displayNumber(0x00);
      delay(300);

      if (digitalRead(pauseButtonPin) == HIGH) {
        isPaused = false;
        delay(300); // debounce
      }
    }

         // --- System Lock ---

    if (digitalRead(tempButtonPin) == HIGH && digitalRead(pauseButtonPin) == HIGH) {   

    systemLocked = !systemLocked;
    delay(300); // debounce
     }

    // --- While Paused ---
    while (systemLocked) {
      digitalWrite(Red, HIGH);

        for (int i = duration; i > 0; i--) {
        displayNumber(i);
        }

        if (i == 0){
          tone(buzzerPin, 1000, 500); // Beep
          displayNumber(0);
        }

      if (digitalRead(tempButtonPin) == HIGH && digitalRead(pauseButtonPin) == HIGH) {
        systemLocked = false;
        digitalWrite(Red, LOW);
        delay(300); // debounce
      }
    }



    delay(1000);
  }

  displayNumber(0);
  digitalWrite(Green, LOW);
  

  tone(buzzerPin, 1000, 500); // Beep

  programRunning = false;
}

// --- Read Temperature from NTC Thermistor ---
float readTemperature() {
  int sensorValue = analogRead(tempPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float resistance = (5.0 - voltage) * 10000.0 / voltage; // 10k resistor
  float temperature = (1 / (log(resistance / 10000.0) / 3950 + 1 / 298.15)) - 273.15;
  return temperature;
}

// --- Setup ---
void setup() {
  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(ledDataPin, OUTPUT);
  pinMode(ledClockPin, OUTPUT);
  pinMode(ledLatchPin, OUTPUT);
  pinMode(segDataPin, OUTPUT);
  pinMode(segClockPin, OUTPUT);
  pinMode(segLatchPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(tempButtonPin, INPUT_PULLUP);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);

  selectProgram(currentProgram);
  displayNumber(0);
  Serial.begin(9600);
}

// --- Main Loop ---
void loop() {
  // Rotary encoder handling
  if (!programRunning) {
    int currentClk = digitalRead(clkPin);
    if (currentClk != lastClk && currentClk == LOW) {
      if (digitalRead(dtPin) != currentClk) {
        currentProgram = (currentProgram + 1) % 8;
      } else {
        currentProgram = (currentProgram - 1 + 8) % 8;
      }
      selectProgram(currentProgram);
      delay(100);
    }
    lastClk = currentClk;

    // Encoder push button to start program
    if (digitalRead(swPin) == LOW) {
      runProgram(programTimes[currentProgram]);
      currentProgram = 0;
      selectProgram(currentProgram);
      displayNumber(0);
      delay(300);
    }
  }

  // If button on pin 3 is pressed, read temperature and show on display
     if (digitalRead(tempButtonPin) == HIGH) {
    float temp = readTemperature();
    int roundedTemp = (int)temp;
    digitalWrite(Yellow,HIGH);
    displayNumber(roundedTemp);
    delay(2000);  // Display the temperature for a second
    displayNumber(0);
    digitalWrite(Yellow,LOW);  }



     // --- System Lock ---

 if (digitalRead(tempButtonPin) == HIGH && digitalRead(pauseButtonPin) == HIGH) {   

    systemLocked = !systemLocked;
    delay(300); // debounce
     }

    // --- While Paused ---
    while (systemLocked) {
      digitalWrite(Red, HIGH);

      if (digitalRead(tempButtonPin) == HIGH && digitalRead(pauseButtonPin) == HIGH) {
        systemLocked = false;
        digitalWrite(Red, LOW);
        delay(300); // debounce
      }
    }
}
