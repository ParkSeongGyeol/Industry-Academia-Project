// StorageEnvironment.h
#pragma once
#include <string>

class StorageEnvironment {
private:
    std::string locationName;
    float temperature; // �µ� (��C)
    float humidity;    // ���� (%)

public:
    StorageEnvironment(const std::string& name, float temp, float hum);

    std::string getLocationName() const;
    float getTemperature() const;
    float getHumidity() const;

    void displayInfo() const;
};
#pragma once
