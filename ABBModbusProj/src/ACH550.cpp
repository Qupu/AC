/*
 * ACH550.cpp
 *
 *  Created on: 28.2.2020
 *      Author: Santeri Rauhala
 */

#include "ACH550.h"

ACH550::ACH550(ModbusMaster mast) : mm(mast) {

}

ACH550::~ACH550() {
	// TODO Auto-generated destructor stub
}

void ACH550::accelerate(){

}

void ACH550::decelerate(){

}

void ACH550::stop(){

}

void ACH550::begin(){

}

void ACH550::updateSpeed(){

	CURRspeed = getCurrSpeed();

	if(CURRspeed > TARGETspeed + 5) {
		decelerate();
	}
	else if (CURRspeed < TARGETspeed - 5) {
		accelerate();
	}

}

int ACH550::getCurrSpeed(){
	int sp;

	return sp;
}

bool ACH550::getState(){
	return state;
}

void ACH550::setTargetSpeed(int sp){
	TARGETspeed = sp;
}













