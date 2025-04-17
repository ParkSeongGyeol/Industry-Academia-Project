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

// main �Լ� ����
int main() {
    std::vector<StorageEnvironment> storageList;

    // ���� ������
    storageList.emplace_back("â�� A", 18.5f, 55.2f);
    storageList.emplace_back("���� �����", 12.0f, 70.0f);
    storageList.emplace_back("����� ������", 22.3f, 40.0f);

    std::cout << "=== ���� ��� ȯ�� ���� ��ȸ ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }

    return 0;
}
