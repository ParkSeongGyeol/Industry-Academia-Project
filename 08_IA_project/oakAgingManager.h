#pragma once
#include <string>

class OakBox {
private:
    std::string boxId; // ��ũ�� ���� ���̵�
	std::string type; // ����
	std::string origin; // �������
	int ripeningPeriod; // ���� �Ⱓ
	int agingCount; // ���� Ƚ��
	double evaporationRate; // ���߷�
	double temperature; // �µ�
	double humidity; // ����
	bool roasted; // �ν��� ����

public:
    OakBox(std::string id, std::string t, std::string o, int period, int count, double evarate, double temp, double hum, bool roast);
    void ShowInfo() const;
};

class OakAgingManager {
public:
    void showOakList();
};
