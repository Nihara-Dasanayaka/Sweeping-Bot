#include <Sonar.h>
#include <Motor.h>
#include <Buzzer.h>

// Constants for Sonar
#define sonarA_ECHO     22
#define sonarB_ECHO     24
#define sonarC_ECHO     26
#define sonarD_ECHO     28
#define sonarE_ECHO     30
#define sonarA_TRIG     23
#define sonarB_TRIG     25
#define sonarC_TRIG     27
#define sonarD_TRIG     29
#define sonarE_TRIG     31
#define MAX_DISTANCE    4000
#define SOUND_FACTOR    59

// Constants for Motors
#define L_Motor_A       2
#define L_Motor_B       3
#define R_Motor_A       4
#define R_Motor_B       5
#define L_Motor_EN      6
#define R_Motor_EN      7
#define L_Motor_Speed   150
#define R_Motor_Speed   150

// Constants for Buzzer
#define Buzzer_Pin      8

// Sonar Sensors
Sonar sonarA(sonarA_ECHO, sonarA_TRIG, MAX_DISTANCE, SOUND_FACTOR);
Sonar sonarB(sonarB_ECHO, sonarB_TRIG, MAX_DISTANCE, SOUND_FACTOR);
Sonar sonarC(sonarC_ECHO, sonarC_TRIG, MAX_DISTANCE, SOUND_FACTOR);
Sonar sonarD(sonarD_ECHO, sonarD_TRIG, MAX_DISTANCE, SOUND_FACTOR);
Sonar sonarE(sonarE_ECHO, sonarE_TRIG, MAX_DISTANCE, SOUND_FACTOR);
// Motors
Motor leftMotor(L_Motor_A, L_Motor_B, L_Motor_EN, L_Motor_Speed);
Motor rightMotor(R_Motor_A, R_Motor_B, R_Motor_EN, R_Motor_Speed);
// Buzzer
Buzzer buzzer(Buzzer_Pin);

void setup() {
  delay(100);
  buzzer.startTone();
  // Turn on some decorative lights
}

void loop() {
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
  delay(5000);
}
