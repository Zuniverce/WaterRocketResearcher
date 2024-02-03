#pragma once
#include "Global.h"

#define OVERWATER 0
#define NORMAL 1
#define FALL 2
#define CRASH 3

struct Observation
{
};
struct Result
{
	double recoilHight = 0, topHeight = 0;				// 反冲结束高度、最大高度
	double recoilTIme = 0, topTime = 0, freelyTime = 0; // 反冲结束时间、到达最高点的时间、自由飞行时间
	int situation = -1;							// 飞行状况：0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH
};
class Rocket // 火箭类
{
public:
	Observation observation;
	Result result;
	void run(const double *parameters);
	void report();
};
