#include "Factor.h"
#include "Definition.h"
void Factor::init()
{
    finished = false;
    for (unsigned i = 0; i < PARAMETERNUM; i++)
    {
        parameter.push_back(Parameter());
    }
    //������
    parameter[0].name = "�������";
    parameter[1].name = "ˮ��";
    parameter[2].name = "���ѹǿ";
    parameter[3].name = "�������ٶ�";
    parameter[4].name = "�ƽ����ܶ�";
    parameter[5].name = "�ƽ����ݻ�";
    //��λ
    parameter[0].unit = "(g)";
    parameter[1].unit = "(ml)";
    parameter[2].unit = "(bar)";
    parameter[3].unit = "(m/s^2)";
    parameter[4].unit = "(g/cm^3)";
    parameter[5].unit = "(ml)";
    //Ĭ��ֵ
    parameter[0].defaultt = 0;
    parameter[1].defaultt = 0;
    parameter[2].defaultt = 0;
    parameter[3].defaultt = 9.807;
    parameter[4].defaultt = 1e3;
    parameter[5].defaultt = 1e-3;
    //�Ƿ����Ĭ��ֵ
    parameter[0].defaultFlag = false;
    parameter[1].defaultFlag = false;
    parameter[2].defaultFlag = false;
    parameter[3].defaultFlag = true;
    parameter[4].defaultFlag = true;
    parameter[5].defaultFlag = true;
    //Ĭ��ģ����
    parameter[0].delta = unitTransform(0, 10, true);
    parameter[1].delta = unitTransform(1, 10, true);
    parameter[2].delta = unitTransform(2, 0.25, true);
    parameter[3].delta = unitTransform(3, 0.1, true);
    parameter[4].delta = unitTransform(4, 0.01, true);
    parameter[5].delta = unitTransform(5, 10, true);
}

void Factor::input()
{
    int numOfVariable = MINUS;
    while (numOfVariable < 1 || numOfVariable > 2)
    {
        cout << "�������� (1~2):" << endl;
        cin >> numOfVariable;
    }
    for (unsigned i = 0; i < PARAMETERNUM; i++)
    {
        cout << i << " " << parameter[i].name << endl;
    }
    for (int i = 0; i < numOfVariable; i++)
    {
        int codeOfVariable = MINUS;
        while (codeOfVariable < 0 || codeOfVariable >= PARAMETERNUM)
        {
            cout << "�� " << i + 1 << " �������Ĵ��� (0~" << PARAMETERNUM << "):" << endl;
            cin >> codeOfVariable;
        }
        variable.push_back(codeOfVariable);
        parameter[codeOfVariable].defaultFlag = false;
        double limitValue = MINUS;
        while (limitValue < 0)
        {
            cout << parameter[codeOfVariable].name << " ���� " <<
                parameter[codeOfVariable].unit << ":" << endl;
            cin >> limitValue;
        }
        parameter[codeOfVariable].limit =
            unitTransform(codeOfVariable, limitValue, true);
        double deltaValue = MINUS;
        while (deltaValue < 0 || deltaValue>limitValue)
        {
            cout << parameter[codeOfVariable].name << " ��� " <<
                parameter[codeOfVariable].unit << ":" << endl;
            cin >> deltaValue;
        }
        parameter[codeOfVariable].delta =
            unitTransform(codeOfVariable, deltaValue, true);
    }
    for (unsigned i = 0; i < PARAMETERNUM; i++)
    {
        bool isVariable = false;
        for (int j = 0; j < numOfVariable; j++)
        {
            if (i == variable[j])
            {
                isVariable = true;
            }
        }
        if (isVariable)
        {
            continue;
        }
        double otherParameterValue = MINUS;
        cout << parameter[i].name << " " <<
            parameter[i].unit << ":" << endl;
        if (parameter[i].defaultFlag)
        {
            cout << "(����-1��ʹ��Ĭ��ֵ " <<
                unitTransform(i, parameter[i].defaultt, false) <<
                " )" << endl;
        }
        else
        {
            cout << "(������д)" << endl;
        }
        cin >> otherParameterValue;
        if (otherParameterValue < 0)
        {
            parameter[i].value = parameter[i].defaultt;
        }
        else
        {
            parameter[i].value = unitTransform(i, otherParameterValue, true);
        }
    }
    cout << "��������:" << endl;
    cin >> step;
    return;
}

void Factor::oneVariable()
{
    double sender[PARAMETERNUM]{};
    for (unsigned i = 0; i < PARAMETERNUM; i++)
    {
        if (i == variable[0])
        {
            sender[i] = 0;
        }
        else
        {
            sender[i] = parameter[i].value;
        }
    }
    cout << parameter[variable[0]].name <<
        parameter[variable[0]].unit <<
        " �߶�" << endl;
    double iterLimit = parameter[variable[0]].limit / parameter[variable[0]].delta;
    for (unsigned iter = 0; iter < iterLimit; iter++)
    {
        sender[variable[0]] += parameter[variable[0]].delta;
        rocket.run(sender);
        cout << unitTransform(variable[0], sender[variable[0]], false) << " ";
        cout << rocket.result.H_top << endl;
    }
}

void Factor::twoVariable()
{
    double sender[PARAMETERNUM]{};
    for (unsigned i = 0; i < PARAMETERNUM; i++)
    {
        bool isVariable = false;
        for (unsigned j = 0; j < variable.size(); j++)
        {
            if (i == variable[j])
            {
                isVariable = true;
            }
        }
        if (isVariable)
        {
            sender[i] = 0;
        }
        else
        {
            sender[i] = parameter[i].value;
        }
    }
    double iterLimitY = parameter[variable[0]].limit / parameter[variable[0]].delta;
    double iterLimitX = parameter[variable[0]].limit / parameter[variable[0]].delta;
    cout << 0;
    for (unsigned iterX = 0; iterX < iterLimitX; iterX++)
    {
        sender[variable[1]] += parameter[variable[1]].delta;
        cout << unitTransform(variable[1], sender[variable[1]], false);
    }
    for (unsigned iterY = 0; iterY < iterLimitY; iterY++)
    {
        sender[variable[0]] += parameter[variable[0]].delta;
        sender[variable[1]] = 0;
        cout << unitTransform(variable[0], sender[variable[0]], false);
        for (unsigned iterX = 0; iterX < iterLimitX; iterX++)
        {
            sender[variable[1]] += parameter[variable[1]].delta;
            rocket.run(sender);
            cout << " " << rocket.result.H_top;
        }
        cout << endl;
    }
}

double Factor::unitTransform(unsigned presentParameter, double value, bool direction)
{
    if (direction == true)
    {
        if (presentParameter == 0)
            return value / 1e3;
        else if (presentParameter == 1)
            return value / 1e6;
        else if (presentParameter == 2)
            return value;
        else if (presentParameter == 3)
            return value;
        else if (presentParameter == 4)
            return value * 1e3;
        else if (presentParameter == 5)
            return value / 1e6;
        return value * value * PI / 4e6;
    }
    else
    {
        if (presentParameter == 0)
            return value * 1e3;
        else if (presentParameter == 1)
            return value * 1e6;
        else if (presentParameter == 2)
            return value;
        else if (presentParameter == 3)
            return value;
        else if (presentParameter == 4)
            return value / 1e3;
        else if (presentParameter == 5)
            return value * 1e6;
        return sqrt(value * 4e6 / PI);
    }
}
