#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>

// DHT11 setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RGB LED pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11;

// Active buzzer pin
int buzzerPin = 6;

// Passive buzzer pin
int passiveBuzzerPin = 7;

// Variables for active buzzer timing
unsigned long previousMillis = 0;
int buzzerInterval = 0;  // interval in ms

// === Emergency Melody for Passive Buzzer ===
int melody[] = { 1000, 1200, 1000, 1200, 800, 0 };  // frequencies (Hz)
int noteDurations[] = { 200, 200, 200, 200, 400, 200 }; // ms
int melodyLength = 6;

void playEmergencyMelody() {
  for (int i = 0; i < melodyLength; i++) {
    if (melody[i] == 0) {
      noTone(passiveBuzzerPin); // rest
    } else {
      tone(passiveBuzzerPin, melody[i], noteDurations[i]);
    }
    delay(noteDurations[i] + 50); // gap between notes
  }
  noTone(passiveBuzzerPin);
}

void setup() {
  Serial.begin(9600);

  // Initialize DHT
  dht.begin();

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Pin modes
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(passiveBuzzerPin, OUTPUT);

  digitalWrite(buzzerPin, LOW); // active buzzer OFF initially
}

// Set RGB color
void setColor(int R, int G, int B) {
  analogWrite(redPin, R);
  analogWrite(greenPin, G);
  analogWrite(bluePin, B);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // === OLED Display ===
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 0);
  display.println("TEMP/HUM");

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print("* Temp = ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 40);
  display.print("* Hum  = ");
  display.print(hum);
  display.println(" %");

  unsigned long currentMillis = millis();

  if (temp < 30) {
    // Normal
    setColor(0, 0, 255); // Blue
    digitalWrite(buzzerPin, LOW);
    buzzerInterval = 0;

  } else if (temp <= 35) {
    // Warning
    setColor(0, 255, 0); // Green
    buzzerInterval = 10000; // one beep every 10 sec

  } else {
    // Danger
    setColor(255, 0, 0); // Red
    buzzerInterval = 1000; // one beep every 1 sec

    // Extra OLED alert
    display.setCursor(0, 55);
    display.setTextSize(1);
    display.println("TEMP RISING ALERT!!");

    // Play emergency melody on passive buzzer
    playEmergencyMelody();
  }

  display.display();

  // === Active Buzzer Handling ===
  if (buzzerInterval > 0) {
    if (currentMillis - previousMillis >= buzzerInterval) {
      previousMillis = currentMillis;
      
      // Short beep (200 ms) on active buzzer
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
    }
  }

  delay(200);
}
