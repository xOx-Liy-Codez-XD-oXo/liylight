//I'm lily and this file's public domain!!! Do whatever with it!!

#ifndef LIYLIGHT_H
#define LIYLIGHT_H

#include <unistd.h> //For usleep
#include <ogc/lwp.h> //For thread

//Im choosed 4096 because it's good balance between flicker and dimmest possible value
//When it's lower, there will be a noticable jump between it's lowest value and off.
//When it's higher, flicker will be more noticable
#define LIYLIGHT_PWM_FREQ 4096

unsigned short _liylight_doody = 0;
unsigned char _liylight_active = 0;
lwp_t _liylight_threadhandle = LWP_THREAD_NULL;

void * _liylight_thread(void * pee) {
	while(_liylight_active) {
		int ontime = _liylight_doody;
		int offtime = (LIYLIGHT_PWM_FREQ) - ontime;
		if(_liylight_doody != 0) {
			*(int *)0xcd8000c0 |= 0x20; //Hollywood gpio disc light
			usleep(ontime);
		}
		*(int *)0xcd8000c0 &= ~0x20;
		usleep(offtime);
	}
	return NULL;
}

void liylight_setDutyDirect(unsigned short a) {
	a = (a > LIYLIGHT_PWM_FREQ - 1) ? LIYLIGHT_PWM_FREQ - 1 : a;
	a = (a < 0) ? 0 : a;
	_liylight_doody = a;
}

void liylight_setDutyFloat(float a) {
	//squaring it just makes it look how you expect it to
	liylight_setDutyDirect( (int)((float)LIYLIGHT_PWM_FREQ * (a * a)) );
}

void liylight_init() {
	if(!_liylight_active) {
		_liylight_active = 1;
		LWP_CreateThread(&_liylight_threadhandle, _liylight_thread, NULL, NULL, 0, 70);
	}
}

void liylight_stop() {
	if(_liylight_active) {
		_liylight_active = 0;
	}
}

#endif //LIYLIGHT_H
