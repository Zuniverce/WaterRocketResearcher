#include "Rocket.h"
#include "Definition.h"

#define EMPTY_MASS 0
#define WATER_VOLUME 1
#define RELATIVE_PRESSURE 2
#define GRAVITY_ACCELERATION 3
#define PROPELLANT_DENSITY 4
#define PROPELLER_VOLUME 5

double spraySpeedK[19] = { // 斜率
    7.8479600, 6.0184000, 5.0778000, 4.4798000, 4.0376000, 3.7338000, 3.4584000,
    3.2498000, 3.0690000, 2.8904000, 2.7870000, 2.6758000, 2.5564000, 2.4628000,
    0.6756000, 2.2750000, 2.1872000, 2.1362000, 2.0724000};
double spraySpeedB[19] = { // 截距
    -2.4262200, 1.2329000, 3.5844000, 5.3784000, 6.9261000, 8.1413000, 9.3806000,
    10.4236000, 11.4180000, 12.4896000, 13.1617000, 13.9401000, 14.8356000, 15.5844000,
    30.4378000, 15.2435000, 16.1215000, 16.6570000, 17.3588000};
double spray(double x) // 根据分段一次函数求喷水流速(相对压强)
{
    x += 1;
    if (x < 1.5)
        return 18.6914400 * x - 28.0371600;
    else if (x >= 11)
        return spraySpeedK[18] * x + spraySpeedB[18];
    else
        return spraySpeedK[int(int(x * 2) - 3)] * x + spraySpeedB[int(int(x * 2) - 3)];
}

void Rocket::run(const double *parameters)
{
    double VentSize = 6.361725123e-5;                               // 喷口面积
    double waterVolume = parameters[WATER_VOLUME];                  // 水体积
    double gasPressure = parameters[RELATIVE_PRESSURE];             // 绝对压强
    double gasVolume = parameters[RELATIVE_PRESSURE] - waterVolume; // 空气体积
    double originalGasVolume = gasVolume * gasPressure;             // 空气膨胀后的体积
    if (originalGasVolume <= parameters[RELATIVE_PRESSURE])         // 判断是否能将水完全喷出
    {
        result.situation = OVERWATER; // 将飞行状况标记为 OVERWATER
    }
    double totalMass = parameters[EMPTY_MASS] + waterVolume * parameters[4]; // 系统总质量
    double presentHight = 0, presentTime = 0;                                // 当前高度、时间
    double flowVelocity, flowVolume, flowMass;                               // 流速、体积流量、质量流量
    double acceleration = 0, velocity = 0;                                   // 加速度、速度
    bool DROP = false;                                                       // 下落标记
    for (; waterVolume >= 0; presentTime += factor.step)                     // 反冲推进阶段
    {
        flowVelocity = spray(gasPressure);                                                     // 根据瓶内压强获取喷水速度
        flowVolume = flowVelocity * VentSize;                                                  // 计算体积流量
        flowMass = parameters[PROPELLANT_DENSITY] * flowVolume;                                // 计算质量流量
        totalMass -= flowMass * factor.step;                                                   // 更新总质量
        acceleration = flowMass * flowVelocity / totalMass - parameters[GRAVITY_ACCELERATION]; // 通过反冲获得加速度
        presentHight += (velocity * 2 + acceleration * factor.step) / 2 * factor.step;         // 更新当前高度
        velocity += acceleration * factor.step;                                                // 更新速度
        waterVolume -= flowVolume * factor.step;                                               // 更新水量
        gasVolume = parameters[RELATIVE_PRESSURE] - waterVolume;                               // 更新空气体积
        gasPressure = originalGasVolume / gasVolume;                                           // 理想气体绝热膨胀求压强

        if ((velocity * 2 + acceleration * factor.step) <= 0 && !DROP) // 下落喷水，即达到最高点
        {
            result.topHeight = presentHight;
            result.topTime = presentTime;
            result.situation = FALL; // 将飞行状况标记为 FALL
            DROP = true;             // 将下落标记更改为真
        }
        if (DROP && presentHight <= 0) // 判定触地喷水
        {
            result.recoilTIme = presentTime;
            result.situation = CRASH; // 将飞行状况标记为 CRASH
            return;                   // 结束模拟
        }
    }
    result.recoilHight = presentHight;
    result.recoilTIme = presentTime;
    result.situation = NORMAL;                                                                           // 将飞行状况标记为 NORMAL
    for (acceleration = -parameters[GRAVITY_ACCELERATION]; presentHight > 0; presentTime += factor.step) // 自由飞行阶段
    {
        velocity += acceleration * factor.step; // 更新速度
        presentHight += velocity * factor.step; // 更新当前高度
        if (!DROP && velocity <= 0)             // 判定下落
        {
            result.topHeight = presentHight;
            result.topTime = presentTime;
            DROP = true; // 将下落标记更改为真
        }
    }
    result.freelyTime = presentTime - result.recoilTIme; // 自由飞行时间
    return;
}

void Rocket::report()
{
    cout << "最大高度 : " << result.topHeight << endl;
    cout << "反冲结束高度 : " << result.recoilHight << endl;
    cout << "反冲结束时间 : " << result.recoilTIme << endl;
    cout << "自由飞行时间 : " << result.freelyTime << endl;
    cout << "最大高度时刻 : " << result.topTime << endl;
    cout << "飞行状况 : " << result.situation << endl;
    cout << "(0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH)" << endl;
}