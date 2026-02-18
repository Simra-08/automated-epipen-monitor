#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Servo.h>

#define REPORTING_PERIOD_MS 1000
#define SERVO_PIN D5

PulseOximeter pox;
Servo servo;

uint32_t lastReportTime = 0;
uint32_t lastServoStepTime = 0;

// ================== SETTINGS ==================
int HR_THRESHOLD_HIGH = 50;   // Trigger point
int HR_THRESHOLD_LOW  = 95;    // Reset point (hysteresis)

// SG90 calibrated safe range
int SERVO_MIN = 10;
int SERVO_MAX = 170;

// Incremental motion
int SERVO_STEP = 2;
int SERVO_STEP_DELAY = 15; // ms
// ==============================================

int currentServoPos = SERVO_MIN;

enum ServoState {
  AT_MIN,
  MOVING_UP,
  AT_MAX,
  MOVING_DOWN
};

ServoState servoState = AT_MIN;

void onBeatDetected() {
  Serial.println("♥ Beat!");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing MAX30100...");

  if (!pox.begin()) {
    Serial.println("MAX30100 FAILED");
    while (1);
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  servo.attach(SERVO_PIN, 500, 2400);

  currentServoPos = SERVO_MIN;
  servo.write(currentServoPos);

  Serial.println("System Ready");
}

void loop() {
  pox.update();

  // ---------- Heart rate evaluation ----------
  if (millis() - lastReportTime > REPORTING_PERIOD_MS) {
    float heartRate = pox.getHeartRate();

    Serial.print("Heart Rate: ");
    Serial.println(heartRate);

    // 🔒 LATCHED LOGIC
    if (servoState == AT_MIN && heartRate > HR_THRESHOLD_HIGH) {
      servoState = MOVING_UP;
      Serial.println("⚠️ Triggered → moving UP");
    }

    if (servoState == AT_MAX && heartRate < HR_THRESHOLD_LOW) {
      servoState = MOVING_DOWN;
      Serial.println("✅ Reset → moving DOWN");
    }

    lastReportTime = millis();
  }

  // ---------- Incremental servo motion ----------
  if (millis() - lastServoStepTime > SERVO_STEP_DELAY) {

    if (servoState == MOVING_UP) {
      currentServoPos += SERVO_STEP;
      if (currentServoPos >= SERVO_MAX) {
        currentServoPos = SERVO_MAX;
        servoState = AT_MAX;   // 🔒 lock here
        Serial.println("🔒 Reached MAX");
      }
      servo.write(currentServoPos);
    }

    else if (servoState == MOVING_DOWN) {
      currentServoPos -= SERVO_STEP;
      if (currentServoPos <= SERVO_MIN) {
        currentServoPos = SERVO_MIN;
        servoState = AT_MIN;   // 🔒 lock here
        Serial.println("🔒 Reached MIN");
      }
      servo.write(currentServoPos);
    }

    lastServoStepTime = millis();
  }
}


