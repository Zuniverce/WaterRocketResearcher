#pragma once
#include "Global.h"
struct Observation
{
};
struct Result
{
	double H_recoil = 0, H_top = 0;				// 反冲结束高度、最大高度
	double T_recoil = 0, T_top = 0, T_free = 0; // 反冲结束时间、到达最高点的时间、自由飞行时间
	int situation = -1;							// 飞行状况：0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH
};
class Rocket // 火箭类
{
public:
	Observation observation;
	Result result;
	void run(const double *prmt);
	void report();
};
