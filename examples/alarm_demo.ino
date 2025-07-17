/*
 * Alarm Demo Sketch for Digital Clock V2.0
 * Author: Nowazish Nur Kayef
 * 
 * Purpose: Demonstrate alarm features and timing functionality
 * Features: Fast time progression, automatic alarm testing, user interaction
 * 
 * This sketch runs a demo of the alarm system with accelerated time
 * to quickly show how both static and dynamic alarms work.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Time variables (accelerated for demo)
int hour = 4;      // Start near alarm times for quick demo
int minute = 48;
int second = 0;

// Demo alarm configuration (shorter intervals)
const int NUM_DEMO_ALARMS = 3;
int demoAlarmHours[NUM_DEMO_ALARMS] = {4, 4, 5};
int demoAlarmMinutes[NUM_DEMO_ALARMS] = {50, 55, 0};

// Dynamic alarm
int dynamicAlarmHour = -1;
int dynamicAlarmMinute = -1;
bool dynamicAlarmActive = false;

// Demo control
const int BTN_DEMO_ALARM = 4;    // Set demo alarm (3 minutes ahead)
const int BTN_STOP_ALARM = 5;    // Stop alarm

// Hardware
const int buzzerPin = 8;
bool buzzerActive = false;
unsigned long buzzerStartMillis = 0;
bool alarmTriggeredThisMinute = false;

// Timing (accelerated for demo - 1 second = 1 minute in demo)
unsigned long previousMinuteMillis = 0;
const unsigned long minuteInterval = 1000; // 1 second = 1 minute in demo

// Display
String lastDisplayedTime = "";
String demoMessage = "";
unsigned long messageStartTime = 0;
const unsigned long messageDisplayTime = 2000;

void setup() {
  Serial.begin(9600);
  
  // Initialize hardware
  lcd.begin(16, 2);
  lcd.backlight();
  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  pinMode(BTN_DEMO_ALARM, INPUT_PULLUP);
  pinMode(BTN_STOP_ALARM, INPUT_PULLUP);
  
  // Demo introduction
  showDemoIntro();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Handle button inputs
  handleDemoButtons();
  
  // Accelerated time progression (every second = 1 minute)
  if (currentMillis - previousMinuteMillis >= minuteInterval) {
    previousMinuteMillis = currentMillis;
    
    updateDemoTime();
    checkDemoAlarms();
    updateDemoDisplay();
  }
  
  // Auto-stop buzzer after 5 seconds in demo
  if (buzzerActive && (currentMillis - buzzerStartMillis >= 5000)) {
    stopBuzzer();
  }
  
  // Clear demo messages
  if (demoMessage != "" && (currentMillis - messageStartTime >= messageDisplayTime)) {
    demoMessage = "";
    updateDemoDisplay();
  }
}

void showDemoIntro() {
  lcd.clear();
  
  // Step 1: Demo title
  lcd.setCursor(0, 0);
  lcd.print("Alarm Demo V2.0");
  lcd.setCursor(0, 1);
  lcd.print("Fast Time Mode");
  delay(3000);
  
  // Step 2: Instructions
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pin4: Demo Alarm");
  lcd.setCursor(0, 1);
  lcd.print("Pin5: Stop Alarm");
  delay(3000);
  
  // Step 3: Demo info
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 sec = 1 minute");
  lcd.setCursor(0, 1);
  lcd.print("Watch alarms!");
  delay(2000);
  
  lcd.clear();
  
  Serial.println("=== Digital Clock V2.0 Alarm Demo ===");
  Serial.println("Time is accelerated: 1 second = 1 minute");
  Serial.println("Static alarms at: 4:50, 4:55, 5:00");
  Serial.println("Press button on Pin 4 to set 3-minute demo alarm");
  Serial.println("Press button on Pin 5 to stop alarms");
  Serial.println("====================================");
}

void handleDemoButtons() {
  // Demo alarm button (Pin 4)
  if (digitalRead(BTN_DEMO_ALARM) == LOW) {
    delay(200); // Simple debounce
    setDemoAlarm();
  }
  
  // Stop alarm button (Pin 5)
  if (digitalRead(BTN_STOP_ALARM) == LOW) {
    delay(200); // Simple debounce
    stopDemoAlarm();
  }
}

void setDemoAlarm() {
  dynamicAlarmHour = hour;
  dynamicAlarmMinute = minute + 3; // 3 minutes ahead in demo time
  
  // Handle minute overflow
  if (dynamicAlarmMinute >= 60) {
    dynamicAlarmMinute -= 60;
    dynamicAlarmHour++;
    if (dynamicAlarmHour >= 24) {
      dynamicAlarmHour = 0;
    }
  }
  
  dynamicAlarmActive = true;
  showDemoMessage("Demo: " + formatDigits(dynamicAlarmHour) + ":" + formatDigits(dynamicAlarmMinute));
  Serial.println("Demo alarm set for: " + formatDigits(dynamicAlarmHour) + ":" + formatDigits(dynamicAlarmMinute));
}

void stopDemoAlarm() {
  if (buzzerActive) {
    stopBuzzer();
    showDemoMessage("Alarm Stopped");
    Serial.println("Demo alarm stopped");
  } else if (dynamicAlarmActive) {
    dynamicAlarmActive = false;
    dynamicAlarmHour = -1;
    dynamicAlarmMinute = -1;
    showDemoMessage("Alarm Cancelled");
    Serial.println("Demo alarm cancelled");
  } else {
    showDemoMessage("No Alarm Active");
  }
}

void stopBuzzer() {
  digitalWrite(buzzerPin, LOW);
  buzzerActive = false;
  alarmTriggeredThisMinute = false;
}

void showDemoMessage(String message) {
  demoMessage = message;
  messageStartTime = millis();
  updateDemoDisplay();
}

void updateDemoTime() {
  minute++;
  alarmTriggeredThisMinute = false; // Reset each minute
  
  if (minute >= 60) {
    minute = 0;
    hour++;
    if (hour >= 24) {
      hour = 0;
    }
  }
  
  // Show time progression in serial
  Serial.println("Demo time: " + formatDigits(hour) + ":" + formatDigits(minute));
}

void checkDemoAlarms() {
  if (alarmTriggeredThisMinute) {
    return;
  }
  
  bool alarmTriggered = false;
  
  // Check demo static alarms
  for (int i = 0; i < NUM_DEMO_ALARMS; i++) {
    if (hour == demoAlarmHours[i] && minute == demoAlarmMinutes[i]) {
      triggerDemoAlarm("Static Demo");
      alarmTriggered = true;
      break;
    }
  }
  
  // Check dynamic alarm
  if (!alarmTriggered && dynamicAlarmActive && 
      hour == dynamicAlarmHour && minute == dynamicAlarmMinute) {
    triggerDemoAlarm("Dynamic Demo");
    dynamicAlarmActive = false;
    dynamicAlarmHour = -1;
    dynamicAlarmMinute = -1;
  }
}

void triggerDemoAlarm(String alarmType) {
  buzzerActive = true;
  buzzerStartMillis = millis();
  alarmTriggeredThisMinute = true;
  digitalWrite(buzzerPin, HIGH);
  showDemoMessage(alarmType + " RING!");
  Serial.println(alarmType + " alarm triggered at " + formatDigits(hour) + ":" + formatDigits(minute));
}

void updateDemoDisplay() {
  String currentTime = formatDigits(hour) + ":" + formatDigits(minute);
  
  if (demoMessage != "" || currentTime != lastDisplayedTime) {
    lcd.clear();
    
    // Row 1: Message or demo info
    if (demoMessage != "") {
      lcd.setCursor(0, 0);
      lcd.print(demoMessage);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Demo Time");
      if (dynamicAlarmActive) {
        lcd.setCursor(12, 0);
        lcd.print("A"); // Alarm indicator
      }
    }
    
    // Row 2: Current time (without seconds in demo)
    lcd.setCursor(4, 1);
    lcd.print(currentTime);
    lcd.print(":xx");
    
    lastDisplayedTime = currentTime;
  }
}

String formatDigits(int number) {
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}