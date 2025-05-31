#pragma once
#include <string>
#include <vector>

using namespace std;

class OakBox {
private:
	string boxId;                 // ��ũ�� ���� ���̵�
	string name;                  // ��ũ�� ��Ī
	string type;                  // ���� (������, �θ��� ��)
	string origin;                // �������
	string woodType;              // ���� ���� (ȭ��Ʈ ��ũ ��)
	string spiritId;              // ������ ���Ǹ� ID 
	string agingStartDate;        // ���� ������ 
	string agingEndDate;          // ���� ������

	int ripeningPeriod = 0;       // ���� �Ⱓ (�ϼ�)
	int agingCount = 0;           // ���� Ƚ��
	int waterAbsorptionTime = 0;  // ���� �ӱ��� �ð�

	double evaporationRate = 0.0; // ���߷�
	double temperature = 0.0;     // �µ�
	double humidity = 0.0;        // ����

	bool roasted = false;         // �ν��� ����

public:
	OakBox() = default;

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
