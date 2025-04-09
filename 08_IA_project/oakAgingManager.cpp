#include "OakAgingManager.h"
#include <iostream>

// Ŭ���� �׸� ��� ���Ϸ� �̵�

OakBox::OakBox(std::string id, std::string t, std::string o, int period, int count, double evarate, double temp, double hum, bool roast)
    : boxId(id), type(t), origin(o), ripeningPeriod(period), agingCount(count),
      evaporationRate(evarate), temperature(temp), humidity(hum), roasted(roast) {}

void OakBox::ShowInfo() const {
    std::cout << "��ũ�� ID : " << boxId << std::endl;
    std::cout << "���� : " << type << std::endl;
    std::cout << "��� ���� : " << origin << std::endl;
    std::cout << "���� �Ⱓ: " << ripeningPeriod << "��" << std::endl;
    std::cout << "���� Ƚ��: " << agingCount << "�� ����" << std::endl;
    std::cout << "���߷�: " << evaporationRate << "%" << std::endl;
    std::cout << "�µ�: " << temperature << "��" << std::endl;
    std::cout << "����: " << humidity << "%" << std::endl;
    std::cout << "�ν��� ����: " << (roasted ? "�ν��õ�" : "�ν��� �ȵ�") << std::endl;
}

void OakAgingManager::showOakList() {
    OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
    OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false);

    std::cout << "\n=== �������� ��ũ�� ���� ===\n\n";

    std::cout << "[1�� ��ũ��]\n";
    box1.ShowInfo();
    std::cout << "\n";

    std::cout << "[2�� ��ũ��]\n";
    box2.ShowInfo();
    std::cout << "\n";
}


// main �Լ��� �ϳ��� ����� ���� �۾�

//int main() {
//	OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
//	OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false); // ���� ��ũ�� 1���� 2��
//
//	cout << "�������� ��ũ�� ����" << endl << endl;
//
//	cout << "1�� ��ũ��" << endl;
//	box1.ShowInfo();
//	cout << endl;
//
//	cout << "2�� ��ũ��" << endl;
//	box2.ShowInfo();
//	cout << endl;
//
//	return 0;
//}


