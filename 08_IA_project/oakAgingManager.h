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
	std::string getType() const;
	std::string getOrigin() const;
	std::string getWoodType() const;

	int getRipeningPeriod() const;
	int getAgingCount() const;
	int getWaterAbsorptionTime() const;

	double getEvaporationRate() const;
	double getTemperature() const;
	double getHumidity() const;

	bool isRoasted() const;

	// Setter
	void setOrigin(string o);
	void setWoodType(string wood);
	void setType(string t);
	void setId(string id);

	void setAgingCount(int count);
	void setWaterAbsorptionTime(int t);
	void setRipeningPeriod(int p);
	
	void setEvaporationRate(double e);
	void setTemperature(double t);
	void setHumidity(double h);

	void setRoasted(bool r);
};

class OakAgingManager {
private:
	vector<OakBox> oakList;

public:
	std::string getSummary(); // 요약 정보 반환
	std::vector<std::string> getPageInfoLines();

	void showOakAgingPage();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
};
