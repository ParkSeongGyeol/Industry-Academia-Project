#include "StorageEnvironment.h"
#include <iostream>
#include <vector>

// Ŭ���� ������
StorageEnvironment::StorageEnvironment(const std::string& name, float temp, float hum)
    : locationName(name), temperature(temp), humidity(hum) {
}

std::string StorageEnvironment::getLocationName() const {
    return locationName;
}

float StorageEnvironment::getTemperature() const {
    return temperature;
}

float StorageEnvironment::getHumidity() const {
    return humidity;
}

void StorageEnvironment::displayInfo() const {
    std::cout << " ���� ���: " << locationName << std::endl;
    std::cout << " �µ�: " << temperature << "��C" << std::endl;
    std::cout << " ����: " << humidity << "%" << std::endl;
}
