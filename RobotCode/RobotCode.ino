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
int calculateSpeed();
void robotMove(int Speed);
void avoidObstacles();

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
  // Calculate speed accordingly
  robotMove(calculateSpeed());
  avoidObstacles();
  delay(10);
}

/*
 An array of 1's and 0's will be created according to position
*/
void scanArea() {
  surrounding[0] = sonarA.near(DANGER_ZONE) ? 1 : 0;
  surrounding[1] = sonarB.near(DANGER_ZONE) ? 1 : 0;
  surrounding[2] = sonarC.near(DANGER_ZONE) ? 1 : 0;
  surrounding[3] = sonarD.near(DANGER_ZONE) ? 1 : 0;
  surrounding[4] = sonarE.near(DANGER_ZONE) ? 1 : 0;
}

/*
 The speed will be calculated according to the position
*/
int calculateSpeed() {
  int SPEED = 0;
  for (int i = 0; i < 5; i++) {
    SPEED += surrounding[i] * (i + 1) * 10;
  }
  return SPEED;
}

void robotMove(int Speed) {
  leftMotor.speedUp(Speed);
  rightMotor.speedUp(Speed);
}

void avoidObstacles() {
  if (surrounding[2] == 1) {
    leftMotor.stopNow();
    rightMotor.stopNow();
  }
}

/*
sonarA.near(10);
  sonarB.near(10);
  sonarC.near(10);
  sonarD.near(10);
  sonarE.near(10);
  delay(100);
  /*
  // Move forward for 2 seconds
  leftMotor.speedUp(100);
  rightMotor.speedUp(100);
  delay(3000);
  // Play alert tone
  buzzer.alertTone();
  // Stop
  rightMotor.stopNow();
  leftMotor.stopNow();
  delay(5000);
  // Play alter tone
  buzzer.alertTone();
  // Reverse for 2 seconds
  leftMotor.reverse(100);
  rightMotor.reverse(100);
  delay(3000);
  // Play alert tone
  buzzer.alertTone();
  // Stop
  rightMotor.stopNow();
  leftMotor.stopNow();
  delay(5000);*/
  /*
  sonarA.near(10);
  sonarB.near(10);
  sonarC.near(10);
  sonarD.near(10);
  sonarE.near(10);  
  delay(100);*/


