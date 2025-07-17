/*
 * Button Test Sketch for Digital Clock V2.0
 * Author: Nowazish Nur Kayef
 * 
 * Purpose: Test button connections and responsiveness
 * Hardware: 4 push buttons connected to pins 2, 3, 4, 5
 * 
 * This sketch helps verify that all buttons are properly connected
 * and responsive before using the full V2.0 clock system.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button pins
const int BTN_MINUTE_PLUS = 2;    // Increment minute
const int BTN_SECOND_MINUS = 3;   // Decrement second  
const int BTN_QUICK_ALARM = 4;    // Set quick alarm
const int BTN_STOP_ALARM = 5;     // Stop alarm

// Button state tracking
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;

// Button press counters
int buttonPressCount[4] = {0, 0, 0, 0};
String buttonNames[] = {"Minute+", "Second-", "Alarm+", "Stop"};

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Initialize buttons with pull-up resistors
  pinMode(BTN_MINUTE_PLUS, INPUT_PULLUP);
  pinMode(BTN_SECOND_MINUS, INPUT_PULLUP);
  pinMode(BTN_QUICK_ALARM, INPUT_PULLUP);
  pinMode(BTN_STOP_ALARM, INPUT_PULLUP);
  
  // Show test information
  lcd.setCursor(0, 0);
  lcd.print("Button Test V2.0");
  lcd.setCursor(0, 1);
  lcd.print("Press any button");
  
  Serial.println("Button Test for Digital Clock V2.0");
  Serial.println("Press buttons to test functionality");
  Serial.println("Button mapping:");
  Serial.println("Pin 2: Minute+ button");
  Serial.println("Pin 3: Second- button");
  Serial.println("Pin 4: Quick Alarm button");
  Serial.println("Pin 5: Stop Alarm button");
  Serial.println("================================");
}

void loop() {
  handleButtons();
  delay(10); // Small delay for stability
}

void handleButtons() {
  int buttons[] = {BTN_MINUTE_PLUS, BTN_SECOND_MINUS, BTN_QUICK_ALARM, BTN_STOP_ALARM};
  
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(buttons[i]);
    
    // Check if button state changed
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    // If enough time has passed, consider it a valid state change
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != currentButtonState[i]) {
        currentButtonState[i] = reading;
        
        // Button pressed (LOW due to pull-up)
        if (currentButtonState[i] == LOW) {
          buttonPressed(i);
        }
      }
    }
    
    lastButtonState[i] = reading;
  }
}

void buttonPressed(int buttonIndex) {
  buttonPressCount[buttonIndex]++;
  
  // Update LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(buttonNames[buttonIndex] + " pressed");
  lcd.setCursor(0, 1);
  lcd.print("Count: " + String(buttonPressCount[buttonIndex]));
  
  // Serial output for debugging
  Serial.println("Button " + String(buttonIndex + 1) + " (" + buttonNames[buttonIndex] + ") pressed");
  Serial.println("Press count: " + String(buttonPressCount[buttonIndex]));
  Serial.println("Pin " + String(buttonIndex + 2) + " working correctly");
  Serial.println("------------------------");
  
  // Visual feedback - brief message
  delay(500);
  
  // Show summary
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Summary:");
  lcd.setCursor(0, 1);
  lcd.print("All: " + String(buttonPressCount[0]) + " " + String(buttonPressCount[1]) + " " + String(buttonPressCount[2]) + " " + String(buttonPressCount[3]));
}