#include <Sonar.h>
#include <Motor.h>
#include <Buzzer.h>

/*********************************** Sonar ************************************/
#define sonarA_ECHO     44
#define sonarA_TRIG     45
#define sonarB_ECHO     46
#define sonarB_TRIG     47
#define sonarC_ECHO     48
#define sonarC_TRIG     49
#define sonarD_ECHO     50
#define sonarD_TRIG     51
#define sonarE_ECHO     52
#define sonarE_TRIG     53
#define sonarA_NTFY     23
#define sonarB_NTFY     25
#define sonarC_NTFY     27
#define sonarD_NTFY     29
#define sonarE_NTFY     31
#define MAX_DISTANCE    3000
#define SOUND_FACTOR    59
const int DANGER_ZONE = 10;
/*********************************** Motor ************************************/
#define L_Motor_A       4
#define L_Motor_B       3
#define R_Motor_A       6
#define R_Motor_B       5
#define L_Motor_EN      2
#define R_Motor_EN      7
#define L_Motor_Speed   150
#define R_Motor_Speed   150
/********************************** Buzzer ************************************/
#define Buzzer_Pin      8

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
/********************************* Templates **********************************/
/******************************************************************************/
void scanArea();
void robotMove();
void avoidObstacles();
void turnAround(bool RIGHT);

/******************************************************************************/
/********************************* Variables **********************************/
/******************************************************************************/
short surrounding[5] = {0, 0, 0, 0, 0};
int leftMotorSpeed = 0, rightMotorSpeed = 0;

void setup() {
  delay(100);
  buzzer.startTone();
  delay(100);
}

void loop() {
  // Scan the surroundings
  scanArea();
  // Move robot
  avoidObstacles();
  robotMove();
  delay(2);
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
  delay(150);
}
