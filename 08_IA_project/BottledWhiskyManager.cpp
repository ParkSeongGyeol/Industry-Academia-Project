#include "BottledWhiskyManager.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>

using namespace std;

// === BottledWhisky Ŭ���� ���� ===
BottledWhisky::BottledWhisky(
    string productId, string name, string label, string batch, string target,
    string oakId, string shipmentDate, string serialNumber, string bottlingManager,
    int count,
    double volume, double price,
    bool labeled)
    : productId(productId), productName(name), labelName(label), batchNumber(batch),
    exportTarget(target), oakId(oakId), shipmentDate(shipmentDate),
    serialNumber(serialNumber), bottlingManager(bottlingManager),
    bottleCount(count), totalVolume(volume), pricePerBottle(price), labeled(labeled) {
}


//����Ű ���� ���
void BottledWhisky::ShowInfo() const {
    cout << "��ǰ ID: " << productId << endl;
    cout << "��ǰ��: " << productName << endl;
    cout << "����: " << bottleCount << "��" << endl;
    cout << "�� �뷮: " << totalVolume << "L" << endl;
    cout << "���� ���� : " << pricePerBottle << "��" << endl;
    cout << "�� ���� ����: " << (labeled ? "O" : "X") << endl;
    cout << "�󺧸�: " << labelName << endl;
    cout << "��ġ ��ȣ: " << batchNumber << endl;
    cout << "��� ���: " << exportTarget << endl;
    cout << "��ũ�� ID: " << oakId << endl;
    cout << "��� ����: " << shipmentDate << endl;
    cout << "���� ��ȣ: " << serialNumber << endl;
    cout << "���� �����: " << bottlingManager << endl;
}


// getter �Լ� - ��� ���� ���� ��ȯ
string BottledWhisky::getName() const { return productName; }
string BottledWhisky::getLabelName() const { return labelName; }
string BottledWhisky::getBatchNumber() const { return batchNumber; }
string BottledWhisky::getExportTarget() const { return exportTarget; }
string BottledWhisky::getProductId() const { return productId; }
string BottledWhisky::getOakId() const { return oakId; }
string BottledWhisky::getShipmentDate() const { return shipmentDate; }
string BottledWhisky::getSerialNumber() const { return serialNumber; }
string BottledWhisky::getBottlingManager() const { return bottlingManager; }

int BottledWhisky::getBottleCount() const { return bottleCount; }

double BottledWhisky::getTotalVolume() const { return totalVolume; }
double BottledWhisky::getPricePerBottle() const { return pricePerBottle; }

bool BottledWhisky::isLabeled() const { return labeled; }

// setter �Լ� - ��� ���� ���� ����
void BottledWhisky::setLabelName(string name) { labelName = name; }
void BottledWhisky::setBatchNumber(string num) { batchNumber = num; }
void BottledWhisky::setExportTarget(string target) { exportTarget = target; }
void BottledWhisky::setName(string name) { productName = name; }
void BottledWhisky::setProductId(string id) { productId = id; }
void BottledWhisky::setOakId(string id) { oakId = id; }
void BottledWhisky::setShipmentDate(string date) { shipmentDate = date; }
void BottledWhisky::setSerialNumber(string serial) { serialNumber = serial; }
void BottledWhisky::setBottlingManager(string manager) { bottlingManager = manager; }

void BottledWhisky::setBottleCount(int count) { bottleCount = count;}

void BottledWhisky::setTotalVolume(double volume) { totalVolume = volume; }
void BottledWhisky::setPricePerBottle(double price) { pricePerBottle = price; }

void BottledWhisky::setLabeled(bool value) { labeled = value; }



// === ShipmentRecord Ŭ���� ���� ===
// ��� �� �� ���� �� �뷮�� ���ҽ�Ű�� �Լ�
void BottledWhisky::decreaseStock(int count) {
    if (bottleCount == 0) return; // ��� ������ ����
    double perBottleVolume = totalVolume / bottleCount; // ���� �뷮
    bottleCount -= count; 
    totalVolume -= perBottleVolume * count;
}


