// StorageEnvironment.h
#pragma once  // �ߺ� ���� ������ ���� ���ù�

#include <string>

//  ���� ȯ�� ������ ��Ÿ���� Ŭ����
class StorageEnvironment {
private:
    //  ���� ��� �̸�
    std::string locationName;

    //  �µ� ���� (���� ��C ����)
    float temperature;

    //  ���� ���� (% ����)
    float humidity;

public:
    //  ������: ���� ��Ҹ�, �µ�, ������ �ʱ�ȭ
    StorageEnvironment(const std::string& name, float temp, float hum);

    //  ���� ��Ҹ� ��ȯ
    std::string getLocationName() const;

    //  �µ� �� ��ȯ
    float getTemperature() const;

    //  ���� �� ��ȯ
    float getHumidity() const;

    //  ���� ȯ�� ������ �ֿܼ� ���
    void displayInfo() const;
};
