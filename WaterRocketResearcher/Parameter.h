#pragma once
#include "Global.h"
class Parameter
{
public:
	string name;        //������
	string unit;        //��λ
	double value;       //����ֵ
	double delta;       //�仯���
	double limit;       //����
	double defaultt;    //Ĭ��ֵ
	bool defaultFlag; //�Ƿ����Ĭ��ֵ
	Parameter();
};

