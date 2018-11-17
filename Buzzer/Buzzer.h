#ifndef Buzzer_h
#define Buzzer_h

#include "Arduino.h"

class Buzzer {

	public:
		Buzzer(int Power);
		void alertTone();
		void startTone();
		void stuckTone();

	private:
		void initiateBuzzer();
		void varyTone(int time, int freq);
		void soundON(int time, int freq);
		void soundOFF();
		int _Power;
};

#endif
