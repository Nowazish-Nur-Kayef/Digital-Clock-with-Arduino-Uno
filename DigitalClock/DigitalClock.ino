#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C address is 0x27 and size is 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Time variables (start time: 08:00:00)
int hour = 15;
int minute = 1;
int second = 20;

// Alarm times: define HH and MM (you can add more)
const int NUM_ALARMS = 9;
int alarmHours[NUM_ALARMS] = {4, 4, 4, 4, 5, 15, 17, 19, 22};
int alarmMinutes[NUM_ALARMS] = {50, 55, 57, 59, 0, 0, 30, 30};

// Buzzer setup
const int buzzerPin = 8;
bool buzzerActive = false;
unsigned long buzzerStartMillis = 0;

// Timing control
unsigned long previousSecondMillis = 0;
const unsigned long secondInterval = 1000;

String lastDisplayedTime = "";

void setup() {
  // Initialize LCD and buzzer
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Step 1: Show "Digital Clock" on top row
  lcd.setCursor(0, 0);
  lcd.print("  Digital Clock");
  delay(2000);
  lcd.clear();

  // Step 2: Type out "Made By Kayef" (or anything you want) letter by letter with flicker
  String flickerText = "  Made By Kayef";
  lcd.setCursor(0, 1); // Second row
  for (int i = 0; i < flickerText.length(); i++) {
    lcd.print(flickerText[i]);

    // Flicker effect: briefly hide/show character
    delay(50);
    lcd.setCursor(i, 1);
    lcd.print(" ");   // Hide
    delay(50);
    lcd.setCursor(i, 1);
    lcd.print(flickerText[i]); // Show again

    delay(100); // Typing delay
  }

  delay(1000); // Hold final splash
  lcd.setCursor(0, 0);
  lcd.print(" Start Your Day");
  delay(1000);

  lcd.clear(); // Ready for main loop
}


void loop() {
  unsigned long currentMillis = millis();

  // Update clock every second
  if (currentMillis - previousSecondMillis >= secondInterval) {
    previousSecondMillis = currentMillis;

    updateTime();
    checkAlarms();
    updateDisplay();
  }

  // Turn off buzzer after 60 seconds
  if (buzzerActive && (currentMillis - buzzerStartMillis >= 30000)) {
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
  }
}

// Increment fake clock time
void updateTime() {
  second++;
  if (second >= 60) {
    second = 0;
    minute++;
    if (minute >= 60) {
      minute = 0;
      hour++;
      if (hour >= 24) {
        hour = 0;
      }
    }
  }
}

// Check if current time matches any alarm time
void checkAlarms() {
  for (int i = 0; i < NUM_ALARMS; i++) {
    if (hour == alarmHours[i] && minute == alarmMinutes[i] && second == 0) {
      buzzerActive = true;
      buzzerStartMillis = millis();
      digitalWrite(buzzerPin, HIGH);
    }
  }
}

// Update LCD only if time changed
void updateDisplay() {
  String currentTime = formatDigits(hour) + ":" + formatDigits(minute) + ":" + formatDigits(second);

  if (currentTime != lastDisplayedTime) {
    lcd.setCursor(6, 0);
    lcd.print("Time"); // clear residue
    lcd.setCursor(4, 1);
    lcd.print("" + currentTime + "");
    lastDisplayedTime = currentTime;
    lcd.backlight();
  }
}


// Format 1-digit numbers to 2-digit (e.g., 8 -> 08)
String formatDigits(int number) {
  if (number < 10) return "0" + String(number);
  return String(number);
}
