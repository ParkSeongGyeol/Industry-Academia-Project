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
	std::string getSummary(); // ��� ���� ��ȯ
	void OakListRun();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
};
