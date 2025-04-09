#include <iostream>
#include <string>
using namespace std;


class OakBox {
private:
	string boxId; // ��ũ�� ���� ���̵�
	string type; // ����
	string origin; // �������
	int ripeningPeriod; // ���� �Ⱓ
	int agingCount; // ���� Ƚ��
	double evaporationRate; // ���߷�
	double temperature; // �µ�
	double humidity; // ����
	bool roasted; // �ν��� ����

public:
	OakBox(string id, string t, string o, int period, int count, double evarate, double temp, double hum, bool roast)
		: boxId(id), type(t), origin(o), ripeningPeriod(period), agingCount(count), evaporationRate(evarate), temperature(temp), humidity(hum), roasted(roast) {
	} // ������

	void ShowInfo() const {
		cout << "��ũ�� ID : " << boxId << endl;
		cout << "���� : " << type << endl;
		cout << "��� ���� : " << origin << endl;
		cout << "���� �Ⱓ: " << ripeningPeriod << "��" << endl;
		cout << "���� Ƚ��: " << agingCount << "�� ����" << endl;
		cout << "���߷�: " << evaporationRate << "%" << endl;
		cout << "�µ�: " << temperature << "��" << endl;
		cout << "����: " << humidity << "%" << endl;
		cout << "�ν��� ����: " << (roasted ? "�ν��õ�" : "�ν��� �ȵ�") << endl;

	}
};

int main() {
	OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
	OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false); // ���� ��ũ�� 1���� 2��

	cout << "�������� ��ũ�� ����" << endl << endl;

	cout << "1�� ��ũ��" << endl;
	box1.ShowInfo();
	cout << endl;

	cout << "2�� ��ũ��" << endl;
	box2.ShowInfo();
	cout << endl;

	return 0;
}