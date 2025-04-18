#include "StorageEnvironment.h"
#include <iostream>
#include <vector>

// 클래스 구현부
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
    std::cout << " 보관 장소: " << locationName << std::endl;
    std::cout << " 온도: " << temperature << "°C" << std::endl;
    std::cout << " 습도: " << humidity << "%" << std::endl;
}
