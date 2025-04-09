#pragma once
#include <string>

class OakBox {
private:
    std::string boxId; // 오크통 고유 아이디
	std::string type; // 종류
	std::string origin; // 출신지역
	int ripeningPeriod; // 숙성 기간
	int agingCount; // 숙성 횟수
	double evaporationRate; // 증발률
	double temperature; // 온도
	double humidity; // 습도
	bool roasted; // 로스팅 여부

public:
    OakBox(std::string id, std::string t, std::string o, int period, int count, double evarate, double temp, double hum, bool roast);
    void ShowInfo() const;
};

class OakAgingManager {
public:
    void showOakList();
};
