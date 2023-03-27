#pragma once
#include "Global.h"
#include "Definition.h"
const double stp = 1e-4; //��������(s)
struct Observation {

};
struct Result {
	double H_recoil = 0, H_top = 0; //��������߶ȡ����߶�
	double T_recoil = 0, T_top = 0, T_free = 0; //�������ʱ�䡢������ߵ��ʱ�䡢���ɷ���ʱ��
	int situation = -1; //����״����0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH
};
class Rocket // �����
{
public:
	Observation observation;
	Result result;
	void run(const double* prmt);
	void report();
};

