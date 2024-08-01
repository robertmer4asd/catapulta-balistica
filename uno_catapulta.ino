#include <Servo.h>
#include <Wire.h>

Servo servo_elastic;
Servo servo_armat;
Servo servo_y;
int currentAngle = 180;
int x = 0; // Variable to store received data

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Attach servos to specified pins
  Serial.begin(9600);
  servo_elastic.attach(9);
  servo_armat.attach(7);
  servo_elastic.write(0);
  servo_armat.write(0);
  delay(1000);
  servo_armat.write(270);

  // Initialize I2C communication as a slave with address 9
  Wire.begin(8); // 9 is the I2C address of the slave device
  Wire.onReceive(receiveEvent); // Register the receiveEvent function
}

void loop() {
  // Print the received value of x to the Serial Monitor
  Serial.println(x);
  if(x == 10)
  {
    servo_elastic.write(200);
    delay(1000);
    servo_armat.write(0);

  }else if(x == 11)
  {
    servo_elastic.write(180);
    delay(1000);  
    servo_armat.write(0);

  }
  else if(x == 12)
  {
    servo_elastic.write(150);
    delay(1000);
    servo_armat.write(0);
  }/*
  else if(x == 13)
  {

    servo_elastic.write(200);
    delay(1000);
    servo_armat.write(0);

  }
  else if(x == 14)
  {
    servo_armat.write(180);
    delay(1000);
    servo_elastic.write(90);
    delay(1000);
    servo_armat.write(0);

  }
  else if(x == 15)
  {
    servo_armat.write(180);
    delay(1000);
    servo_elastic.write(45);
    delay(1000);
    servo_armat.write(0);

  }
  else if(x == 16)
  {
    servo_armat.write(180);
    delay(1000);
    servo_elastic.write(15);
    delay(1000);
    servo_armat.write(0);

  }
  else if(x == 17)
  {
    servo_armat.write(180);
    delay(1000);
    servo_elastic.write(0);
    delay(1000);
    servo_armat.write(0);

  }

  delay(100);
  if(x == 30)
  {
    moveServoSlowly(servo_y, 0, 2);
  }*/
}

void receiveEvent(int bytes) {
  while (Wire.available()) {
    x = Wire.read(); 
  }
}
void moveServoSlowly(Servo &servo, int targetAngle, int stepDelay) {
  if (targetAngle < currentAngle) {
    for (int angle = currentAngle; angle >= targetAngle; angle--) {
      servo.write(angle);
      delay(stepDelay);
    }
  } else if (targetAngle > currentAngle) {
    for (int angle = currentAngle; angle <= targetAngle; angle++) {
      servo.write(angle);
      delay(stepDelay);
    }
  }
  currentAngle = targetAngle;
}
