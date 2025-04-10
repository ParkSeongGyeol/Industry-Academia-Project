#include "BottledWhiskyManager.h"
#include <iostream>

using namespace std;

// === BottledWhisky Ŭ���� ���� ===
BottledWhisky::BottledWhisky(string name, int count, double volume, double price)
    : productName(name), bottleCount(count), totalVolume(volume), pricePerBottle(price) {
}

void BottledWhisky::ShowInfo() const {
    cout << "��ǰ��: " << productName << ", ����: " << bottleCount << "��"
        << ", �� �뷮: " << totalVolume << "L, ���� ����: " << pricePerBottle << "��" << endl;
}

// getter
string BottledWhisky::getName() const { return productName; }
int BottledWhisky::getBottleCount() const { return bottleCount; }
double BottledWhisky::getTotalVolume() const { return totalVolume; }
double BottledWhisky::getPricePerBottle() const { return pricePerBottle; }

// ��� �� �� ���� �� �뷮�� ���ҽ�Ű�� �Լ�
void BottledWhisky::decreaseStock(int count) {
    if (bottleCount == 0) return;
    double perBottleVolume = totalVolume / bottleCount; // ���� �뷮
    bottleCount -= count;
    totalVolume -= perBottleVolume * count;
}

ShipmentRecord::ShipmentRecord(string name, string date, int qty, double price)
    : productName(name), date(date), quantity(qty), totalPrice(price) {
}
// ��� ��� ���
void ShipmentRecord::ShowInfo() const {
    cout << "[�����] ��¥: " << date << ", ��ǰ��: " << productName
        << ", ��� ����: " << quantity << "��, �� ����: " << totalPrice << "��" << endl;
}

// ��ü ��� ���
void BottledWhiskyManager::showInventory() {
    if (inventory.empty()) {
        cout << "���� ��� �����ϴ�.\n";
        return;
    }
    cout << "\n[���� ����Ű ��� ���]\n";
    for (const auto& whisky : inventory) {
        whisky.ShowInfo();
    }
}

// �� ��ǰ �߰�
void BottledWhiskyManager::addWhisky() {
    string name;
    int count;
    double volume, price;

    cout << "\n[����Ű �߰�]\n";
    cout << "��ǰ��: "; cin >> name;
    cout << "����(��): "; cin >> count;
    cout << "�� �뷮(L): "; cin >> volume;
    cout << "���� ����: "; cin >> price;

    BottledWhisky newWhisky(name, count, volume, price);
    inventory.push_back(newWhisky);

    cout << "��ǰ�� �߰��Ǿ����ϴ�.\n";
}
// ��� ��� ���� �� ��� ����
void BottledWhiskyManager::shipWhisky() {
    string name, buyer, date;
    int qty;

    cout << "\n[��� ���]\n";
    cout << "����� ��ǰ��: "; cin >> name;

    for (auto& whisky : inventory) {
        if (whisky.getName() == name) {
            cout << "��� ����(��): "; cin >> qty;
            if (qty > whisky.getBottleCount()) {
                cout << "��� ������ ����� �����ϴ�.\n";
                return;
            }

            cout << "��� ��¥ (YYYY-MM-DD): "; cin >> date;

            double totalPrice = qty * whisky.getPricePerBottle();
            ShipmentRecord record(name, date, qty, totalPrice);
            shipmentLog.push_back(record);

            whisky.decreaseStock(qty);

            cout << "��� ����� ��ϵǾ����ϴ�.\n";
            return;
        }
    }

    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
}
// ��� ��� ���
void BottledWhiskyManager::showShipmentLog() {
    if (shipmentLog.empty()) {
        cout << "��� ����� �����ϴ�.\n";
        return;
    }

    cout << "\n[��� ��� ���]\n";
    for (const auto& record : shipmentLog) {
        record.ShowInfo();
    }
}

// ���� ����Ű ���� �޴� ����
void BottledWhiskyManager::BottledListRun() {
    int choice;

    do {
        cout << "\n[���� �� �ϼ�ǰ ���� �޴�]\n";
        cout << "1. ��� ��� ����\n";
        cout << "2. ��ǰ �߰�\n";
        cout << "3. ��ǰ ���\n";
        cout << "4. ��� ��� ����\n";
        cout << "0. ���� �޴��� ���ư���\n";
        cout << "����: ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n";
        }
    } while (choice != 0);
}
