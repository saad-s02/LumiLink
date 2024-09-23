# LumiLink - ESP32 Friendship Lamp

**LumiLink** is a custom friendship lamp project that allows two ESP32 boards to communicate with each other over the internet using Firebase Realtime Database. When a button is pressed on one lamp, the LED on the other lamp lights up, no matter where in the world it is, as long as both lamps are connected to WiFi. This project is designed to keep loved ones connected through light, even at a distance.

## Features

- Real-time communication between two ESP32 boards using Firebase.
- Built-in LED toggles state on one lamp and synchronizes with the other lamp.
- Dynamic WiFi configuration using WiFiManager.
- Uses interrupts to detect button presses.
- Simple and reliable communication through Firebase Realtime Database.

## Hardware Requirements

- 2 x ESP32 Development Boards
- 2 x Push buttons
- Jumper wires
- Breadboard (optional for prototyping)

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Firebase ESP32 Client Library](https://github.com/mobizt/Firebase-ESP32)
- [WiFiManager Library](https://github.com/tzapu/WiFiManager)
- [FastLED Library](https://github.com/FastLED/FastLED)

## Setup Instructions

### 1. Install the Required Libraries

Ensure you have the following libraries installed in the Arduino IDE:

- **Firebase ESP32 Client**: Install via Library Manager or download from [Firebase ESP32 Client Library](https://github.com/mobizt/Firebase-ESP32).
- **WiFiManager**: Install via Library Manager or download from [WiFiManager](https://github.com/tzapu/WiFiManager).
- **FastLED**: Install via Library Manager or download from [FastLED](https://github.com/FastLED/FastLED).

### 2. Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/) and create a new project.
2. Enable Realtime Database and set up your Firebase credentials.
3. Add a new path to the database (e.g., `/LumiLink/LED_status`) to store the LED status.
4. Update the `FIREBASE_HOST` in the code with your project-specific values from Firebase.

### 3. Flashing the Code to ESP32

1. Open the `LumiLink` sketch in the Arduino IDE.
2. Connect your ESP32 board to your computer.
3. Select the correct board and port in **Tools > Board** and **Tools > Port**.
4. Flash the code to each ESP32 device.

### 4. Connecting to WiFi

After uploading the code, the ESP32 will start in access point mode (AP mode) the first time it boots. Connect to the WiFi network `LumiLink` using a phone or computer, and configure it with your home WiFi credentials via the captive portal.

### 5. Testing

- Press the button on one ESP32 device, and the built-in LED on the other device should turn on or off, depending on the current state.
- The LED states are synchronized in real-time using Firebase, so changes will reflect across devices.

## Code Structure

```cpp
#include <WiFiManager.h>
#include <FirebaseESP32.h>

// Firebase and WiFi configuration
#define FIREBASE_HOST "your-firebase-url"
#define FIREBASE_AUTH "your-firebase-api-key"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String path = "/LumiLink/LED_status";
bool toggle_pressed = false;
int button1 = 13; // GPIO pin for button
int LED = 2; // GPIO pin for built-in LED

## How It Works

1. **Button Press Detection:** The ESP32 uses an interrupt to detect when the button is pressed.
2. **Firebase Synchronization:** When the button is pressed, the built-in LED toggles its state, and the state is updated in Firebase.
3. **Remote LED Update:** The other ESP32 device reads the updated state from Firebase and toggles its LED accordingly.

## Customization

- You can expand this project by adding more ESP32 devices to the network, allowing multiple lamps to communicate with each other.
- Customize the LED behavior (e.g., add color, brightness control if using RGB LEDs).

## Troubleshooting

- If the ESP32 doesn't connect to WiFi, try resetting the device to trigger the WiFiManager access point mode again.
- Check the Serial Monitor for Firebase-related error messages if the LED state doesn't synchronize correctly.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.