#include <Servo.h>
Servo sv;
void setup()
{
  sv.attach(9, 800, 2300);
}

void loop()
{
  for( int i = 0; i < 180; i++ ){
    sv.write(i);
    delayMicroseconds(4166);
  }
  delay(1000);
 for( int i = 2300; i >= 800; i-- ){
    sv.writeMicroseconds(i);
    delay(1);
  }
  delay(1000);
}
