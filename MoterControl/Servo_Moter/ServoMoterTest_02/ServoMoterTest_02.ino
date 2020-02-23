#include <Servo.h>
Servo myservo;
int pos = 0;

void setup() {
    myservo.attach(9);
}

void loop() {

     myservo.write(15);
     delay(1000);
     myservo.write(165);
     delay(1000);
     myservo.write(15);
     delay(2000);

    for(pos = 15; pos <= 165; pos++){
      myservo.write(pos);
      delay(10);
    }
     delay(3000);

    for(pos = 165; pos>= 15; pos--){
        myservo.write(pos);
        delay(10);
    }
     delay(3000);

    for(pos = 15; pos <= 165; pos+=30){
      myservo.write(pos);
      delay(500);
    }
     delay(3000);

    for(pos = 165; pos>= 15; pos-=30){
        myservo.write(pos);
        delay(500);
    }
     delay(3000);

}

