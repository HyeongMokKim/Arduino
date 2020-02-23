#include <Servo.h>
Servo servo;

//int servo_pin = 5;
//int min_value = 244;  // 펄프폭 최소값   0 도
//int max_value = 2400; // 펄스폭 최대값 180 도
int servo_pin = 9;
int min_value = 800;  // 펄프폭 최소값   0 도
int max_value = 2300; // 펄스폭 최대값 180 도
int servo_value;

void setup() {
    Serial.begin(9600);
    Serial.println("Servo Value = ?");

    // 서보모터 최기화
    servo.attach(servo_pin, min_value, max_value);
    servo.write(90);  // 서보모터 중심찾기
}

void loop() {

    if(Serial.available() > 0) {  // 시리얼 통신으로부터 값이 입력되면 실행
      // 입력된 값을 servo_value에 저장
      servo_value = Serial.parseInt();
      
      // 서보모터 실행
      servo.write(servo_value);
      Serial.println(servo_value);
    }
}

