const int ledPin = 7; 

void setup() {
 
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  digitalWrite(ledPin, HIGH);
  delay(1000);


  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, LOW);  // LED 끄기
    delay(100);                 // 0.1초 대기
    digitalWrite(ledPin, HIGH); // LED 켜기
    delay(100);                 // 0.1초 대기
  }

  digitalWrite(ledPin, LOW);

  while (1) {
    // 아무 작업도 수행하지 않음
  }
}
