#pragma once
#include "Global.h"
#include "Factor.h"	
#include "Rocket.h"
#define PARAMETERNUM 6
#define MINUS -1
#define PI 3.1415926
double unitTransform(unsigned presentParameter, double value, bool direction);
extern Factor factor;
extern Rocket rocket;