//libs
#include <AFMotor.h>
#include <HCSR04.h>

//definitions
// IR sensor pins
#define IR1 44
#define IR2 24
// color sensor pins
#define s0 18
#define s1 17
#define s2 15
#define s3 14
#define out 16

//variables
// checks
bool linecode = 0;
bool ultracode = 0;
bool stopcode = 0;
int is_blue = 0;
int is_red = 0;
// speeds
int basespeed = 205;
int decspeed = 50;
// color sensor
int red = 0;
int blue = 0;
int green = 0;
int clear = 0;
int color;
int get1 = -1;
int get2 = -1;
int get3 = -1;
int get4 = -1;
int get5 = -1;
// IR sensor
int IRS1;
int IRS2;
// ultrasound sensor
UltraSonicDistanceSensor distanceSensor1(50, 51);
UltraSonicDistanceSensor distanceSensor2(28, 29);
UltraSonicDistanceSensor distanceSensor3(34, 35);
int leftultra;
int rightultra;
int midultra;
//motor
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

//go forward function
void goforward() {
  motor1.setSpeed(basespeed - decspeed - 20);
  motor2.setSpeed(basespeed - decspeed - 20);
  motor3.setSpeed(basespeed - decspeed - 20);
  motor4.setSpeed(basespeed - decspeed - 20);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

//go forward function for ultrasound
void goforwardultra() {
  motor1.setSpeed(basespeed - 20);
  motor2.setSpeed(basespeed - 20);
  motor3.setSpeed(basespeed - 20);
  motor4.setSpeed(basespeed - 20);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

//turn right function
void turnright() {
  motor1.setSpeed(basespeed + 20);
  motor2.setSpeed(basespeed + 20);
  motor3.setSpeed(basespeed - 40);
  motor4.setSpeed(basespeed - 40);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// turn right ultrasound
void turnrightultra() {
  motor1.setSpeed(basespeed + 50);
  motor2.setSpeed(basespeed + 50);
  motor3.setSpeed(basespeed + 50);
  motor4.setSpeed(basespeed + 50);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

//turn left function
void turnleft() {
  motor1.setSpeed(basespeed - 40);
  motor2.setSpeed(basespeed - 40);
  motor3.setSpeed(basespeed + 20);
  motor4.setSpeed(basespeed + 20);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

// turn left ultrasound
void turnleftultra() {
  motor1.setSpeed(basespeed + 50);
  motor2.setSpeed(basespeed + 50);
  motor3.setSpeed(basespeed + 50);
  motor4.setSpeed(basespeed + 50);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

//stop car function
void stopcar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// go backwards function
void goback() {
  motor1.setSpeed(basespeed - decspeed);
  motor2.setSpeed(basespeed - decspeed);
  motor3.setSpeed(basespeed - decspeed);
  motor4.setSpeed(basespeed - decspeed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// line follow function
void linefollow() {
  IRS1 = digitalRead(IR1);
  IRS2 = digitalRead(IR2);
  if (IRS1 == 0 && IRS2 == 0) {
    goforward();
    delay(30);
  }

  else if (IRS1 == 0 && IRS2 == 1) {
    turnright();
    delay(70);
  }

  else if (IRS1 == 1 && IRS2 == 0) {
    turnleft();
    delay(70);
  }
}

// color funtion
int getcolor() {

  //clear
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
  clear = pulseIn(out, HIGH);

  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  red = pulseIn(out, HIGH);

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  green = pulseIn(out, HIGH);

  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  blue = pulseIn(out, HIGH);

  red = map(red, 80, 30, 0, 255);
  green = map(green, 80, 30, 0, 255);
  blue = map(blue, 80, 30, 0, 255);

  if (red > 100 && green < 100 && blue < 100) {
    return 100;
  } else if (red < 100 && green > 150 && blue < 150) {
  } else if (red < 100 && green < 100 && blue > 70) {
    return 10000;
  } else {
    return 0;
  }
}

//get stable reading
int stablemidultra() {
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += distanceSensor3.measureDistanceCm();
  }
  return total / 5;
}
int stablerightultra() {
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += distanceSensor2.measureDistanceCm();
  }
  return total / 5;
}
int stableleftultra() {
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += distanceSensor1.measureDistanceCm();
  }
  return total / 5;
}

// decider funtion
int checker() {
  do {
    get1 = getcolor();
    delay(10);
    get2 = getcolor();
    delay(10);
    get3 = getcolor();
    delay(10);
    get4 = getcolor();
    delay(10);
    get5 = getcolor();
    delay(10);
  } while (get1 != get2 || get1 != get3 || get1 != get4 || get1 != get5);

  if (get1 == 0) {
    linecode = 1;
    ultracode = 0;
  } else if (get1 == 100) {
    if (is_red < 1) {
      goforward();
      delay(100);
      turnright();
      delay(100);
      goforward();
      delay(100);
    }
    ultracode = 1;
    linecode = 0;
    is_red++;
  } else if (get1 == 10000) {
    linecode = 0;
    ultracode = 0;
    is_blue++;
  }
}

// line center function
// void center() {
//   midultra = stablemidultra();
//   leftultra = stableleftultra();
//   rightultra = stablerightultra();
//   if (midultra > 30) {
//     if (leftultra < 5) {
//       turnright();
//       delay(100);
//     } else if (rightultra < 5) {
//       turnleft();
//       delay(100);
//     }
//   }
// }

//obstacle avoid function
void obsavoid() {
  midultra = stablemidultra();
  Serial.print(" MID  ");
  Serial.print(midultra);
  if (midultra < 17 && midultra > 5) {
    leftultra = stableleftultra();
    rightultra = stablerightultra();
    Serial.print(" LEFT  ");
    Serial.print(leftultra);
    Serial.print(" RIGHT  ");
    Serial.println(rightultra);
    if (leftultra <= 12 && leftultra >= 0) {
      turnright();
      delay(50);
    } else if (rightultra <= 10 && rightultra >= 0) {
      turnleft();
      delay(50);
    }
    leftultra = stableleftultra();
    rightultra = stablerightultra();
    Serial.print(" LEFT  ");
    Serial.print(leftultra);
    Serial.print(" RIGHT  ");
    Serial.println(rightultra);
    if (leftultra - rightultra > 3) {
      turnleftultra();
      delay(400);
    } else if (rightultra - leftultra > 3) {
      turnrightultra();
      delay(400);

    } else {
      goforwardultra();
      delay(30);
    }
  } else if (midultra <= 5 && midultra >= 0) {
    stopcar();
    delay(50);
    goback();
    delay(600);
  } else {
    goforwardultra();
    delay(30);
    leftultra = stableleftultra();
    rightultra = stablerightultra();
    Serial.print(" LEFT  ");
    Serial.print(leftultra);
    Serial.print(" RIGHT  ");
    Serial.println(rightultra);
    if (leftultra <= 12 && leftultra >= 0) {
      turnright();
      delay(50);
    } else if (rightultra <= 10 && rightultra >= 0) {
      turnleft();
      delay(50);
    }
  }
}


//setup
void setup() {
  // put your setup code here, to run once:
  //IR sensor pins setup
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  // color sensor pins setup
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  //motor setup
  motor1.setSpeed(basespeed);
  motor2.setSpeed(basespeed);
  motor3.setSpeed(basespeed);
  motor4.setSpeed(basespeed);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  //serial monitor
  Serial.begin(9600);
}

//main
void loop() {
  // put your main code here, to run repeatedly:
  checker();
  if (linecode) {
    linefollow();
  } else if (ultracode) {
    obsavoid();
  } else if (is_blue <= 1) {
    goforward();
    delay(500);
  } else if (is_blue >= 2) {
    stopcar();
    delay(10000);
  }
}
