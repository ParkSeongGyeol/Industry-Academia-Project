#include "BottledWhiskyManager.h"
#include <iostream>

using namespace std;

// === BottledWhisky Ŭ���� ���� ===
BottledWhisky::BottledWhisky(string name, string label, string batch, string target, int count, double volume, double price, bool labeled)
    : productName(name), labelName(label), batchNumber(batch), exportTarget(target), bottleCount(count), totalVolume(volume), pricePerBottle(price), labeled(labeled) {}

void BottledWhisky::ShowInfo() const {
    cout << "��ǰ��: " << productName << endl;
    cout << "����: " << bottleCount << "��" << endl;
    cout << "�� �뷮: " << totalVolume << "L" << endl;
    cout << "���� ���� : " << pricePerBottle << "��" << endl;
    cout << "�� ���� ����: " << (labeled ? "O" : "X") << endl;
    cout << "�󺧸�: " << labelName << endl;
    cout << "��ġ ��ȣ: " << batchNumber << endl;
    cout << "��� ���: " << exportTarget << endl;
}

// getter
string BottledWhisky::getName() const { return productName; }
string BottledWhisky::getLabelName() const { return labelName; }
string BottledWhisky::getBatchNumber() const { return batchNumber; }
string BottledWhisky::getExportTarget() const { return exportTarget; }

int BottledWhisky::getBottleCount() const { return bottleCount; }

double BottledWhisky::getTotalVolume() const { return totalVolume; }
double BottledWhisky::getPricePerBottle() const { return pricePerBottle; }

bool BottledWhisky::isLabeled() const { return labeled; }

// setter
void BottledWhisky::setLabelName(string name) { labelName = name; }
void BottledWhisky::setBatchNumber(string num) { batchNumber = num; }
void BottledWhisky::setExportTarget(string target) { exportTarget = target; }
void BottledWhisky::setLabeled(bool value) { labeled = value; }

// ��� �� �� ���� �� �뷮�� ���ҽ�Ű�� �Լ�
void BottledWhisky::decreaseStock(int count) {
    if (bottleCount == 0) return;
    double perBottleVolume = totalVolume / bottleCount; // ���� �뷮
    bottleCount -= count;
    totalVolume -= perBottleVolume * count;
}


// === ShipmentRecord Ŭ���� ���� ===
ShipmentRecord::ShipmentRecord(string name, string date, int qty, double price)
    : productName(name), date(date), quantity(qty), totalPrice(price) {
}
// ��� ��� ���
void ShipmentRecord::ShowInfo() const {
    cout << "[�����] ��¥: " << date << endl;
    cout << "��ǰ��: " << productName << endl;
    cout << "��� ����: " << quantity << "��" << endl;
    cout << "�� ���� : " << totalPrice << "��" << endl;
}


// === BottledWhiskyManager Ŭ���� ���� ===
std::string BottledWhiskyManager::getSummary() {
    int totalCount = 0;
    double totalPrice = 0;

    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalPrice += w.getPricePerBottle();
    }

    std::string result = "����: " + std::to_string(totalCount) + "��";
    if (!inventory.empty()) {
        result += " / ��հ�: " + std::to_string(static_cast<int>(totalPrice / inventory.size())) + "��";
    }
    return result;
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
    string name, label, batch, target;
    int count;
    double volume, price;
    int labeledInt;

    cout << "\n[����Ű �߰�]\n";
    cout << "��ǰ��: "; cin >> name;
    cout << "�󺧸�: "; cin >> label;
    cout << "��ġ ��ȣ: "; cin >> batch;
    cout << "��� ���: "; cin >> target;
    cout << "����(��): "; cin >> count;
    cout << "�� �뷮(L): "; cin >> volume;
    cout << "���� ����: "; cin >> price;
    cout << "�� ���� ���� (1: O, 0: X): "; cin >> labeledInt;

    BottledWhisky newWhisky(name, label, batch, target, count, volume, price, labeledInt == 1);
    inventory.push_back(newWhisky);

    cout << "��ǰ�� �߰��Ǿ����ϴ�.\n";
}

void BottledWhiskyManager::updateWhisky() {
    string name;
    cout << "������ ��ǰ�� �Է�: ";
    cin >> name;

    for (auto& w : inventory) {
        if (w.getName() == name) {
            int choice;
            cout << "[1] �󺧸�" << endl;
            cout << "[2] ��ġ��ȣ" << endl;
            cout << "[3] �����" << endl;
            cout << "[4] �� ����\n����: ";
            cin >> choice;
            switch (choice) {
            case 1: { string label; cout << "�� �󺧸�: "; cin >> label; w.setLabelName(label); break; }
            case 2: { string batch; cout << "�� ��ġ��ȣ: "; cin >> batch; w.setBatchNumber(batch); break; }
            case 3: { string target; cout << "�� ��� ���: "; cin >> target; w.setExportTarget(target); break; }
            case 4: { int l; cout << "�� ���� (1: O, 0: X): "; cin >> l; w.setLabeled(l == 1); break; }
            default: cout << "�߸��� �����Դϴ�.\n";
            }
            return;
        }
    }
    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
}

void BottledWhiskyManager::deleteWhisky() {
    string name;
    cout << "������ ��ǰ�� �Է�: ";
    cin >> name;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getName() == name) {
            inventory.erase(it);
            cout << "�����Ǿ����ϴ�.\n";
            return;
        }
    }
    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
}

// ��� ��� ���� �� ��� ����
void BottledWhiskyManager::shipWhisky() {
    string name, date;
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
        cout << "5. ����ǰ ����\n";
        cout << "6. ����ǰ ����\n";
        cout << "0. ���� �޴��� ���ư���\n";
        cout << "����: ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n";
        }
    } while (choice != 0);
}
