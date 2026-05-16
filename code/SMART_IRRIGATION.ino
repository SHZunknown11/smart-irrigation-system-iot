#define BLYNK_TEMPLATE_ID "PASTE_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "PASTE_YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "PASTE_YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi Credentials
char ssid[] = "PASTE_YOUR_WIFI_NAME";
char pass[] = "PASTE_YOUR_WIFI_PASSWORD";

// Pin Definitions
#define soil_moisture_pin A0
#define relayPin D1

BlynkTimer timer;

// 0 = Auto Mode
// 1 = Manual Pump ON
int manualControl = 0;

bool pumpRunning = false;
bool waitingPhase = false;
bool lastPumpState = false;

unsigned long pumpStartTime = 0;
unsigned long waitStartTime = 0;

// Moisture threshold
const int DRY_THRESHOLD = 800;

// Timing settings
const unsigned long PUMP_DURATION = 3000;    // 3 seconds
const unsigned long WAIT_DURATION = 40000;   // 40 seconds

// ==============================
// Pump Control Function
// ==============================
void setPump(bool on) {

  if (on != lastPumpState) {

    // Relay is Active LOW
    digitalWrite(relayPin, on ? LOW : HIGH);

    lastPumpState = on;

    Serial.print("Pump State: ");
    Serial.println(on ? "ON 🚿" : "OFF 🛑");
  }
}

// ==============================
// Blynk Manual Control (V2)
// ==============================
BLYNK_WRITE(V2) {

  manualControl = param.asInt();

  Serial.print("Manual Control: ");
  Serial.println(manualControl ? "ON" : "OFF");

  if (manualControl == 1) {

    pumpRunning = false;
    waitingPhase = false;

    setPump(true);

  } else {

    setPump(false);
  }
}

// ==============================
// Main Logic Function
// ==============================
void sendData() {

  int moisture = analogRead(soil_moisture_pin);

  // Serial Monitor Output
  Serial.print("Moisture Value: ");
  Serial.print(moisture);
  Serial.print(" | ");

  if (moisture > DRY_THRESHOLD) {

    Serial.println("💀 DRY soil 😭🌱");

  } else if (moisture > 400) {

    Serial.println("😌 Moist soil 🌿✨");

  } else {

    Serial.println("💦 Wet soil 🌧️");
  }

  // Send moisture data to Blynk
  Blynk.virtualWrite(V0, moisture);
  Blynk.virtualWrite(V1, moisture);

  // ==========================
  // Manual Mode
  // ==========================
  if (manualControl == 1) {

    pumpRunning = false;
    waitingPhase = false;

    setPump(true);

    return;
  }

  // ==========================
  // Smart Auto Irrigation Logic
  // ==========================
  unsigned long currentTime = millis();

  // Pump currently running
  if (pumpRunning) {

    if (currentTime - pumpStartTime >= PUMP_DURATION) {

      setPump(false);

      pumpRunning = false;
      waitingPhase = true;

      waitStartTime = currentTime;

      Serial.println("⏳ Pump stopped, waiting before recheck...");
    }
  }

  // Waiting phase
  else if (waitingPhase) {

    if (currentTime - waitStartTime >= WAIT_DURATION) {

      waitingPhase = false;

      Serial.println("🔄 Rechecking soil moisture...");
    }
  }

  // Normal monitoring
  else {

    if (moisture > DRY_THRESHOLD) {

      setPump(true);

      pumpRunning = true;

      pumpStartTime = currentTime;

      Serial.println("🚿 Pump started for 3 seconds");

    } else {

      setPump(false);
    }
  }
}

// ==============================
// Setup Function
// ==============================
void setup() {

  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);

  // Pump OFF initially
  digitalWrite(relayPin, HIGH);

  Serial.print("Connecting to WiFi and Blynk...");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Sync manual button state
  Blynk.syncVirtual(V2);

  Serial.println("\nWiFi Connected ✅");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Blynk Connected ✅");

  // Run sendData every second
  timer.setInterval(1000L, sendData);
}

// ==============================
// Main Loop
// ==============================
void loop() {

  Blynk.run();
  timer.run();
}