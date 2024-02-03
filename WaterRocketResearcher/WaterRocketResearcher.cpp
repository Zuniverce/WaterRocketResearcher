#include "Global.h"
#include "Definition.h"
#include "Parameter.h"
#include "Rocket.h"
#include "Factor.h"

int main()
{
	factor.init();
	factor.input();
	switch (factor.variable.size())
	{
	case 0:
		factor.zeroVariable();
	case 1:
		factor.oneVariable();
	case 2:
		factor.twoVariable();
	default:
		break;
	}
	return 0;
}
