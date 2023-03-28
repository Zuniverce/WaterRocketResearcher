#include "Global.h"
#include "Definition.h"
#include "Parameter.h"
#include "Rocket.h"
#include "Factor.h"
#include <Windows.h>

int main()
{
	factor.init();
	factor.input();
	/*if (factor.variable.size() == 1)
	{
		factor.oneVariable();
	}
	else 
	{
		factor.twoVariable();
	}*/
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
	system("pause");
	return 0;
}
