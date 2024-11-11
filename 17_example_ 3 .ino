#include <Servo.h>

// Arduino pin assignment
#define PIN_IR    0         // IR sensor at Pin A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 600       // servo full clockwise position (0 degree)
#define _DUTY_NEU 1500      // servo neutral position (90 degree)
#define _DUTY_MAX 2400      // servo full counter-clockwise position (180 degree)

#define _DIST_MIN  100.0    // minimum distance 100mm
#define _DIST_MAX  250.0    // maximum distance 250mm

#define EMA_ALPHA  0.2      // for EMA Filter
#define LOOP_INTERVAL 20    // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time = 0; // unit: msec

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(250000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9.0)) - 4.0) * 10.0;

  // Range Filter: if in range, turn on LED
  if (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  // Apply EMA Filter
  dist_ema = EMA_ALPHA * dist_raw + (1 - EMA_ALPHA) * dist_prev;
  dist_prev = dist_ema;

  // Mapping without map()
  duty = _DUTY_MIN + ((dist_ema - _DIST_MIN) * (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN));
  
  myservo.writeMicroseconds(duty);
// Serial output for plotting
Serial.print(_DUTY_MIN);
Serial.print(","); Serial.print(_DIST_MIN);
Serial.print(","); Serial.print(a_value);
Serial.print(","); Serial.print(dist_raw);
Serial.print(","); Serial.print(dist_ema);
Serial.print(","); Serial.print(duty);
Serial.print(","); Serial.print(_DIST_MAX);
Serial.print(","); Serial.print(_DUTY_MAX);
Serial.println("");  // 줄바꿈으로 각 데이터 묶음이 한 줄로 출력
}
