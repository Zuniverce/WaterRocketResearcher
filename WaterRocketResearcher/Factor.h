#pragma once
#include "Global.h"
#include "Definition.h"
#include "Parameter.h"
class Factor
{
public:
	vector<Parameter>parameter;
	vector<unsigned>variable;
	double step;
	bool finished;
	void init();
	void input();
	void oneVariable();
};

