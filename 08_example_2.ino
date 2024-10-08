// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficient to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;

  // wait until next sampling time
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  // measure the distance
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // LED brightness control based on distance
  if (distance >= _DIST_MIN && distance <= _DIST_MAX) {
    int brightness = calculateLEDBrightness(distance);
    analogWrite(PIN_LED, brightness);
  } else {
    // If out of range, turn off LED (activelow)
    analogWrite(PIN_LED, 255); // LED OFF
  }

  // Output distance to the serial monitor
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(", distance:"); Serial.print(distance);
  Serial.print(", Max:");      Serial.print(_DIST_MAX);
  Serial.println("");
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// Function to calculate LED brightness based on distance
int calculateLEDBrightness(float distance) {
  if (distance <= 150) {
    // If distance is closer than 150mm, adjust brightness linearly between 100mm (off) and 200mm (max brightness)
    return map(distance, 100, 200, 255, 0);  // 255 is off, 0 is max brightness
  } else if (distance <= 250) {
    // Between 150mm and 250mm, set brightness to 50%
    return 127; // 50% brightness
  } else {
    // From 250mm to 300mm, adjust brightness linearly
    return map(distance, 250, 300, 0, 255); // 0 is max brightness, 255 is off
  }
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
