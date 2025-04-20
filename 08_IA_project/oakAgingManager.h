#pragma once
#include <string>
#include <vector>

using namespace std;

class OakBox {
private:
	string boxId;       // ��ũ�� ���� ���̵�
	string type;         // ����
	string origin;       // �������
	string woodType; // ���� ����

	int ripeningPeriod;  // ���� �Ⱓ
	int agingCount;      // ���� Ƚ��
	int waterAbsorptionTime; // ���� �ӱ��� �ð�

	double evaporationRate; // ���߷�
	double temperature;  // �µ�
	double humidity;     // ����

	bool roasted;        // �ν��� ����

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
	std::string getSummary(); // ��� ���� ��ȯ
	std::vector<std::string> getPageInfoLines();

	void showOakAgingPage();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
};
