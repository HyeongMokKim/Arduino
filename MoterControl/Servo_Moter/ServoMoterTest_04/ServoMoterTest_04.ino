#include <Servo.h>
Servo servo;

int servo_pin = 11;
int min_value = 800;  // 펄프폭 최소값   0 도
int max_value = 2300; // 펄스폭 최대값 180 도
int servo_value;

void setup() {

    // 서보모터 최기화
    servo.attach(servo_pin, min_value, max_value);
    servo.write(90);  // 서보모터 중심찾기
}

void loop() {

    // 서보모터 실행
    servo.write(0);
    delay(300);
    
    // 서보모터 실행
    servo.write(90);
    delay(300);
    
    // 서보모터 실행
    servo.write(180);
    delay(300);
    
    // 서보모터 실행
    servo.write(90);
    delay(300);
}

