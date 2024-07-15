//#include "ACS712.h"
//ACS712 sensor(ACS712_05B, A3);


// Define pins for output
const int pinA = 2;  // LSB -> A0
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;  // MSB -> A3

// Define analog input pins for adjusting clock period
const int analogInput1 = A0; // Connect input 1 to A0
const int analogInput2 = A5; // Connect input 2 to A5
const int analogInput3 = A3; // Connect input 3 to A3

// Define pin for the push button
const int buttonPin = 6; // Connect the push button to pin 6

float V = 0; 

// Variables to store current count and time
int count = 0;
unsigned long previousMillis = 0;
const int numReadings = 10; // Number of analog readings per clock cycle
float readingsInput1[numReadings];  // Array to store analog readings for input 1
float readingsInput2[numReadings];  // Array to store analog readings for input 2
float readingsInput3[numReadings];  // Array to store analog readings for input 2
int readIndex = 0;           // Index of the current reading

float input1Value = 0;
float input2Value = 0;
float input3Value = 0;

bool buttonState = false;   // Current state of the button
bool lastButtonState = false;  // Previous state of the button
unsigned long lastDebounceTime = 0;  // Time at which the button last changed state
unsigned long debounceDelay = 50;  // Delay in milliseconds for debouncing the button

bool countingStarted = false; // Flag to indicate if counting has started
bool countingCompleted = false; // Flag to indicate if counting has completed

  int adc = 0;
  float voltage_acs =0;
  float current = 0;

void setup() {

   // Initialize Serial communication
  Serial.begin(9600); // Set baud rate to 9600 (or any other baud rate you prefer)

  // Initialize digital pins as outputs
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);

  // Initialize analog input pins
  pinMode(analogInput1, INPUT);
  pinMode(analogInput2, INPUT);

  // Initialize the arrays of readings to 0
  for (int i = 0; i < numReadings; i++) {
    readingsInput1[i] = 0;
    readingsInput2[i] = 0;
    readingsInput3[i] = 0;
  }

  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  
  
  // sensor.calibrate();
}

void loop() {
  // Read the state of the button
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Reset debounce timer
  }

  // Check if debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Update button state if it has been stable for a certain time
    if (reading != buttonState) {
      buttonState = reading;

      // If the button is pressed (LOW), start the operation
      if (buttonState == LOW && !countingStarted) {
        countingStarted = true; // Set the flag to indicate counting has started

        // Get current time
        unsigned long currentMillis = millis();


        // Calculate clock period based on analog input values
        unsigned long period = 7000; // Map input2 to adjust the range furtherr

        // Reset count and previousMillis
        count = 0;
        previousMillis = currentMillis;

        // Perform counting and output operations
        while (millis() - previousMillis < period * 16) { // Count for 16 cycles
          if (millis() - previousMillis >= period) {
            // Increment count and reset if it reaches 16 (4-bit overflow)
            count++;
            if (count == 16) {
              count = 0;
              countingCompleted = true; // Set flag to indicate counting has completed
              break; // Exit the counting loop once completed
            }

                    // Read analog input 1 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput1[i] = analogRead(analogInput1);
          delay(100); // Delay between readings, adjust as needed
         // Serial.println(readingsInput1[i]);
        }

        // Read analog input 2 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput2[i] = analogRead(analogInput2);
          delay(100); // Delay between readings, adjust as needed
          
          
           }

                // Read analog input 2 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput3[i] = analogRead(analogInput3);
          delay(100); // Delay between readings, adjust as needed
        }



        // Calculate average of analog input 1 readings
        input1Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input1Value += readingsInput1[i];
        }
        input1Value /= numReadings;

        input1Value = input1Value*(25.0/1024.0);

        // Calculate average of analog input 2 readings
        input2Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input2Value += readingsInput2[i];
        }
        input2Value /= numReadings;


   //     input2Value = (460*input2Value)/(5-input2Value);

                // Calculate average of analog input 2 readings
        input3Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input3Value += readingsInput3[i];
        }
        input3Value /= numReadings;

          voltage_acs = input3Value*5/1023.0;
          input3Value = (voltage_acs-2.5)/0.185;
     //     Serial.print("Current : ");




          // Initialize the arrays of readings to 0
          for (int i = 0; i < numReadings; i++) {
              readingsInput1[i] = 0;
              readingsInput2[i] = 0;
              readingsInput3[i] = 0;
            }


            // Print input values to serial monitor
        //    Serial.print("Input 1: ");
            Serial.print(input1Value);
       //     Serial.print("\t Input 2: ");
       
      Serial.print(" ");
            Serial.println(input2Value);
      //      Serial.print(" ");
    //        Serial.println(input3Value);

            delay(1000);

            // Output binary count to the pins
            digitalWrite(pinA, (count >> 0) & 0x01);
            digitalWrite(pinB, (count >> 1) & 0x01);
            digitalWrite(pinC, (count >> 2) & 0x01);
            digitalWrite(pinD, (count >> 3) & 0x01);

            delay(1000);


            // Update previousMillis
            previousMillis += period;
          }
        }

                     // Read analog input 1 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput1[i] = analogRead(analogInput1);
          delay(100); // Delay between readings, adjust as needed
         // Serial.println(readingsInput1[i]);
        }

        // Read analog input 2 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput2[i] = analogRead(analogInput2);
          delay(100); // Delay between readings, adjust as needed
        }

                // Read analog input 2 multiple times per clock cycle
        for (int i = 0; i < numReadings; i++) {
          readingsInput3[i] = analogRead(analogInput3);
          delay(100); // Delay between readings, adjust as needed
        }



        // Calculate average of analog input 1 readings
        input1Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input1Value += readingsInput1[i];
        }
        input1Value /= numReadings;

        input1Value = input1Value*(25.0/1024.0);

        // Calculate average of analog input 2 readings
        input2Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input2Value += readingsInput2[i];
              }
        input2Value /= numReadings;


    //    input2Value = (460*input2Value)/(5-input2Value);

                // Calculate average of analog input 2 readings
        input3Value = 0;
        for (int i = 0; i < numReadings; i++) {
          input3Value += readingsInput3[i];
        }
        input3Value /= numReadings;

          voltage_acs = input3Value*5/1023.0;
          input3Value = (voltage_acs-2.5)/0.185;
     //     Serial.print("Current : ");




          // Initialize the arrays of readings to 0
          for (int i = 0; i < numReadings; i++) {
              readingsInput1[i] = 0;
              readingsInput2[i] = 0;
              readingsInput3[i] = 0;
            }


            // Print input values to serial monitor
        //    Serial.print("Input 1: ");
            Serial.print(input1Value);
       //     Serial.print("\t Input 2: ");
            Serial.print(" ");
            Serial.println(input2Value);
  //          Serial.print(" ");
//            Serial.println(input3Value);



        // Set all outputs low after one cycle
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
        digitalWrite(pinC, LOW);
        digitalWrite(pinD, LOW);
      }
    }
  }


  // If button is released and counting has started and completed, reset the flags
  if (buttonState == HIGH && countingStarted && countingCompleted) {
    countingStarted = false;
    countingCompleted = false;
  }
  // Update lastButtonState
  lastButtonState = reading;

}
  
