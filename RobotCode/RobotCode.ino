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
#define L_Motor_Speed   200
#define R_Motor_Speed   200
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
/*********************************** MPU **************************************/
/******************************************************************************/
const int MPU          = 0x68;
const int GYRO_LIMIT   = 3;
const int MAX_TEMP     = 40;
const int GYRO_SAMPLES = 5;

/******************************************************************************/
/********************************* Templates **********************************/
/******************************************************************************/
void scanArea();
void robotMove();
void avoidObstacles();
void turnAround(bool RIGHT);
void initiateVariables();
void configureMPU();
void readMPU();
void fillArray(int16_t value, int16_t arr[]);
bool isStuck();
bool overHeating();
bool checkElements(int16_t arr[]);
void printArray(int16_t arr[]);
void unStuck();

/******************************************************************************/
/********************************* Variables **********************************/
/******************************************************************************/
short surrounding[5] = {0, 0, 0, 0, 0};
int leftMotorSpeed = 0, rightMotorSpeed = 0;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int16_t Gx[GYRO_SAMPLES] = {10, 10, 10, 10, 10};
int16_t Gy[GYRO_SAMPLES] = {10, 10, 10, 10, 10};
int16_t Gz[GYRO_SAMPLES] = {10, 10, 10, 10, 10};

void setup() {
  // Play start tone
  delay(100);
  buzzer.startTone();
  delay(200);
  buzzer.alertTone();
  delay(200);
  // Setup communication
  configureMPU();
  // Setup logging  
  Serial.begin(9600);
}

void loop() {
  // Check if the system is too hot
  scanArea();
  readMPU();
  if (!overHeating()) {
    Serial.println("I am cool!");
    if (!isStuck()) {
      Serial.println("I am fre!");
      avoidObstacles();
      robotMove();
    } else {
      buzzer.stuckTone();
      Serial.println("I am stuck!");
      unStuck();
      // Move a bit away
    }
  } else {
    Serial.println("I am overheating!");
    leftMotor.stopNow();
    rightMotor.stopNow();
    delay(500);
  }
  delay(100);
}

void unStuck() {
  leftMotor.reverse(100);
  rightMotor.reverse(100);
  delay(25);
  leftMotor.reverse(100);
  rightMotor.speedUp(100);
  delay(500);
  leftMotor.stopNow();
  rightMotor.stopNow();
  delay(10);
}

void logSonars() {
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
  leftMotor.speedUp(leftMotorSpeed + 20);
  rightMotor.speedUp(rightMotorSpeed - 20);
  delay(50);
  leftMotor.speedUp(leftMotorSpeed - 20);
  rightMotor.speedUp(rightMotorSpeed + 20);
  delay(50);
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

/**
 * Set up communication between MPU and Arduino
 */
void configureMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

/**
 * Take readings from the IMU
 */
void readMPU() {
  // Initiate communication
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  // Read register values
  AcX = (Wire.read() << 8 | Wire.read()) / 16384;   
  AcY = (Wire.read() << 8 | Wire.read()) / 16384;
  AcZ = (Wire.read() << 8 | Wire.read()) / 16384;
  Tmp = ((Wire.read() << 8 | Wire.read()) / 340.00) + 36.53;
  GyX = (Wire.read() << 8 | Wire.read()) / 131;
  GyY = (Wire.read() << 8 | Wire.read()) / 131;
  GyZ = (Wire.read() << 8 | Wire.read()) / 131;
  // Fill out the arrays
  fillArray(GyX, Gx);
  fillArray(GyY, Gy);
  fillArray(GyZ, Gz);
}

void fillArray(int16_t value, int16_t arr[]) {
  for (int i = 1; i < GYRO_SAMPLES; i++) {
    arr[i - 1] = arr[i];
  }
  arr[GYRO_SAMPLES - 1] = value;
}

void printArray(int16_t arr[]) {
  Serial.print("{");
  for (int i = 0; i < GYRO_SAMPLES; i++) {
    Serial.print(arr[i]);
    if (i != (GYRO_SAMPLES - 1)) {
      Serial.print(", ");
    }
  }
  Serial.println("}");
}

/**
 * Checks whether the robot is stalling for a while
 */
bool isStuck() {
  if (checkElements(Gx) && checkElements(Gy) && checkElements(Gz)) {
    return true;
  }
  return false;
}

/**
 * Checks if all the elements in the array are close by values
 */
bool checkElements(int16_t arr[]) {
  int16_t normal = 0;
  for (int n = 0; n < GYRO_SAMPLES; n++) {
    normal += abs(arr[n]);
  }
  normal = (normal / GYRO_SAMPLES);
  return ((normal < GYRO_LIMIT));
}

/**
 * Checks if the internal of robot is getting too hot
 */
bool overHeating() {
  if (Tmp > MAX_TEMP) {
    Serial.print("Temperature is ");
    Serial.println(Tmp);
    buzzer.alertTone();
    return true;
  } else {
    return false;
  }
}
