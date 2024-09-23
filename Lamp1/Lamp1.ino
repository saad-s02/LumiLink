#include <ESP32Firebase.h>
#include <Bounce2.h>
#include <FastLED.h>
#include <WiFiManager.h>

#define FIREBASE_HOST "Firebase Host ID"
Firebase firebase(FIREBASE_HOST);

int button1 = 13;  // Connect push button on this pin

unsigned long previousMillis = 0;  // Stores the last time Firebase was checked
const long interval = 5000;        // Interval at which to check Firebase (5 seconds)

Bounce debouncer = Bounce();

#define LED_PIN     2       // The pin connected to the data line of the WS2812B
#define NUM_LEDS    7      // Number of LEDs in your strip
#define BRIGHTNESS  64      // Set the brightness of the LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Setup");

  WiFiManager wifiManager;

  pinMode(button1, INPUT_PULLUP);  // Set pin 13 as INPUT with pull-up resistor

  debouncer.attach(button1);  // Attach the button pin
  debouncer.interval(50);     // Set the debounce interval to 50ms

  if (!wifiManager.autoConnect("LumiLink")) {  //Tries to auto connect to known wifi, will creat an AP if no known wifis
    Serial.println("Failed to connect to WiFi and hit timeout.");
    ESP.restart();
  }

  Serial.println("WiFi Connected");

  // Initialize the LED strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // Clear the strip
  FastLED.show();
}

void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking check for Firebase every 5 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    checkStatus();
  }

  debouncer.update();

  if (debouncer.fell()) {  // If the button is pressed  
    Serial.println("Button Pressed!");  // Print a message

    // Update Firebase with error handling
    if (firebase.setInt("Lamp 2 Field ID", 1)) {
      Serial.println("Lamp 2 set to 1 in Firebase.");
    } else {
      Serial.println("Failed to set Lamp 2 in Firebase.");
    }
  }
}

void checkStatus() {
  int led_status = firebase.getInt("Lamp 1 Field ID");

  if (led_status == 1) {
    Serial.println("ON");

    CRGB chosenColor;
    if (random(0, 2) == 0) {  // 50% chance for each color
      chosenColor = CRGB(39, 87, 184);  // First color (light blue)
    } else {
      chosenColor = CRGB(34, 139, 34);  // Second color (greenish)
    }

    fadeInEffect(chosenColor, 1000);
    firebase.setInt("Lamp 1", 0);  // Reset Lamp 1 in Firebase
  } else {
    Serial.println("OFF");
    fadeOutEffect(1000);  // Fade out the LEDs over 1 second
  }
}

void fadeInEffect(CRGB color, int duration) {
  for (int brightness = 0; brightness <= BRIGHTNESS; brightness++) {
    FastLED.setBrightness(brightness);
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    delay(duration / BRIGHTNESS);  // Adjust delay to control the speed
  }
  FastLED.setBrightness(BRIGHTNESS);  // Ensure full brightness at the end
}

void fadeOutEffect(int duration) {
  for (int brightness = BRIGHTNESS; brightness >= 0; brightness--) {
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(duration / BRIGHTNESS);  // Adjust delay to control the speed of the fade-out
  }
  FastLED.clear();  // Ensure all LEDs are turned off at the end
  FastLED.show();
}