// ��� ����� �����ϴ� Ŭ����
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
// ���� ����Ű�� �����ϴ� Ŭ����
string BottledWhiskyManager::getSummary() {
    int totalCount = 0;
    double totalPrice = 0;

    // ��ü �� ���� ��� ���� ���
    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalPrice += w.getPricePerBottle();
    }

    string result = "����: " + to_string(totalCount) + "��";
    if (!inventory.empty()) {
        result += " / ��հ�: " + to_string(static_cast<int>(totalPrice / inventory.size())) + "��";
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
    string id, name, label, batch, target;
    string oakId, date, serial, manager;
    int count;
    double volume, price;
    int labeledInt;

    cout << "\n[����Ű �߰�]\n";
    cout << "��ǰ ID: "; cin >> id;
    cout << "��ǰ��: "; cin >> name;
    cout << "�󺧸�: "; cin >> label;
    cout << "��ġ ��ȣ: "; cin >> batch;
    cout << "��� ���: "; cin >> target;
    cout << "��ũ�� ID: "; cin >> oakId;
    cout << "��� ���� (YYYY-MM-DD): "; cin >> date;
    cout << "���� ��ȣ: "; cin >> serial;
    cout << "���� �����: "; cin >> manager;
    cout << "����(��): "; cin >> count;
    cout << "�� �뷮(L): "; cin >> volume;
    cout << "���� ����: "; cin >> price;
    cout << "�� ���� ���� (1: O, 0: X): "; cin >> labeledInt;


	// ���ο� ����Ű ��ü ���� �� ��� �߰�
    BottledWhisky newWhisky(id, name, label, batch, target, oakId, date, serial, manager, count, volume, price, labeledInt == 1);

    inventory.push_back(newWhisky);

    cout << "��ǰ�� �߰��Ǿ����ϴ�.\n";
}

// ��ǰ ���� ����
void BottledWhiskyManager::updateWhisky() {
    string name;
    cout << "������ ��ǰ�� �Է�: ";
    cin >> name;

    for (auto& w : inventory) {
        if (w.getName() == name) {
            while (true) {
                int choice;
                cout << "\n[������ �׸� ����]\n";
                cout << "[1] ��ǰ��\n";
                cout << "[2] �󺧸�\n";
                cout << "[3] ��ġ��ȣ\n";
                cout << "[4] �����\n";
                cout << "[5] ����(��)\n";
                cout << "[6] �� �뷮(L)\n";
                cout << "[7] ���� ����\n";
                cout << "[8] �� ����\n";
                cout << "[9] ��ũ�� ID\n";
                cout << "[10] ��� ����\n";
                cout << "[11] ���� ��ȣ\n";
                cout << "[12] ���� �����\n";
                cout << "[13] ��ǰ ID\n";
                cout << "[0] ������ �Ϸ��ϰ� ������\n";
                cout << "����: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    string newName;
                    cout << "�� ��ǰ��: ";
                    cin >> newName;
                    w.setName(newName);
                    break;
                }
                case 2: {
                    string label;
                    cout << "�� �󺧸�: ";
                    cin >> label;
                    w.setLabelName(label);
                    break;
                }
                case 3: {
                    string batch;
                    cout << "�� ��ġ��ȣ: ";
                    cin >> batch;
                    w.setBatchNumber(batch);
                    break;
                }
                case 4: {
                    string target;
                    cout << "�� ��� ���: ";
                    cin >> target;
                    w.setExportTarget(target);
                    break;
                }
                case 5: {
                    int count;
                    cout << "�� ����(��): ";
                    cin >> count;
                    w.setBottleCount(count);
                    break;
                }
                case 6: {
                    double volume;
                    cout << "�� �� �뷮(L): ";
                    cin >> volume;
                    w.setTotalVolume(volume);
                    break;
                }
                case 7: {
                    double price;
                    cout << "�� ���� ����: ";
                    cin >> price;
                    w.setPricePerBottle(price);
                    break;
                }
                case 8: {
                    int l;
                    cout << "�� ���� (1: O, 0: X): ";
                    cin >> l;
                    w.setLabeled(l == 1);
                    break;
                }
                case 9: {
                    string oak;
                    cout << "�� ��ũ�� ID: ";
                    cin >> oak;
                    w.setOakId(oak);
                    break;
                }
                case 10: {
                    string date;
                    cout << "�� ��� ���� (YYYY-MM-DD): ";
                    cin >> date;
                    w.setShipmentDate(date);
                    break;
                }
                case 11: {
                    string serial;
                    cout << "�� ���� ��ȣ: ";
                    cin >> serial;
                    w.setSerialNumber(serial);
                    break;
                }
                case 12: {
                    string manager;
                    cout << "�� ���� �����: ";
                    cin >> manager;
                    w.setBottlingManager(manager);
                    break;
                }
                case 13: {
                    string pid;
                    cout << "�� ��ǰ ID: ";
                    cin >> pid;
                    w.setProductId(pid);
                    break;
                }
                case 0:
                    cout << "������ �Ϸ��߽��ϴ�.\n";
                    return;
                default:
                    cout << "�߸��� �����Դϴ�.\n";
                }
            }
        }
    }

    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
}

