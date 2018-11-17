#include "Arduino.h"
#include "Buzzer.h"

Buzzer::Buzzer(int Power) {
	// Initiate instance variables
	_Power = Power;
	// Initiate Buzzer
	initiateBuzzer();
}

void Buzzer::initiateBuzzer() {
	pinMode(_Power, OUTPUT);
}

void Buzzer::alertTone() {
	for (int i = 0; i < 2; i++) {
		varyTone(20, 10);
		varyTone(20, 25);
		varyTone(20, 50);
		varyTone(20, 75);
		varyTone(20, 85);
		varyTone(20, 95);
		varyTone(20, 100);
		varyTone(20, 105);
		varyTone(20, 100);
		varyTone(20, 95);
		varyTone(20, 85);
		varyTone(20, 75);
		varyTone(20, 50);
		varyTone(20, 25);		
		varyTone(1, 0);		
	}
}

void Buzzer::startTone() {
	for (int i = 0; i < 5; i++) {
		varyTone(20, 10);
		varyTone(20, 25);
		varyTone(1, 0);
	}
}

void Buzzer::stuckTone() {
	varyTone(30, 10);
	varyTone(5, 0);
	varyTone(20, 200);
	varyTone(1, 0);
}

void Buzzer::varyTone(int time, int freq) {
	soundON(time, freq);
}

void Buzzer::soundON(int time, int freq) {
	analogWrite(_Power, freq);
	delay(time);
}

void Buzzer::soundOFF() {
	analogWrite(_Power, 0);
}

