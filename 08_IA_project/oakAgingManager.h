#pragma once
#include <string>
#include <vector>

using namespace std;

class OakBox {
private:
	string boxId;       // ��ũ�� ���� ���̵�
	string name;        // ��ũ�� ��Ī
	string type;        // ���� (������, �θ��� ��)
	string origin;      // �������
	string woodType;    // ���� ���� (ȭ��Ʈ ��ũ ��)
	string spiritId;    // ������ ���Ǹ� ID 
	string agingStartDate; // ���� ������ 
	string agingEndDate;   // ���� ������

	int ripeningPeriod;  // ���� �Ⱓ (�ϼ�)
	int agingCount;      // ���� Ƚ��
	int waterAbsorptionTime; // ���� �ӱ��� �ð�

	double evaporationRate; // ���߷�
	double temperature;  // �µ�
	double humidity;     // ����

	bool roasted;        // �ν��� ����

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
	string getSummary(); // ��� ���� ��ȯ
	vector<string> getPageInfoLines();

	void showOakAgingPage();
	void showOakList();
	void addOakBox();
	void updateOakBox();
	void deleteOakBox();
	void exportOakBoxesToCSV(const string& filename);
};
