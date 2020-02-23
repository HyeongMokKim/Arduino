#include <Servo.h>
Servo servo;

// # Editor     : Lauren from DFRobot
// # Date       : 17.02.2012

// # Product name: L298N motor driver module DF-MD v1.3
// # Product SKU : DRI0002
// # Version     : 1.0

// # Description:
// # The sketch for using the motor driver L298N
// # Run with the PWM mode

// # Connection:
// #        M1 pin  -> Digital pin 6
// #        E1 pin  -> Digital pin 7
// #        M2 pin  -> Digital pin 8
// #        E2 pin  -> Digital pin 9
// #        Motor Power Supply -> Centor blue screw connector(5.08mm 3p connector)
// #        Motor A  ->  Screw terminal close to E1 driver pin
// #        Motor B  ->  Screw terminal close to E2 driver pin
// # 
// # Note: You should connect the GND pin from the DF-MD v1.3 to your MCU controller. They should share the GND pins.
// #

/* ------------------------- */
/* HC-SR04 음파 거리측정기용 */
/* ------------------------- */
const int trigPin    = 2;                      // Trig 핀 할당
const int echoPin    = 4;                      // Echo 핀 할당
double  Distance[3]  = {0.0, 0.0, 0.0};        // 거리

/* ------------------------- */
  /* 서브모터 조작제여용       */
/* ------------------------- */
int servo_pin = 11;
int min_value = 800;                           // 펄프폭 최소값   0 도
int max_value = 2300;                          // 펄스폭 최대값 180 도
int servo_value;

/* ----------------------------- */
/* DC모터DRI2000 드라이버 제어용 */
/* ----------------------------- */
int E1 = 8, E2  = 7;
int M1 = 9, M2  = 6;
int mod;

const int FRONT = 0;
const int REAL  = 1;
const int RIGHT = 2;
const int LEFT  = 3;

void setup()
{
    // 시리얼포트 연결
    Serial.begin(115200);                      // 시리얼 초기화

    // 서보모터 최기화
    servo.attach(servo_pin, min_value, max_value);
    servo.write(90);                           // 서보모터 중심으로이동
    delay(200);
    servo.detach();

    // 거리센서 초기화
    pinMode(trigPin, OUTPUT);                  // Trig 핀 output으로 세팅
    pinMode(echoPin, INPUT);                   // Echo 핀 input으로 세팅
    
    // 모터 드라이버 제어용 초기화
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop() 
{ 
    double dist  = 0.0;
    int spd = 255;

    sorvo_Control(Distance);
    
    for (int idx = 0; idx <= 2; idx++)
    {
        Serial.print("Distance [");
        Serial.print(idx);
        Serial.print("] => ");
        Serial.print(Distance[idx]);
        Serial.print("Cm");
        Serial.println();
    }
    
    //moveControl(mod, spd);
    Serial.print("FRONT MOVE ");
    Serial.println();
    for(int value = 0 ; value < 50; value++) 
    {
        mod = FRONT;
        moveControl(mod, spd);
    }
    
    Serial.print("RIGHT MOVE ");
    Serial.println();
    for(int value = 0 ; value < 50; value++) 
    {
        mod = RIGHT;
        moveControl(mod, spd);
    }
    
    Serial.print("LEFT MOVE ");
    Serial.println();
    for(int value = 0 ; value < 50; value++) 
    {
        mod = LEFT;
        moveControl(mod, spd);
    }
    
    Serial.print("REAL MOVE ");
    Serial.println();
    for(int value = 0 ; value < 50; value++) 
    {
        mod = REAL;
        moveControl(mod, spd);
    }
}

/* --------------------------------------------
   서보모터 컨트롤처리
   --------------------------------------------
   서보모터를 각도별로 회전시켜 거리측정을하고
   측정한 거리를 배역에 저장한다.
   --------------------------------------------
   *pDist   :  측정한 거리를 저장할 배열의 포인터
   -------------------------------------------- */
void sorvo_Control(double *pDist)
{
    servo.attach(servo_pin, min_value, max_value);

    for (int idx = 0; idx <= 2; idx++)
    {
        // 거리츨정센서의 방향이동
        servo.write(idx*90);
        // 각 방향의 거리를 측정하여 배엘에 저장
        *(pDist + idx) = Distance_Cal(); 
        delay(100);
    }

    // 서보모터 실행(정면)
    servo.write(90);
    delay(200);

    servo.detach();

}

/* --------------------------------------------
   음파가 반사된 시간을 거리로 환산
   --------------------------------------------
   음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
   따라서, 음파의 이동거리 = 왕복시간 / 1cm 이동 시간 / 2 이다.
   distance = (duration / 2.9) / 2;           // 밀리미터로 환산
   distance = duration / 74 / 2;              // 인치로 환산
   -------------------------------------------- */
double Distance_Cal()
{
    delay(100);
    double duration, distance;                 // 기본 변수 선언
 
    //Trig 핀으로 10us의 pulse 발생
    digitalWrite(trigPin, LOW);                // Trig 핀 Low
    delayMicroseconds(2);                      // 2us 유지
    digitalWrite(trigPin, HIGH);               // Trig 핀 High
    delayMicroseconds(10);                     // 10us 유지
    digitalWrite(trigPin, LOW);                // Trig 핀 Low
 
    /*Echo 핀으로 들어오는 펄스의 시간 측정    */
    duration = pulseIn(echoPin, HIGH);         // pulseIn함수가 호출되고 펄스가 입력될 때까지의 시간. us단위로 값을 리턴.
    distance = duration / 29 / 2;              // 센치미터로 환산

    return distance;
}

/* --------------------------------------------
   모터동작 제어처리
       전후좌우 이동을 제어한다.
   --------------------------------------------
   입력치      ： 동작모드、속도
   리턴치      ： 없음
   -------------------------------------------- */
void moveControl(int moveMode, int speed)
{
    
    // 距離測定による、モータコントロール
    switch (moveMode)
    {
        case FRONT:
             digitalWrite(M1,HIGH);
             digitalWrite(M2,HIGH);
             break;
        case REAL:
             digitalWrite(M1,LOW);
             digitalWrite(M2,LOW);
             break;
        case RIGHT:
             digitalWrite(M1,LOW);
             digitalWrite(M2,HIGH);
             break;
        case LEFT:
             digitalWrite(M1,HIGH);
             digitalWrite(M2,LOW);
             break;
        default:
             break;
    } 
    analogWrite(E1, speed);   // PWM Speed Control
    analogWrite(E2, speed);   // PWM Speed Control
    delay(30);

}

