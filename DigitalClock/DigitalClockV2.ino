#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Clock time
int hour = 0, minute = 0, second = 0;

// Static alarms
const int NUM_ALARMS = 9;
int alarmHours[NUM_ALARMS]   = {4, 4, 4, 4, 5, 15, 17, 19, 22};
int alarmMinutes[NUM_ALARMS] = {50, 55, 57, 59, 0, 0, 30, 0, 30};
bool staticAlarmTriggered[NUM_ALARMS] = {false};

// Temporary alarm
int tempAlarmHour = -1, tempAlarmMinute = -1, tempAlarmSecond = -1;
bool tempAlarmSet = false;

// Buzzer
const int buzzerPin = 8;
bool buzzerActive = false;
unsigned long buzzerStartMillis = 0;

// Time update
unsigned long previousSecondMillis = 0;
const unsigned long secondInterval = 1000;
String lastDisplayedTime = "";

// Buttons
const int btnIncMin     = 2;
const int btnDecSec     = 3;
const int btn20MinAlarm = 4;  // D4
const int btnStopAlarm  = 5;  // D5

// D5 logic
unsigned long d5PressStart = 0;
bool d5Holding = false;
bool d5DismissedThisHold = false;

// D4 state
bool d4PrevState = HIGH;

// Message system
String messageText = "";
unsigned long messageStartTime = 0;
unsigned long messageDurationMs = 0;

// Function prototypes
String formatDigits(int n);
void showMessageBottom(String msg, unsigned long duration = 2000);

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  pinMode(btnIncMin,     INPUT_PULLUP);
  pinMode(btnDecSec,     INPUT_PULLUP);
  pinMode(btn20MinAlarm, INPUT_PULLUP);
  pinMode(btnStopAlarm,  INPUT_PULLUP);

  splashScreen();
}

void loop() {
  unsigned long now = millis();

  handleButtons();

  if (now - previousSecondMillis >= secondInterval) {
    previousSecondMillis = now;
    updateTime();
    checkAlarms();
    updateDisplay();
  }

  if (buzzerActive && now - buzzerStartMillis >= 30000) {
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
  }

  // Clear bottom message after timeout
  if (millis() - messageStartTime > messageDurationMs && messageText != "") {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    messageText = "";
  }
}

void handleButtons() {
  unsigned long now = millis();

  // +1 minute
  if (digitalRead(btnIncMin) == LOW) {
    minute = (minute + 1) % 60;
    if (minute == 0) hour = (hour + 1) % 24;
    delay(200);
  }

  // -1 second
  if (digitalRead(btnDecSec) == LOW) {
    if (second > 0) second--;
    else {
      second = 59;
      if (minute > 0) minute--;
      else {
        minute = 59;
        hour = (hour > 0 ? hour - 1 : 23);
      }
    }
    delay(200);
  }

  // D5 (Stop alarm or dismiss temp alarm)
  bool d5Pressed = digitalRead(btnStopAlarm) == LOW;
  if (d5Pressed) {
    if (!d5Holding) {
      d5PressStart = now;
      d5Holding = true;
      d5DismissedThisHold = false;
    }

    unsigned long heldTime = now - d5PressStart;
    if (heldTime >= 10000 && !d5DismissedThisHold) {
      if (tempAlarmSet) {
        tempAlarmSet = false;
        digitalWrite(buzzerPin, LOW);
        buzzerActive = false;
        showMessageBottom(" Alarm Dismissed ");
      }
      d5DismissedThisHold = true;
    }
  } else {
    if (d5Holding) {
      unsigned long heldTime = now - d5PressStart;
      d5Holding = false;
      if (heldTime < 10000) {
        if (buzzerActive) {
          digitalWrite(buzzerPin, LOW);
          buzzerActive = false;
          showMessageBottom(" Alarm Stopped   ");
        }
      }
    }
  }

  // D4 (Set alarm 20 mins from now)
  bool d4CurrState = digitalRead(btn20MinAlarm);
  if (d4PrevState == HIGH && d4CurrState == LOW) {
    // Button just pressed
    tempAlarmHour = hour;
    tempAlarmMinute = minute;
    tempAlarmSecond = second;

    tempAlarmMinute += 20;
    if (tempAlarmMinute >= 60) {
      tempAlarmMinute -= 60;
      tempAlarmHour = (tempAlarmHour + 1) % 24;
    }

    tempAlarmSet = true;

    showMessageBottom("Alarm: " + formatDigits(tempAlarmHour) + ":" +
                                formatDigits(tempAlarmMinute) + ":" +
                                formatDigits(tempAlarmSecond));
  }
  d4PrevState = d4CurrState;
}

void updateTime() {
  second++;
  if (second >= 60) {
    second = 0;
    minute++;
    if (minute >= 60) {
      minute = 0;
      hour = (hour + 1) % 24;
    }
  }
}

void checkAlarms() {
  for (int i = 0; i < NUM_ALARMS; i++) {
    if (hour == alarmHours[i] && minute == alarmMinutes[i] && second == 0 && !staticAlarmTriggered[i]) {
      triggerAlarm();
      staticAlarmTriggered[i] = true;
    } else if (minute != alarmMinutes[i]) {
      staticAlarmTriggered[i] = false;
    }
  }

  if (tempAlarmSet && hour == tempAlarmHour && minute == tempAlarmMinute && second == tempAlarmSecond) {
    triggerAlarm();
    tempAlarmSet = false;
  }
}

void triggerAlarm() {
  digitalWrite(buzzerPin, HIGH);
  buzzerActive = true;
  buzzerStartMillis = millis();
}

void updateDisplay() {
  String timeStr = formatDigits(hour) + ":" + formatDigits(minute) + ":" + formatDigits(second);
  if (timeStr != lastDisplayedTime) {
    lcd.setCursor(4, 0);
    lcd.print(timeStr);  // <-- YOUR FORMAT
    lastDisplayedTime = timeStr;
  }

  if (tempAlarmSet && !buzzerActive && messageText == "") {
    lcd.setCursor(0, 1);
    lcd.print("Next: " + formatDigits(tempAlarmHour) + ":" +
                      formatDigits(tempAlarmMinute) + ":" +
                      formatDigits(tempAlarmSecond) + " ");
  }
}

String formatDigits(int n) {
  return (n < 10 ? "0" : "") + String(n);
}

void showMessageBottom(String msg, unsigned long duration) {
  messageText = msg;
  messageStartTime = millis();
  messageDurationMs = duration;
  lcd.setCursor(0, 1);
  lcd.print(msg + "                ");
}

void splashScreen() {
  lcd.clear();

  // Step 1: Show "Digital Clock" on top row
  lcd.setCursor(0, 0);
  lcd.print("  Digital Clock");
  delay(2000);
  lcd.clear();

  // Step 2: Type out "  Made By Kayef" with flicker on second row
  String flickerText = "  Made By Kayef";
  lcd.setCursor(0, 1); // Second row
  for (int i = 0; i < flickerText.length(); i++) {
    lcd.print(flickerText[i]);

    // Flicker effect: hide and show character
    delay(50);
    lcd.setCursor(i, 1);
    lcd.print(" ");   
    delay(50);
    lcd.setCursor(i, 1);
    lcd.print(flickerText[i]);

    delay(100); // Typing delay
  }

  delay(1000); // Hold splash screen

  // Step 3: Show encouragement message on top row
  lcd.setCursor(0, 0);
  lcd.print(" Start Your Day");
  delay(1000);

  lcd.clear(); // Ready for main loop
}
