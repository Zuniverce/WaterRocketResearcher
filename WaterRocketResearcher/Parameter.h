#pragma once
#include "Global.h"
class Parameter
{
public:
	string name;	  // 参数名
	string unit;	  // 单位
	double value;	  // 参数值
	double min;		  // 下限
	double step;	  // 变化间隔
	double max;		  // 上限
	double defaultt;  // 默认值
	bool defaultFlag; // 是否存在默认值
	Parameter();
};