// ��ǰ ����
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

            //��� ��� ����
            ShipmentRecord record(name, date, qty, totalPrice);
            shipmentLog.push_back(record);

            // ��� ����
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
void BottledWhiskyManager::showBottledWhiskyPage() {
    int choice;

    do {
		system("cls"); // ȭ�� �����
		// ��ú��� ���
		// ���� �� �ϼ�ǰ ���� ������
		cout << "=== ���� �� �ϼ�ǰ ���� �޴� ===\n\n";

        // ���� ���� ���� (infoLines)
        vector<string> infoLines;
        int totalCount = 0;
        double totalVolume = 0, totalValue = 0;

        for (const auto& w : inventory) {
            totalCount += w.getBottleCount();
            totalVolume += w.getTotalVolume();
            totalValue += w.getBottleCount() * w.getPricePerBottle();
        }

        infoLines.push_back("��ü �� ����: " + to_string(totalCount) + "��");
        infoLines.push_back("��ü �뷮: " + to_string((int)totalVolume) + "L");
        if (totalCount > 0) {
            infoLines.push_back("��� ���� ����: " + to_string((int)(totalValue / totalCount)) + "��");
        }
        else {
            infoLines.push_back("��� ���� ����: -");
        }

        // ���� �޴� ����
        vector<string> menu = {
            "[1] ��� ��� ����",
            "[2] ��ǰ �߰�",
            "[3] ��ǰ ���",
            "[4] ��� ��� ����",
            "[5] ����ǰ ����",
            "[6] ����ǰ ����",
            "[7] CSV�� ����",
            "[0] ���� �޴��� ���ư���"
        };

        // ��ú��� ���
        UIUtils::drawDashboard(infoLines, menu, 72, 30);

        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 7: exportInventoryToCSV("bottled_inventory.csv"); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}

// ���� ����Ű ��� CSV ���Ϸ� ��������
void BottledWhiskyManager::exportInventoryToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "CSV ������ �� �� �����ϴ�.\n";
        return;
    }

    // CSV ���
    file << "ProductID,ProductName,LabelName,BatchNumber,ExportTarget,"
        << "OakID,ShipmentDate,SerialNumber,BottlingManager,"
        << "BottleCount,TotalVolume,PricePerBottle,Labeled\n";

    for (const auto& w : inventory) {
        file << w.getProductId() << ","
            << w.getName() << ","
            << w.getLabelName() << ","
            << w.getBatchNumber() << ","
            << w.getExportTarget() << ","
            << w.getOakId() << ","
            << w.getShipmentDate() << ","
            << w.getSerialNumber() << ","
            << w.getBottlingManager() << ","
            << w.getBottleCount() << ","
            << w.getTotalVolume() << ","
            << w.getPricePerBottle() << ","
            << (w.isLabeled() ? "Yes" : "No") << "\n";
    }

    file.close();
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
}
