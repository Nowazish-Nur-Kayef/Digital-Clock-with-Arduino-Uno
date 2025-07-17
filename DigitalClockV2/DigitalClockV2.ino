/*
 * Digital Clock V2.0 - Interactive Alarm Clock System
 * Author: Nowazish Nur Kayef
 * 
 * Features:
 * - 4 Button Interface (pins 2, 3, 4, 5)
 * - Interactive time adjustment
 * - Dual alarm system (static + dynamic)
 * - Professional user interface
 * - Smart alarm management
 * 
 * Hardware Requirements:
 * - Arduino Uno
 * - 16x2 LCD with I2C backpack
 * - 5V Buzzer
 * - 4x Push buttons (with internal pull-up resistors)
 * 
 * Pin Configuration:
 * - LCD: SDA (A4), SCL (A5), VCC (5V), GND
 * - Buzzer: Pin 8
 * - Buttons: Pin 2 (Minute+), Pin 3 (Second-), Pin 4 (Quick Alarm), Pin 5 (Stop Alarm)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Time variables
int hour = 0;
int minute = 0;
int second = 0;

// Static alarm configuration (predefined alarms)
const int NUM_STATIC_ALARMS = 9;
int staticAlarmHours[NUM_STATIC_ALARMS] = {4, 4, 4, 4, 5, 15, 17, 19, 22};
int staticAlarmMinutes[NUM_STATIC_ALARMS] = {50, 55, 57, 59, 0, 0, 30, 30, 0};

// Dynamic alarm (user-set)
int dynamicAlarmHour = -1;    // -1 means no alarm set
int dynamicAlarmMinute = -1;
bool dynamicAlarmActive = false;

// Button configuration
const int BTN_MINUTE_PLUS = 2;    // Increment minute
const int BTN_SECOND_MINUS = 3;   // Decrement second  
const int BTN_QUICK_ALARM = 4;    // Set 20-minute alarm
const int BTN_STOP_ALARM = 5;     // Stop/dismiss alarms

// Button state management
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;

// Buzzer control
const int buzzerPin = 8;
bool buzzerActive = false;
unsigned long buzzerStartMillis = 0;
bool alarmTriggeredThisMinute = false;

// Timing control
unsigned long previousSecondMillis = 0;
const unsigned long secondInterval = 1000;

// Display management
String lastDisplayedTime = "";
String currentMessage = "";
unsigned long messageStartTime = 0;
const unsigned long messageDisplayTime = 3000; // Show messages for 3 seconds

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Initialize buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // Initialize buttons with internal pull-up resistors
  pinMode(BTN_MINUTE_PLUS, INPUT_PULLUP);
  pinMode(BTN_SECOND_MINUS, INPUT_PULLUP);
  pinMode(BTN_QUICK_ALARM, INPUT_PULLUP);
  pinMode(BTN_STOP_ALARM, INPUT_PULLUP);
  
  // Professional boot sequence
  showBootSequence();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Handle button inputs (non-blocking)
  handleButtons();
  
  // Update clock every second
  if (currentMillis - previousSecondMillis >= secondInterval) {
    previousSecondMillis = currentMillis;
    
    updateTime();
    checkAlarms();
    updateDisplay();
  }
  
  // Auto-turn off buzzer after 30 seconds
  if (buzzerActive && (currentMillis - buzzerStartMillis >= 30000)) {
    stopBuzzer();
  }
  
  // Clear message after display time
  if (currentMessage != "" && (currentMillis - messageStartTime >= messageDisplayTime)) {
    currentMessage = "";
    updateDisplay(); // Refresh display without message
  }
}

// Professional boot sequence with enhanced animation
void showBootSequence() {
  lcd.clear();
  
  // Step 1: Show system name
  lcd.setCursor(0, 0);
  lcd.print("Digital Clock");
  lcd.setCursor(0, 1);
  lcd.print("     V2.0");
  delay(2000);
  
  // Step 2: Loading animation
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.print("#");
    delay(100);
  }
  
  // Step 3: Author credit with typing effect
  lcd.clear();
  String authorText = "  Made By Kayef";
  lcd.setCursor(0, 0);
  lcd.print("Interactive Mode");
  lcd.setCursor(0, 1);
  
  for (int i = 0; i < authorText.length(); i++) {
    lcd.print(authorText[i]);
    delay(150);
  }
  
  delay(1500);
  
  // Step 4: Ready message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Press any button");
  delay(2000);
  
  lcd.clear();
}

// Handle all button inputs with debouncing
void handleButtons() {
  int buttons[] = {BTN_MINUTE_PLUS, BTN_SECOND_MINUS, BTN_QUICK_ALARM, BTN_STOP_ALARM};
  
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(buttons[i]);
    
    // Check if button state changed
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    // If enough time has passed, consider it a valid button press
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != currentButtonState[i]) {
        currentButtonState[i] = reading;
        
        // Button pressed (LOW due to pull-up)
        if (currentButtonState[i] == LOW) {
          handleButtonPress(i);
        }
      }
    }
    
    lastButtonState[i] = reading;
  }
}

// Process individual button presses
void handleButtonPress(int buttonIndex) {
  switch (buttonIndex) {
    case 0: // Minute Plus (Pin 2)
      incrementMinute();
      break;
    case 1: // Second Minus (Pin 3)
      decrementSecond();
      break;
    case 2: // Quick Alarm (Pin 4)
      setQuickAlarm();
      break;
    case 3: // Stop Alarm (Pin 5)
      stopAlarm();
      break;
  }
}

// Increment minute by 1
void incrementMinute() {
  minute++;
  if (minute >= 60) {
    minute = 0;
    hour++;
    if (hour >= 24) {
      hour = 0;
    }
  }
  showMessage("Time: +" + String(minute < 10 ? "0" : "") + String(minute) + "m");
  Serial.println("Minute incremented to: " + String(minute));
}

// Decrement second by 1
void decrementSecond() {
  second--;
  if (second < 0) {
    second = 59;
    minute--;
    if (minute < 0) {
      minute = 59;
      hour--;
      if (hour < 0) {
        hour = 23;
      }
    }
  }
  showMessage("Time: -1s");
  Serial.println("Second decremented to: " + String(second));
}

// Set 20-minute alarm from current time
void setQuickAlarm() {
  dynamicAlarmHour = hour;
  dynamicAlarmMinute = minute + 20;
  
  // Handle minute overflow
  if (dynamicAlarmMinute >= 60) {
    dynamicAlarmMinute -= 60;
    dynamicAlarmHour++;
    if (dynamicAlarmHour >= 24) {
      dynamicAlarmHour = 0;
    }
  }
  
  dynamicAlarmActive = true;
  showMessage("Alarm: " + formatDigits(dynamicAlarmHour) + ":" + formatDigits(dynamicAlarmMinute));
  Serial.println("Quick alarm set for: " + formatDigits(dynamicAlarmHour) + ":" + formatDigits(dynamicAlarmMinute));
}

// Stop active alarms or dismiss temporary alarms
void stopAlarm() {
  if (buzzerActive) {
    stopBuzzer();
    showMessage("Alarm Stopped");
    Serial.println("Alarm stopped by user");
  } else if (dynamicAlarmActive) {
    dynamicAlarmActive = false;
    dynamicAlarmHour = -1;
    dynamicAlarmMinute = -1;
    showMessage("Alarm Dismissed");
    Serial.println("Dynamic alarm dismissed");
  } else {
    showMessage("No Active Alarm");
    Serial.println("No alarm to stop/dismiss");
  }
}

// Stop buzzer and reset alarm trigger flag
void stopBuzzer() {
  digitalWrite(buzzerPin, LOW);
  buzzerActive = false;
  alarmTriggeredThisMinute = false;
}

// Display temporary message on LCD
void showMessage(String message) {
  currentMessage = message;
  messageStartTime = millis();
  updateDisplay();
}

// Increment time (fake clock)
void updateTime() {
  second++;
  if (second >= 60) {
    second = 0;
    minute++;
    alarmTriggeredThisMinute = false; // Reset alarm trigger flag each minute
    if (minute >= 60) {
      minute = 0;
      hour++;
      if (hour >= 24) {
        hour = 0;
      }
    }
  }
}

// Check both static and dynamic alarms
void checkAlarms() {
  // Prevent alarm re-triggering in the same minute
  if (alarmTriggeredThisMinute || second != 0) {
    return;
  }
  
  bool alarmTriggered = false;
  
  // Check static alarms
  for (int i = 0; i < NUM_STATIC_ALARMS; i++) {
    if (hour == staticAlarmHours[i] && minute == staticAlarmMinutes[i]) {
      triggerAlarm("Static Alarm");
      alarmTriggered = true;
      break;
    }
  }
  
  // Check dynamic alarm
  if (!alarmTriggered && dynamicAlarmActive && 
      hour == dynamicAlarmHour && minute == dynamicAlarmMinute) {
    triggerAlarm("Quick Alarm");
    dynamicAlarmActive = false; // One-time alarm
    dynamicAlarmHour = -1;
    dynamicAlarmMinute = -1;
  }
}

// Trigger alarm with buzzer and message
void triggerAlarm(String alarmType) {
  buzzerActive = true;
  buzzerStartMillis = millis();
  alarmTriggeredThisMinute = true;
  digitalWrite(buzzerPin, HIGH);
  showMessage(alarmType + " RING!");
  Serial.println(alarmType + " triggered at " + formatDigits(hour) + ":" + formatDigits(minute));
}

// Update LCD display with time and status
void updateDisplay() {
  String currentTime = formatDigits(hour) + ":" + formatDigits(minute) + ":" + formatDigits(second);
  
  // Always update if there's a message or time changed
  if (currentMessage != "" || currentTime != lastDisplayedTime) {
    lcd.clear();
    
    // Row 1: Time or message
    if (currentMessage != "") {
      lcd.setCursor(0, 0);
      lcd.print(currentMessage);
    } else {
      lcd.setCursor(3, 0);
      lcd.print("Time");
    }
    
    // Row 2: Current time and next alarm info
    lcd.setCursor(2, 1);
    lcd.print(currentTime);
    
    // Show next alarm indicator if no message
    if (currentMessage == "" && dynamicAlarmActive) {
      lcd.setCursor(13, 1);
      lcd.print("A"); // Alarm indicator
    }
    
    lastDisplayedTime = currentTime;
  }
}

// Format single digit numbers to two digits
String formatDigits(int number) {
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}