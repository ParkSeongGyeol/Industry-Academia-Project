#include "StorageEnvironment.h"
#include <iostream>
#include <vector>

//  생성자 구현
//  보관 장소 이름, 온도, 습도를 초기화
StorageEnvironment::StorageEnvironment(const std::string& name, float temp, float hum)
    : locationName(name), temperature(temp), humidity(hum) {
}

//  보관 장소 이름 반환
std::string StorageEnvironment::getLocationName() const {
    return locationName;
}

//  온도 값 반환
float StorageEnvironment::getTemperature() const {
    return temperature;
}

//  습도 값 반환
float StorageEnvironment::getHumidity() const {
    return humidity;
}

//  보관 환경 정보 출력 함수
//  위치명, 온도, 습도를 콘솔에 출력
void StorageEnvironment::displayInfo() const {
    std::cout << " 보관 장소: " << locationName << std::endl;
    std::cout << " 온도: " << temperature << "°C" << std::endl;
    std::cout << " 습도: " << humidity << "%" << std::endl;
}
