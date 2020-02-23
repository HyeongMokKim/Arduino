// # Editor     : Lauren from DFRobot
// # Date       : 12018. 03. 11

// # Product name: L298N motor driver module DF-MD v1.3
// # Product SKU : DRI0002
// # Version     : 1.0

// # Description:
// # The sketch for using the motor driver L298N
// # Run with the PWM mode

// # Connection:
// #        M1 pin  -> Digital pin 4
// #        E1 pin  -> Digital pin 5
// #        M2 pin  -> Digital pin 7
// #        E2 pin  -> Digital pin 6
// #        Motor Power Supply -> Centor blue screw connector(5.08mm 3p connector)
// #        Motor A  ->  Screw terminal close to E1 driver pin
// #        Motor B  ->  Screw terminal close to E2 driver pin
// # 
// # Note: You should connect the GND pin from the DF-MD v1.3 to your MCU controller. They should share the GND pins.
// #

#include <Servo.h>
Servo servo;

// 거리측정요 핀 설정값
#define echo      2 // Echo    Pin
#define Trigger   3 // Trigger Pin

// 서보모터용 핀 설정및 최대,최소 펄스폭 설정값
int servo_pin = 9;
int min_value = 800;  // 펄프폭 최소값   0 도
int max_value = 2300; // 펄스폭 최대값 180 도
int servo_value;

// 수신
double Duration = 0; //수신간격

// DC모터(DRI2000드라이브)용 제어핀 설정
int E1 = 6, E2  = 5;
int M1 = 7, M2  = 4;
int mod;

// DC모터 전후좌우설정
const int FRONT = 0;
const int REAL  = 1;
const int RIGHT = 2;
const int LEFT  = 3;

// 거리측정값 저장 배열(0도일때의거리, 90도일때의 거리, 180도일때의 거리)
double Distance[3] = {0.0, 0.0, 0.0};   //거리

void setup()
{
    // 거리측정센서값 초기설정
    pinMode( echo, INPUT );
    pinMode( Trigger, OUTPUT );
    
    // DC모터 드라이버 제어용 초기값 설정
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    
    // Servo모터 최기값 설정
    servo.write(90);  // 서보모터 중심설정
}

void loop() 
{ 
    double dist  = 0.0;
    int spd = 255;

    for (int idx = 0; idx <= 2; idx++)
    {
        // 거리츨정센서의 방향이동
        servo.write(idx*90);
        delay(100);
        // 각 방향의 거리를 측정하여 배엘에 저장
        Distance[idx] = distance(); 
    }
    // 각방향의 거리측정후 거리측정센서의 최기화
    servo.write(90);

    if (Distance[0] > 50.0 && Distance[1] > 50.0 && Distance[2] > 50.0)
    {
        mod = FRONT;
        spd = 255;
        for(int idx = 0; idx < 1000; idx++){
            moveControl(mod, spd);
        }
    }


/* --------------------------------------------
   거리측정을 하고, 측정한 값을 리턴 한다.
   --------------------------------------------
   입력 파라메터  ： 이동모드, 속도
   리턴값         ： double형측정거리
   -------------------------------------------- */
double distance()
{
    double rtndbl = 0.0;

    // 거리측정을 하기위한 처리
    digitalWrite(Trigger, LOW); 
    delayMicroseconds(2); 
    
    digitalWrite(Trigger, HIGH);                    // 초음파를 출력
    delayMicroseconds(10);                          // 
    
    digitalWrite(Trigger, LOW);
    
    Duration = pulseIn(echo, HIGH);                 // 센서로부터의 입력
    
    if (Duration > 0)
    {
        Duration = Duration / 2;                    // 왕복거리를 반으로 나누어서 계산往復距離を半分にする
        rtndbl   = Duration * 340 * 100 / 1000000;  // 초속을 계산해서 거리산출(340m/s)
    }

    return rtndbl;
}

/* --------------------------------------------
   모터 제어 처리.
       전후좌우이동르 제어 한다.
   --------------------------------------------
   입력 파라메터    ： 이동모드, 속도
   리턴값           ： 없음
   -------------------------------------------- */
void moveControl(int moveMode, int speed)
{
    
    //  거리측정에의한 모터 컨트롤
    switch (moveMode)
    {
        case FRONT:
             // 전진이동
             digitalWrite(M1,HIGH);
             digitalWrite(M2,HIGH);
             break;
        case REAL:
             // 후진이동
             digitalWrite(M1,LOW);
             digitalWrite(M2,LOW);
             break;
        case RIGHT:
             // 우측으로 이동
             digitalWrite(M1,LOW);
             digitalWrite(M2,HIGH);
             break;
        case LEFT:
             // 좌측으로 이동
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

