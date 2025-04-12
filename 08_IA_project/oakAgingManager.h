#pragma once
#include <string>
#include <vector>

using namespace std;

class OakBox {
private:
	string boxId;       // 오크통 고유 아이디
	string type;         // 종류
	string origin;       // 출신지역
	string woodType; // 나무 종류

	int ripeningPeriod;  // 숙성 기간
	int agingCount;      // 숙성 횟수
	int waterAbsorptionTime; // 물을 머금은 시간

	double evaporationRate; // 증발률
	double temperature;  // 온도
	double humidity;     // 습도

	bool roasted;        // 로스팅 여부

public:
	OakBox(string id, string t, string o, string wood, 
		int period, int count, int waterTime,
		double evarate, double temp, double hum,
		bool roast);

	void ShowInfo() const;

	// Getter
	string getId() const;

	// Setter
	void setOrigin(string o);
	void setAgingCount(int count);
	void setWaterAbsorptionTime(int t);
	void setRoasted(bool r);
	void setWoodType(string wood);
};

class OakAgingManager {
private:
	vector<OakBox> oakList;

public:
	std::string getSummary(); // 요약 정보 반환
	void OakListRun();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
};
