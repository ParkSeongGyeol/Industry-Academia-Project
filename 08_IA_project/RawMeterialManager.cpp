#include "RawMaterialManager.h"
#include <iostream>
#include "UIUtils.h"

using namespace std;

std::string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.exit_date.empty()) // ������ ���� ���
            totalKg += item.weight_kg;

    return "�����: " + to_string(materials.size()) + "�� / " + to_string((int)totalKg) + "kg";
}

void RawMaterialManager::initializeDummyData() {
    materials = {
        {"����", "�", "����Ʋ����", 1200, "â�� A", "���� ����", "2025-12-01", "2025-03-01", ""},
        {"ȣ��", "�", "�̱�", 800, "â�� A", "��� ����", "2025-11-15", "2025-03-03", ""},
        {"��", "��Ÿ", "���� ����", 5000, "��ũ B", "û�� �к�", "-", "2025-03-02", ""},
        {"����", "�", "����Ʋ����", 900, "â�� A", "���� ����", "2025-10-10", "2025-01-10", "2025-03-01"}
    };
}

void RawMaterialManager::showInventory() {
    cout << "\n=== ���� ���� ����� ��� ===\n";
    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            cout << "�̸�: " << m.name << "\n"
                << "����: " << m.type << "\n"
                << "���: " << m.origin << "\n"
                << "���: " << m.weight_kg << "kg\n"
                << "���� ��ġ: " << m.storage_location << "\n"
                << "���� ���: " << m.storage_method << "\n"
                << "�������: " << m.expiry_date << "\n"
                << "�԰���: " << m.entry_date << "\n"
                << "-----------------------------\n";
        }
    }
}

void RawMaterialManager::showAllMaterials() {
    cout << "\n=== ��ü ����� ����� �̷� ===\n";
    for (const auto& m : materials) {
        cout << "�̸�: " << m.name << "\n"
            << "����: " << m.type << "\n"
            << "���: " << m.origin << "\n"
            << "����: " << m.weight_kg << "kg\n"
            << "���� ��ġ: " << m.storage_location << "\n"
            << "���� ���: " << m.storage_method << "\n"
            << "�������: " << m.expiry_date << "\n"
            << "�԰���: " << m.entry_date << "\n"
            << "�����: " << (m.exit_date.empty() ? "-" : m.exit_date) << "\n"
            << "-----------------------------\n";
    }
}

void RawMaterialManager::showRawMaterialPage() {
    initializeDummyData();

    int choice;
    do {
        system("cls");
        cout << "=== ����� ���� �޴� ===\n\n";

        vector<string> infoLines = { getSummary() };
        vector<string> menu = {
            "[1] ���� ���� ��� ����",
            "[2] ��ü �̷� ����",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: showAllMaterials(); break;
        case 0: cout << "�������� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}
