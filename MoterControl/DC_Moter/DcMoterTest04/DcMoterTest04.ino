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
const int trigPin  = 2;                 // Trig 핀 할당
const int echoPin  = 3;                 // Echo 핀 할당

// 서보모터용 핀 설정및 최대,최소 펄스폭 설정값
int servo_pin     = 9;                  // 서보모터핀설정
int min_value     = 800;                // 펄프폭 최소값   0 도
int max_value     = 2300;               // 펄스폭 최대값 180 도
int servo_value;

// 수신
double Duration   = 0;                  // 수신간격

// DC모터(DRI2000드라이브)용 제어핀 설정
int E1 = 6, E2    = 5;
int M1 = 7, M2    = 4;
int mod;

// DC모터 전후좌우설정
const int FRONT   = 0;                  // 전진처리
const int REAL    = 1;                  // 후진처리
const int RIGHT   = 2;                  // 우회처리
const int LEFT    = 3;                  // 좌회처리

// 거리측정값 저장 배열(0도일때의거리, 90도일때의 거리, 180도일때의 거리)
double Distance[3] = {0.0, 0.0, 0.0};   //거리

/* --------------------------------------------
   측정된 거리로 어떻게 이동할지를 판단.
   --------------------------------------------
   입력 파라메터  ： 측정거리 배열(0, 90, 180), 
                     배열수
   리턴값         ： 없음
   -------------------------------------------- */
void checkNmove(double *p_distance, int arrCnt)
{
    int spd = 0;
    
    // 전방좌우측의 장애물과의 거리가 100Cm이상일경우에 최고속도로 전진
    if (*p_distance > 100.0 && *(p_distance + 1) > 100.0 && *(p_distance + 2) > 100.0)
    {
        mod = FRONT;
        spd = 255;
        for(int idx = 0; idx < 500; idx++){
            moveControl(mod, spd);
        }
        return;
    }
    
    // 전방좌우측의 장애물과의 거리가 50Cm미만 일경우에 속도를 줄여서 전진
    if (*p_distance < 50.0 && *(p_distance + 1) < 50.0 && *(p_distance + 2) < 50.0)
    {
        mod = FRONT;
        spd = 125;
        for(int idx = 0; idx < 250; idx++){
            moveControl(mod, spd);
        }
        return;
    }
    
    // 전방좌우측의 장애물과의 거리가 25Cm미만 일경우에 속도를 줄여서 전진
    if (*p_distance < 25.0 && *(p_distance + 1) > 25.0 && *(p_distance + 2) > 25.0)
    {
        mod = FRONT;
        spd = 60;
        for(int idx = 0; idx < 250; idx++){
            moveControl(mod, spd);
        }
        return;
    }
    
    // 전방좌우측의 장애물과의 거리가 5Cm미만 일경우에 속도를 줄여서 전진
    if (*p_distance < 5.0 && *(p_distance + 1) < 5.0 && *(p_distance + 2) < 5.0)
    {
        mod = REAL;
        spd = 60;
        for(int idx = 0; idx < 100; idx++){
            moveControl(mod, spd);
        }
        mod = RIGHT;
        spd = 60;
        for(int idx = 0; idx < 50; idx++){
            moveControl(mod, spd);
        }
        return;
    }
    
    // 전방좌우측의 장애물과의 거리가 10Cm미만 일경우에 속도를 줄여서 전진
    if (*p_distance < 10.0 && *(p_distance + 1) < 10.0 && *(p_distance + 2) < 10.0)
    {
        // 우측에장해물이 있을경우 우축으로 이동후 다음동작
        if (*p_distance < 10.0)
        {
            mod = RIGHT;
            spd = 60;
            for(int idx = 0; idx < 100; idx++){
                moveControl(mod, spd);
            }
            return;
        }
        
        // 정면에장해물이 있을경우 뒤로이동후 우측으로 회전
        if (*(p_distance + 1) < 10.0)
        {
            mod = REAL;
            spd = 60;
            for(int idx = 0; idx < 200; idx++){
                moveControl(mod, spd);
            }
            mod = RIGHT;
            spd = 60;
            for(int idx = 0; idx < 100; idx++){
                moveControl(mod, spd);
            }
            return;
        }
        
        // 좌측에장해물이 있을경우 좌측으로 회전후 다음동작
        if (*(p_distance + 2) < 10.0)
        {
            mod = LEFT;
            spd = 60;
            for(int idx = 0; idx < 100; idx++){
                moveControl(mod, spd);
            }
        }
        return;
    }

}

