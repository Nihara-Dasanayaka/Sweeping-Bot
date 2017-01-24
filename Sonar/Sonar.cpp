#include "Arduino.h"
#include "Sonar.h"

Sonar::Sonar(int ECHO, int TRIG, int TimeOut, float SoundFactor) {
	// Set pin modes
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	// Initiate instance variables
	_TRIG = TRIG;
	_ECHO = ECHO;
	_TimeOut = TimeOut;
	_SoundFactor = SoundFactor; // <- 14.5 ~ 15.5
}

long Sonar::time() {
	// Trigger the device
	trigger();
	// Measuse time
	long t = pulseIn(_ECHO, HIGH, _TimeOut);
	// Return the time taken
	return t;
}

void Sonar::trigger() {
	// Make sure the TRIG pin is low to trigger
	digitalWrite(_TRIG, LOW);
	delayMicroseconds(2);
	// Make the trigger
	digitalWrite(_TRIG, HIGH);
	delayMicroseconds(10);
	// Stop the pulse
	digitalWrite(_TRIG, LOW);
}

long Sonar::distance() {
	// Measure the time taken
	long t = time();
	// Return the distance in centimeters
	return (t / _SoundFactor);
}

bool Sonar::near(long range) {
	// Take 3 measurements
	long read_1 = distance();
	delayMicroseconds(50);
	long read_2 = distance();
	delayMicroseconds(50);
	long read_3 = distance();
	// Calculate the average distance
	long current_distance = (read_1 + read_2 + read_3) / 3;
	// Decide
	if (current_distance <= range) {
		return true;
	} else {
		return false;
	}
}
