#include <Motor.h>
#include <Sonar.h>
#include <Buzzer.h>
#include <Wire.h>

/*********************************** Sonar ************************************/
#define sonarA_ECHO     53
#define sonarA_TRIG     52
#define sonarB_ECHO     51
#define sonarB_TRIG     50
#define sonarC_ECHO     49
#define sonarC_TRIG     48
#define sonarD_ECHO     47
#define sonarD_TRIG     46
#define sonarE_ECHO     45
#define sonarE_TRIG     44
#define sonarA_NTFY     36
#define sonarB_NTFY     37
#define sonarC_NTFY     38
#define sonarD_NTFY     39
#define sonarE_NTFY     40
#define MAX_DISTANCE    3000
#define SOUND_FACTOR    59
const int DANGER_ZONE = 10;
/*********************************** Motor ************************************/
#define L_Motor_A       30
#define L_Motor_B       31
#define R_Motor_A       33
#define R_Motor_B       32
#define L_Motor_EN      8
#define R_Motor_EN      9
#define L_Motor_Speed   150
#define R_Motor_Speed   150
/********************************** Buzzer ************************************/
#define Buzzer_Pin      10
/******************************************************************************/
/********************************** Sonars ************************************/
/******************************************************************************/
Sonar sonarA(sonarA_ECHO, sonarA_TRIG, MAX_DISTANCE, SOUND_FACTOR, sonarA_NTFY);
Sonar sonarB(sonarB_ECHO, sonarB_TRIG, MAX_DISTANCE, SOUND_FACTOR, sonarB_NTFY);
Sonar sonarC(sonarC_ECHO, sonarC_TRIG, MAX_DISTANCE, SOUND_FACTOR, sonarC_NTFY);
Sonar sonarD(sonarD_ECHO, sonarD_TRIG, MAX_DISTANCE, SOUND_FACTOR, sonarD_NTFY);
Sonar sonarE(sonarE_ECHO, sonarE_TRIG, MAX_DISTANCE, SOUND_FACTOR, sonarE_NTFY);
/******************************************************************************/
/********************************** Motors ************************************/
/******************************************************************************/
Motor leftMotor(L_Motor_A, L_Motor_B, L_Motor_EN, L_Motor_Speed);
Motor rightMotor(R_Motor_A, R_Motor_B, R_Motor_EN, R_Motor_Speed);
/******************************************************************************/
/********************************** Buzzer ************************************/
/******************************************************************************/
Buzzer buzzer(Buzzer_Pin);
/******************************************************************************/
/********************************** Buzzer ************************************/
/******************************************************************************/
const int MPU = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
/******************************************************************************/
/********************************* Templates **********************************/
/******************************************************************************/
void scanArea();
void robotMove();
void avoidObstacles();
void turnAround(bool RIGHT);
void configureMPU();
void readMPU();

/******************************************************************************/
/********************************* Variables **********************************/
/******************************************************************************/
short surrounding[5] = {0, 0, 0, 0, 0};
int leftMotorSpeed = 0, rightMotorSpeed = 0;

void setup() {
  delay(100);
  buzzer.startTone();
  delay(200);
  buzzer.alertTone();
  delay(200);
  configureMPU();
  Serial.begin(9600);
}

void loop() {
  // Scan the surroundings
  /*
  long a = sonarA.distance();
  Serial.print("A is - ");
  Serial.print(a);
  long b = sonarB.distance();
  Serial.print("  B is - ");
  Serial.print(b);
  long c = sonarC.distance();
  Serial.print("  C is - ");
  Serial.print(c);
  long d = sonarD.distance();
  Serial.print("  D is - ");
  Serial.print(d);
  long e = sonarE.distance();
  Serial.print("  E is - ");
  Serial.println(e);
  delay(100);*/
  
  //scanArea();
  // Avoid any obstacles
  //avoidObstacles();
  // Move straight
  leftMotor.speedUp(100);
  rightMotor.speedUp(100);
  readMPU();  
  Serial.print("Tmp = "); Serial.print(Tmp);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(100);
  //robotMove();
}

/*
  An array of 1's and 0's will be created according to position
*/
void scanArea() {
  surrounding[0] = sonarA.near(DANGER_ZONE - 5) ? 1 : 0;
  surrounding[1] = sonarB.near(DANGER_ZONE - 1) ? 1 : 0;
  surrounding[2] = sonarC.near(DANGER_ZONE) ? 1 : 0;
  surrounding[3] = sonarD.near(DANGER_ZONE - 1) ? 1 : 0;
  surrounding[4] = sonarE.near(DANGER_ZONE - 5) ? 1 : 0;
}

/*
  Move robot with the calculated speed with a delay of 2 ms
*/
void robotMove() {
  leftMotor.speedUp(leftMotorSpeed);
  rightMotor.speedUp(rightMotorSpeed);
}

/*
  Front Sonar encouters will make the robot turn around
*/
void avoidObstacles() {
  if (surrounding[0] == 1) {
    turnAround(true);
  } else if (surrounding[1] == 1) {
    turnAround(true);
  } else if (surrounding[2] == 1) {
    turnAround(true);
  } else if (surrounding[3] == 1) {
    turnAround(false);
  } else if (surrounding[4] == 1) {
    turnAround(false);
  }
}

/*
  Turn the robot around to avoid front obstacle
*/
void turnAround(bool RIGHT) {
  if (!RIGHT) {
    leftMotor.reverse(100);
    rightMotor.speedUp(100);
  } else {
    leftMotor.speedUp(100);
    rightMotor.reverse(100);
  }
  delay(350);
  leftMotor.stopNow();
  rightMotor.stopNow();
  delay(10);
}

void configureMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  AcX = (Wire.read() << 8 | Wire.read()) / 16384;   
  AcY = (Wire.read() << 8 | Wire.read()) / 16384;
  AcZ = (Wire.read() << 8 | Wire.read()) / 16384;
  Tmp = ((Wire.read() << 8 | Wire.read()) / 340.00) + 36.53;
  GyX = (Wire.read() << 8 | Wire.read()) / 131;
  GyY = (Wire.read() << 8 | Wire.read()) / 131;
  GyZ = (Wire.read() << 8 | Wire.read()) / 131;
}

