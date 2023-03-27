#include "Rocket.h"
#include "Definition.h"

double spray(double x) //���ݷֶ�һ�κ�������ˮ����(���ѹǿ)
{
    double jetting_speed_k[19] = { //б��
        7.8479600 , 6.0184000 , 5.0778000 , 4.4798000 , 4.0376000 , 3.7338000 , 3.4584000 ,
        3.2498000 , 3.0690000 , 2.8904000 , 2.7870000 , 2.6758000 , 2.5564000 , 2.4628000 ,
        0.6756000 , 2.2750000 , 2.1872000 , 2.1362000 , 2.0724000
    };
    double jetting_speed_b[19] = { //�ؾ�
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
    //prmt��0��ƿ���� 1ˮ�� 2���ѹǿ 3�������ٶ� 4�ƽ����ܶ� 5�ƽ����ݻ�
    double VentSize = 6.361725123e-5; //������
    double V_water = prmt[1]; //ˮ���
    double P_air = prmt[2]; //����ѹǿ
    double V_air = prmt[5] - V_water; //�������
    double V_origin = V_air * P_air; //�������ͺ�����
    if (V_origin <= prmt[5]) //�ж��Ƿ��ܽ�ˮ��ȫ���
        result.situation = 0; //������״�����Ϊ OVERWATER
    double M = prmt[0] + V_water * prmt[4]; //ϵͳ������
    double h_present = 0, t_present = 0; //��ǰ�߶ȡ�ʱ��
    double v_e, V_e, m_e; //���١������������������
    double a = 0, u = 0; //���ٶȡ��ٶ�
    bool DROP = false; //������
    for (; V_water >= 0; t_present += factor.step) //�����ƽ��׶�
    {
        v_e = spray(P_air); //����ƿ��ѹǿ��ȡ��ˮ�ٶ�
        V_e = v_e * VentSize; //�����������
        m_e = prmt[4] * V_e; //������������
        M -= m_e * factor.step; //����������
        a = m_e * v_e / M - prmt[3]; //ͨ�������ü��ٶ�
        h_present += (u * 2 + a * factor.step) / 2 * factor.step; //���µ�ǰ�߶�
        u += a * factor.step; //�����ٶ�
        V_water -= V_e * factor.step; //����ˮ��
        V_air = prmt[5] - V_water; //���¿������
        P_air = V_origin / V_air; //�����������������ѹǿ

        if ((u * 2 + a * factor.step) <= 0 && !DROP) //������ˮ�����ﵽ��ߵ�
        {
            result.H_top = h_present;
            result.T_top = t_present;
            result.situation = 2; //������״�����Ϊ FALL
            DROP = true; //�������Ǹ���Ϊ��
        }
        if (DROP && h_present <= 0) //�ж�������ˮ  
        {
            result.T_recoil = t_present;
            result.situation = 3; //������״�����Ϊ CRASH
            return; //����ģ��
        }
    }
    result.H_recoil = h_present;
    result.T_recoil = t_present;
    result.situation = 1; //������״�����Ϊ NORMAL
    for (a = -prmt[3]; h_present > 0; t_present += factor.step) //���ɷ��н׶�
    {
        u += a * factor.step; //�����ٶ�
        h_present += u * factor.step; //���µ�ǰ�߶�
        if (!DROP && u <= 0) //�ж�����
        {
            result.H_top = h_present;
            result.T_top = t_present;
            DROP = true; //�������Ǹ���Ϊ��
        }
    }
    result.T_free = t_present - result.T_recoil; //���ɷ���ʱ��
    return;
}

void Rocket::report()
{
    cout << "���߶� : " << result.H_top << endl;
    cout << "��������߶� : " << result.H_recoil << endl;
    cout << "�������ʱ�� : " << result.T_recoil << endl;
    cout << "���ɷ���ʱ�� : " << result.T_free << endl;
    cout << "���߶�ʱ�� : " << result.T_top << endl;
    cout << "����״�� : " << result.situation << endl;
    cout << "(0 OVERWATER, 1 NORMAL, 2 FALL , 3 CRASH)" << endl;
}