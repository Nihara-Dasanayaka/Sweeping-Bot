#ifndef Sonar_h
#define Sonar_h

#include "Arduino.h"

class Sonar {

	public:
		Sonar(int ECHO, int TRIG, int TimeOut, float SoundFactor);
		long distance();
		bool near(long range);

	private:
		long time();
		void trigger();
		int _ECHO;
		int _TRIG;
		int _TimeOut;
		float _SoundFactor;
};

#endif
