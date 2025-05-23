#pragma once
#include <string>
#include <vector>

using namespace std;

class OakBox {
private:
	string boxId;       // 오크통 고유 아이디
	string name;        // 오크통 별칭
	string type;        // 종류 (버번통, 셰리통 등)
	string origin;      // 출신지역
	string woodType;    // 나무 종류 (화이트 오크 등)
	string spiritId;    // 숙성된 스피릿 ID 
	string agingStartDate; // 숙성 시작일 
	string agingEndDate;   // 숙성 종료일

	int ripeningPeriod;  // 숙성 기간 (일수)
	int agingCount;      // 숙성 횟수
	int waterAbsorptionTime; // 물을 머금은 시간

	double evaporationRate; // 증발률
	double temperature;  // 온도
	double humidity;     // 습도

	bool roasted;        // 로스팅 여부

public:
	OakBox(string id, string name, string t, string o, string wood,
		string spiritId, string startDate, string endDate,
		int period, int count, int waterTime,
		double evarate, double temp, double hum,
		bool roast);

	void ShowInfo() const;

	// Getter
	string getId() const;
	string getName() const;            
	string getType() const;
	string getOrigin() const;
	string getWoodType() const;
	string getSpiritId() const;         
	string getAgingStartDate() const;    
	string getAgingEndDate() const;      

	int getRipeningPeriod() const;
	int getAgingCount() const;
	int getWaterAbsorptionTime() const;

	double getEvaporationRate() const;
	double getTemperature() const;
	double getHumidity() const;

	bool isRoasted() const;

	// Setter
	void setId(string id);
	void setName(string name);                
	void setType(string t);
	void setOrigin(string o);
	void setWoodType(string wood);
	void setSpiritId(string id);              
	void setAgingStartDate(string date);      
	void setAgingEndDate(string date);        

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
	string getSummary(); // 요약 정보 반환
	vector<string> getPageInfoLines();

	void showOakAgingPage();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
	void exportOakBoxesToCSV(const string& filename);
};
