#include <Servo.h>

Servo myservo;
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;

long duration;
float distance;
float dist_ema = 0; // EMA 값을 저장할 변수
const float alpha = 0.1; // EMA 필터를 위한 alpha 값 (실험을 통해 조정 가능)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  myservo.attach(11); // 서보 핀 설정
}

void loop() {
  // 초음파 센서로 거리 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // EMA 필터 적용
  dist_ema = alpha * distance + (1 - alpha) * dist_ema;

  // 서보 각도 제어
  if (dist_ema < 18) {
    myservo.write(0); // 거리 18cm 이하: 0도
  } else if (dist_ema > 36) {
    myservo.write(180); // 거리 36cm 이상: 180도
  } else {
    int angle = map(dist_ema, 18, 36, 0, 180);
    myservo.write(angle); // 거리 18cm~36cm: 각도 비례 제어
  }

  // 범위 필터에 따른 LED 제어
  if (dist_ema >= 18 && dist_ema <= 36) {
    digitalWrite(ledPin, HIGH); // 범위 내에 있을 때 LED 점등
  } else {
    digitalWrite(ledPin, LOW);
  }

  // 시리얼 모니터에 출력
  Serial.print("dist_raw:"); Serial.print(distance);
  Serial.print(", ema:"); Serial.print(dist_ema);
  Serial.print(", Servo:"); Serial.print(myservo.read());
  Serial.println();
  
  delay(100);
}
