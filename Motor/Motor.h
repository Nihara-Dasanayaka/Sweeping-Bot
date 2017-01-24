#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {

	public:
		Motor(int INA, int INB, int EN, int BaseSpeed);
		void speedUp(int speed);
		void reverse(int speed);
		void stopNow();

	private:
		void initiatePins();
		int _INA;
		int _INB;
		int _EN;
		int _BaseSpeed;
};

#endif
