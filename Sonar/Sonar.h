#ifndef Sonar_h
#define Sonar_h

#define DANGER	true
#define SAFE	false

#include "Arduino.h"

class Sonar {

	public:
		Sonar(int ECHO, int TRIG, int TimeOut, float SoundFactor, int Notifier);
		long distance();
		bool near(long range);

	private:
		long time();
		void trigger();
		void notify(bool state);
		int _ECHO;
		int _TRIG;
		int _TimeOut;
		int _Notifier;
		float _SoundFactor;
};

#endif
