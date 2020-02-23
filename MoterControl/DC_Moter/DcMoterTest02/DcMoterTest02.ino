// # Editor     : Lauren from DFRobot
// # Date       : 17.02.2012

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

// 距離測定用ピン設定
#define echo      2 // Echo    Pin
#define Trigger   3 // Trigger Pin

double Duration = 0; //受信した間隔
double Distance = 0; //距離

// DCモータ(DRI2000 ドライバ)制御用
//int E1 = 6, E2  = 5;
//int M1 = 7, M2  = 4;
int E1 = 8, E2  = 7;
int M1 = 9, M2  = 6;
int mod;

const int FRONT = 0;
const int REAL  = 1;
const int RIGHT = 2;
const int LEFT  = 3;

void setup()
{
    // 距離センサー
    pinMode( echo, INPUT );
    pinMode( Trigger, OUTPUT );
    
    // モータドライバ制御用初期化
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop() 
{ 
    double dist  = 0.0;
    int spd = 255;
    
    // 速度測定処理
    //dist = distance();

    //if (dist > 50.0)
    //{
    //    mod = FRONT;
    //    spd = 255;
    //}else if (dist > 30.0) {
    //    mod = FRONT;
    //    spd = 125;
    //}else if (dist > 10.0) {
    //    // バック移動
    //    for(int idx = 0; idx < 255; idx = idx + 5){
    //        moveControl(REAL, 125);
    //    }
    //    // 方向を変更
    //    for(int idx = 0; idx < 125; idx = idx + 5){
    //        moveControl(RIGHT, 125);
    //    }
    //    mod = FRONT;
    //    spd = 125;
    //}
    
    //moveControl(mod, spd);
    for(int value = 0 ; value < 100; value++) 
    {
        mod = FRONT;
        moveControl(mod, spd);
    }
    
    for(int value = 0 ; value < 100; value++) 
    {
        mod = RIGHT;
        moveControl(mod, spd);
    }
    
    for(int value = 0 ; value < 100; value++) 
    {
        mod = LEFT;
        moveControl(mod, spd);
    }
    
    for(int value = 0 ; value < 100; value++) 
    {
        mod = REAL;
        moveControl(mod, spd);
    }
}

/* --------------------------------------------
   距離測定を行い、測定した値をリターンする
   --------------------------------------------
   入力値      ： 移動モード、速度
   リターン値  ： double型 距離
   -------------------------------------------- */
double distance()
{
    double rtndbl = 0.0;

    // 距離測定を行う為の処理
    digitalWrite(Trigger, LOW); 
    delayMicroseconds(2); 
    
    digitalWrite(Trigger, HIGH);                    // 超音波を出力
    delayMicroseconds(10);                          // 
    
    digitalWrite(Trigger, LOW);
    
    Duration = pulseIn(echo, HIGH);                 // センサからの入力
    
    if (Duration > 0)
    {
        Duration = Duration / 2;                    // 往復距離を半分にする
        rtndbl   = Duration * 340 * 100 / 1000000;  // 音速を340m/sに設定
    }

    return rtndbl;
}

/* --------------------------------------------
   モータ動作制御処理。
       前後左右移動を制御する。
   --------------------------------------------
   入力値      ： 移動モード、速度
   リターン値  ： 無し
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

