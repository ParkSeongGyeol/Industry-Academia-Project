#include "RawMaterialManager.h"
#include "UIUtils.h"
#include "StorageEnvironment.h"
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>

using namespace std;

std::string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.exit_date.empty())
            totalKg += item.weight_kg;

    return "�����: " + to_string(materials.size()) + "�� / " + to_string((int)totalKg) + "kg";
}

std::vector<std::string> RawMaterialManager::getPageInfoLines() {
    int totalKinds = 0;
    double totalWeight = 0;
    std::set<std::string> storagePlaces;
    std::map<std::string, int> typeCount;

    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            ++totalKinds;
            totalWeight += m.weight_kg;
            storagePlaces.insert(m.storage_location);
            typeCount[m.type]++;
        }
    }

    std::vector<std::string> lines;
    lines.push_back("���� ���� �����: " + std::to_string(totalKinds) + "�� / " + std::to_string((int)totalWeight) + "kg");

    std::string typeSummary = "������ ����: ";
    for (const auto& pair : typeCount) {
        typeSummary += pair.first + ": " + std::to_string(pair.second) + "��  ";
    }
    lines.push_back(typeSummary);

    std::string storageSummary = "���� ��ġ: ";
    for (const auto& place : storagePlaces) {
        storageSummary += place + "  ";
    }
    lines.push_back(storageSummary);

    return lines;
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

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ���� ���� ��� ����",
            "[2] ��ü �̷� ����",
            "[3] ����� �߰�",
            "[4] ����� ���� ����",
            "[5] ����� ���� (��� ó��)",
            "[6] ����� �˻�",
            "[7] ���� ��� ȯ������ ����",  
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: showAllMaterials(); break;
        case 3: addMaterial(); break;
        case 4: updateMaterial(); break;
        case 5: deleteMaterial(); break;
        case 6: searchMaterial(); break;
        case 7: showStorageEnvironment(); break;  
        case 0: cout << "�������� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}

void RawMaterialManager::addMaterial() {
    RawMaterial newItem;
    cin.ignore(); // ���� �ʱ�ȭ
    cout << "\n=== ����� �߰� ===\n";
    cout << "�̸�: "; getline(cin, newItem.name);
    cout << "����: "; getline(cin, newItem.type);
    cout << "��� ����: "; getline(cin, newItem.origin);
    cout << "����(kg): "; cin >> newItem.weight_kg; cin.ignore();
    cout << "���� ��ġ: "; getline(cin, newItem.storage_location);
    cout << "���� ���: "; getline(cin, newItem.storage_method);
    cout << "�������: "; getline(cin, newItem.expiry_date);
    cout << "�԰���(YYYY-MM-DD): "; getline(cin, newItem.entry_date);
    newItem.exit_date = "";

    materials.push_back(newItem);
    cout << "����ᰡ �߰��Ǿ����ϴ�.\n";
}

void RawMaterialManager::updateMaterial() {
    string name;
    cin.ignore();
    cout << "\n������ ����� �̸� �Է�: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.name == name && m.exit_date.empty()) {
            cout << "=== ����� ���� ===\n";

            cout << "���� ��ġ (" << m.storage_location << "): ";
            getline(cin, m.storage_location);

            cout << "���� ��� (" << m.storage_method << "): ";
            getline(cin, m.storage_method);

            cout << "������� (" << m.expiry_date << "): ";
            getline(cin, m.expiry_date);

            cout << "����(kg) (" << m.weight_kg << "): ";
            cin >> m.weight_kg; cin.ignore();

            cout << "���� �Ϸ�.\n";
            return;
        }
    }
    cout << "�ش� �̸��� �����(���)�� ã�� �� �����ϴ�.\n";
}

void RawMaterialManager::deleteMaterial() {
    string name;
    cin.ignore();
    cout << "\n��� ó���� ����� �̸� �Է�: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.name == name && m.exit_date.empty()) {
            cout << "����� �Է� (YYYY-MM-DD): ";
            getline(cin, m.exit_date);
            cout << "��� ó�� �Ϸ�.\n";
            return;
        }
    }

    cout << "�ش� �̸��� �����(���)�� ã�� �� �����ϴ�.\n";
}

void RawMaterialManager::searchMaterial() {
    string name;
    cin.ignore();
    cout << "\n�˻��� ����� �̸� �Է�: ";
    getline(cin, name);

    bool found = false;
    for (const auto& m : materials) {
        if (m.name == name) {
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
            found = true;
        }
    }

    if (!found) {
        cout << "�ش� �̸��� ����Ḧ ã�� �� �����ϴ�.\n";
    }
}

void RawMaterialManager::showStorageEnvironment() {
    std::vector<StorageEnvironment> storageList = {
        {"â�� A", 18.5f, 55.2f},
        {"���� �����", 12.0f, 70.0f},
        {"����� ������", 22.3f, 40.0f}
    };

    std::cout << "\n=== ���� ��� ȯ�� ���� ��ȸ ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }

    std::cout << "\n����Ϸ��� Enter�� ��������...";
    std::cin.ignore(); std::cin.get();
}
