// StorageEnvironment.h
#pragma once  // 중복 포함 방지를 위한 지시문

#include <string>

//  보관 환경 정보를 나타내는 클래스
class StorageEnvironment {
private:
    //  보관 장소 이름
    std::string locationName;

    //  온도 정보 (섭씨 °C 단위)
    float temperature;

    //  습도 정보 (% 단위)
    float humidity;

public:
    //  생성자: 보관 장소명, 온도, 습도를 초기화
    StorageEnvironment(const std::string& name, float temp, float hum);

    //  보관 장소명 반환
    std::string getLocationName() const;

    //  온도 값 반환
    float getTemperature() const;

    //  습도 값 반환
    float getHumidity() const;

    //  보관 환경 정보를 콘솔에 출력
    void displayInfo() const;
};
