#include "StorageEnvironment.h"
#include <iostream>
#include <vector>

//  ������ ����
//  ���� ��� �̸�, �µ�, ������ �ʱ�ȭ
StorageEnvironment::StorageEnvironment(const std::string& name, float temp, float hum)
    : locationName(name), temperature(temp), humidity(hum) {
}

//  ���� ��� �̸� ��ȯ
std::string StorageEnvironment::getLocationName() const {
    return locationName;
}

//  �µ� �� ��ȯ
float StorageEnvironment::getTemperature() const {
    return temperature;
}

//  ���� �� ��ȯ
float StorageEnvironment::getHumidity() const {
    return humidity;
}

//  ���� ȯ�� ���� ��� �Լ�
//  ��ġ��, �µ�, ������ �ֿܼ� ���
void StorageEnvironment::displayInfo() const {
    std::cout << " ���� ���: " << locationName << std::endl;
    std::cout << " �µ�: " << temperature << "��C" << std::endl;
    std::cout << " ����: " << humidity << "%" << std::endl;
}
