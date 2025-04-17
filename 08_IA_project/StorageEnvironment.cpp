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

// main 함수 포함
int main() {
    std::vector<StorageEnvironment> storageList;

    // 샘플 데이터
    storageList.emplace_back("창고 A", 18.5f, 55.2f);
    storageList.emplace_back("지하 저장고", 12.0f, 70.0f);
    storageList.emplace_back("실험실 보관소", 22.3f, 40.0f);

    std::cout << "=== 보관 장소 환경 정보 조회 ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }

    return 0;
}
