#include "Rocket.h"
#include "Definition.h"

double spray(double x) //根据分段一次函数求喷水流速(相对压强)
{
    double jetting_speed_k[19] = { //斜率
        7.8479600 , 6.0184000 , 5.0778000 , 4.4798000 , 4.0376000 , 3.7338000 , 3.4584000 ,
        3.2498000 , 3.0690000 , 2.8904000 , 2.7870000 , 2.6758000 , 2.5564000 , 2.4628000 ,
        0.6756000 , 2.2750000 , 2.1872000 , 2.1362000 , 2.0724000
    };
    double jetting_speed_b[19] = { //截距
        -2.4262200 , 1.2329000 , 3.5844000 , 5.3784000 , 6.9261000 , 8.1413000 , 9.3806000 ,
        10.4236000 ,11.4180000 ,12.4896000 ,13.1617000 ,13.9401000 ,14.8356000 ,15.5844000 ,
        30.4378000 ,15.2435000 ,16.1215000 ,16.6570000 ,17.3588000
    };
    x += 1;
    if (x < 1.5)
        return 18.6914400 * x - 28.0371600;
    else if (x >= 11)
        return jetting_speed_k[18] * x + jetting_speed_b[18];
    else
        return jetting_speed_k[int(int(x * 2) - 3)] * x + jetting_speed_b[int(int(x * 2) - 3)];
}

void Rocket::run(const double* prmt)
{
    //prmt：0空瓶质量 1水量 2相对压强 3重力加速度 4推进剂密度 5推进器容积
    double VentSize = 6.361725123e-5; //喷口面积
    double V_water = prmt[1]; //水体积
    double P_air = prmt[2]; //绝对压强
    double V_air = prmt[5] - V_water; //空气体积
    double V_origin = V_air * P_air; //空气膨胀后的体积
    if (V_origin <= prmt[5]) //判断是否能将水完全喷出
        result.situation = 0; //将飞行状况标记为 OVERWATER
    double M = prmt[0] + V_water * prmt[4]; //系统总质量
    double h_present = 0, t_present = 0; //当前高度、时间
    double v_e, V_e, m_e; //流速、体积流量、质量流量
    double a = 0, u = 0; //加速度、速度
    bool DROP = false; //下落标记
    for (; V_water >= 0; t_present += factor.step) //反冲推进阶段
    {
        v_e = spray(P_air); //根据瓶内压强获取喷水速度
        V_e = v_e * VentSize; //计算体积流量
        m_e = prmt[4] * V_e; //计算质量流量
        M -= m_e * factor.step; //更新总质量
        a = m_e * v_e / M - prmt[3]; //通过反冲获得加速度
        h_present += (u * 2 + a * factor.step) / 2 * factor.step; //更新当前高度
        u += a * factor.step; //更新速度
        V_water -= V_e * factor.step; //更新水量
        V_air = prmt[5] - V_water; //更新空气体积
        P_air = V_origin / V_air; //理想气体绝热膨胀求压强

        if ((u * 2 + a * factor.step) <= 0 && !DROP) //下落喷水，即达到最高点
        {
            result.H_top = h_present;
            result.T_top = t_present;
            result.situation = 2; //将飞行状况标记为 FALL
            DROP = true; //将下落标记更改为真
        }
        if (DROP && h_present <= 0) //判定触地喷水  
        {
            result.T_recoil = t_present;
            result.situation = 3; //将飞行状况标记为 CRASH
            return; //结束模拟
        }
    }
    result.H_recoil = h_present;
    result.T_recoil = t_present;
    result.situation = 1; //将飞行状况标记为 NORMAL
    for (a = -prmt[3]; h_present > 0; t_present += factor.step) //自由飞行阶段
    {
        u += a * factor.step; //更新速度
        h_present += u * factor.step; //更新当前高度
        if (!DROP && u <= 0) //判定下落
        {
            result.H_top = h_present;
            result.T_top = t_present;
            DROP = true; //将下落标记更改为真
        }
    }
    result.T_free = t_present - result.T_recoil; //自由飞行时间
    return;
}

void Rocket::report()
{
    cout << "最大高度 : " << result.H_top << endl;
    cout << "反冲结束高度 : " << result.H_recoil << endl;
    cout << "反冲结束时间 : " << result.T_recoil << endl;
    cout << "自由飞行时间 : " << result.T_free << endl;
    cout << "最大高度时刻 : " << result.T_top << endl;
    cout << "飞行状况 : " << result.situation << endl;
    cout << "(0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH)" << endl;
}