/* --------------------------------------------
   거리측정을 하고, 측정한 값을 리턴 한다.
   --------------------------------------------
   입력 파라메터  ： 이동모드, 속도
   리턴값         ： double형측정거리
   -------------------------------------------- */
double distanceMeasurement()
{
    delay(200);
    double duration, distance;                   // 기본 변수 선언
 
    //Trig 핀으로 10us의 pulse 발생
    digitalWrite(trigPin, LOW);                // Trig 핀 Low
    delayMicroseconds(2);                      // 2us 유지
    digitalWrite(trigPin, HIGH);               // Trig 핀 High
    delayMicroseconds(10);                     // 10us 유지
    digitalWrite(trigPin, LOW);                // Trig 핀 Low
 
    /*Echo 핀으로 들어오는 펄스의 시간 측정    */
    duration = pulseIn(echoPin, HIGH);         // pulseIn함수가 호출되고 펄스가 입력될 때까지의 시간. us단위로 값을 리턴.
 
    /*음파가 반사된 시간을 거리로 환산
      음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
      따라서, 음파의 이동거리 = 왕복시간 / 1cm 이동 시간 / 2 이다. */
      
    distance = duration / 29 / 2;              // 센치미터로 환산
    //distance = (duration / 2.9) / 2;         // 밀리미터로 환산
    //distance =  duration / 74 / 2;           // 인치로 환산
 
    //Serial.print(distance);
    //Serial.print("mm");
    //Serial.println();
 
    return distance;
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
    
    return;

}

void setup()
{
    Serial.begin(115200);               // 시리얼 초기화
    
    // 거리측정센서값 초기설정
    pinMode( echoPin, INPUT );
    pinMode( echoPin, OUTPUT );
    
    // DC모터 드라이버 제어용 초기값 설정
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    
    // Servo모터 최기값 설정
    servo.attach(servo_pin, min_value, max_value);
    servo.write(90);                    // 서보모터 중심설정
}

void loop() 
{ 
    // for (int idx = 0; idx <= 2; idx++)
    // {
    //     // 거리츨정센서의 방향이동
    //     servo.write(idx*90);
    //     delay(100);
    //     // 각 방향의 거리를 측정하여 배엘에 저장
    //     Distance[idx] = distanceMeasurement(); 
    //     
    //     Serial.print("Distance [");
    //     Serial.print(idx);
    //     Serial.print("] => ");
    //     Serial.print(Distance[idx]);
    //     Serial.print("mm");
    //     Serial.println();
    // }
    // // 각방향의 거리측정후 거리측정센서의 최기화
    // servo.write(90);


    // 서보모터 실행(우측)
    servo.write(0);
    Distance[0] = distanceMeasurement();
    Serial.print("Distance[0] => ");
    Serial.print(Distance[0]);
    Serial.print("mm");
    Serial.println();
    delay(50);
    
    // 서보모터 실행(정면)
    servo.write(90);
    Distance[1] = distanceMeasurement();
    Serial.print("Distance[1] => ");
    Serial.print(Distance[1]);
    Serial.print("mm");
    Serial.println();
    delay(50);
    
    // 서보모터 실행(좌측)
    servo.write(180);
    Distance[2] = distanceMeasurement();
    Serial.print("Distance[2] => ");
    Serial.print(Distance[2]);
    Serial.print("mm");
    Serial.println();
    Serial.println();
    delay(50);
    
    servo.write(90);

    // 거리판단및 이동처리
    int arrCnt = sizeof Distance / sizeof Distance[0];      // 배열의 갯수를 산출
    checkNmove(Distance, arrCnt);
}
  
