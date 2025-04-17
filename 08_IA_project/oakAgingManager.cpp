#include "OakAgingManager.h"
#include <iostream>

using namespace std;

// Ŭ���� �׸� ��� ���Ϸ� �̵�

OakBox::OakBox(string id, string t, string o, string wood,
    int period, int count, int waterTime,
    double evarate, double temp, double hum,
    bool roast)
    : boxId(id), type(t), origin(o), woodType(wood), 
    ripeningPeriod(period), agingCount(count), waterAbsorptionTime(waterTime),
    evaporationRate(evarate), temperature(temp), humidity(hum),
    roasted(roast) {}


void OakBox::ShowInfo() const {
    cout << "��ũ�� ID : " << boxId << endl;
    cout << "���� : " << type << endl;
    cout << "��� ���� : " << origin << endl;
    cout << "���� �Ⱓ: " << ripeningPeriod << "��" << endl;
    cout << "���� Ƚ��: " << agingCount << "�� ����" << endl;
    cout << "���߷�: " << evaporationRate << "%" << endl;
    cout << "�µ�: " << temperature << "��" << endl;
    cout << "����: " << humidity << "%" << endl;
    cout << "���� �ӱ��� �ð�: " << waterAbsorptionTime << "�ð�" << endl;
    cout << "���� ����: " << woodType << endl;
    cout << "�ν��� ����: " << (roasted ? "�ν��õ�" : "�ν��� �ȵ�") << endl;
}

// Getter / Setter ����
string OakBox::getId() const { return boxId; }

void OakBox::setOrigin(string o) { origin = o; }
void OakBox::setWoodType(string wood) { woodType = wood; }
void OakBox::setType(string t) { type = t; }
void OakBox::setId(string id) { boxId = id; }

void OakBox::setAgingCount(int count) { agingCount = count; }
void OakBox::setWaterAbsorptionTime(int t) { waterAbsorptionTime = t; }
void OakBox::setRipeningPeriod(int p) { ripeningPeriod = p; }

void OakBox::setEvaporationRate(double e) { evaporationRate = e; }
void OakBox::setTemperature(double t) { temperature = t; }
void OakBox::setHumidity(double h) { humidity = h; }

void OakBox::setRoasted(bool r) { roasted = r; }





void OakAgingManager::showOakAgingPage() {
    int choice;

    do {
        cout << "\n[��ũ�� ���� ���� �޴�]\n";
        cout << "1. ��ũ�� ��� ����\n";
        cout << "2. ��ũ�� �߰�\n";
        cout << "3. ��ũ�� ����\n";
        cout << "4. ��ũ�� ����\n";
        cout << "0. �ڷ� ����\n";
        cout << "����: ";
        cin >> choice;

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n";
        }
    } while (choice != 0);
}


// OakAgingManager ��� ����

std::string OakAgingManager::getSummary() {
    // ����� ���� 2�� ����
    return "������ ��: 2��";
}


void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "��ϵ� ��ũ���� �����ϴ�.\n";
        return;
    }
    for (const auto& oak : oakList) {
        oak.ShowInfo();
    }
}

void OakAgingManager::addOakBox() {
    string id, type, origin, wood;
    int period, count, water;
    double evap, temp, hum;
    bool roasted;

    cout << "[��ũ�� �߰�]\n";
    cout << "1. ��ũ�� ID: "; cin >> id;
    cout << "2. ����: "; cin >> type;
    cout << "3. ��� ����: "; cin >> origin;
    cout << "4. ���� �Ⱓ(��): "; cin >> period;
    cout << "5. ���� Ƚ��: "; cin >> count;
    cout << "6. ���߷�(%): "; cin >> evap;
    cout << "7. �µ�(��): "; cin >> temp;
    cout << "8. ����(%): "; cin >> hum;
    cout << "9. ���� �ӱ��� �ð�(�ð�): "; cin >> water;
    cout << "10. ���� ����: "; cin >> wood;
    int roastInt; // �Է� �ް� Ȯ���ϱ� ���� int roastInt
    cout << "11. �ν��� ���� (1: ����, 0: ����): "; cin >> roastInt;
    roasted = roastInt == 1;

    OakBox newBox(id, type, origin, wood, period, count, water, evap, temp, hum, roasted);
    oakList.push_back(newBox);
    cout << "��ũ���� �߰��Ǿ����ϴ�!\n";
}

void OakAgingManager::updateOakBox() {
    string id;
    cout << "������ ��ũ�� ID �Է�: ";
    cin >> id;

    for (auto& box : oakList) {
        if (box.getId() == id) {
            while (true) {
                int choice;
                cout << "\n[������ �׸� ����]\n";
                cout << "[1] ��� ����\n";
                cout << "[2] ���� Ƚ��\n";
                cout << "[3] �� �ӱ��� �ð�\n";
                cout << "[4] �ν��� ����\n";
                cout << "[5] ���� ����\n";
                cout << "[6] ����\n";
                cout << "[7] ���� �Ⱓ(��)\n";
                cout << "[8] ���߷�(%)\n";
                cout << "[9] �µ�(��)\n";
                cout << "[10] ����(%)\n";
                cout << "[11] ��ũ�� ID\n";
                cout << "[0] ������ �Ϸ��ϰ� ������\n";
                cout << "����: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    string o;
                    cout << "�� ��� ����: ";
                    cin >> o;
                    box.setOrigin(o);
                    break;
                }
                case 2: {
                    int c;
                    cout << "�� ���� Ƚ��: ";
                    cin >> c;
                    box.setAgingCount(c);
                    break;
                }
                case 3: {
                    int t;
                    cout << "�� �� �ӱ��� �ð�(�ð�): ";
                    cin >> t;
                    box.setWaterAbsorptionTime(t);
                    break;
                }
                case 4: {
                    int r;
                    cout << "�ν��� ���� (1: ����, 0: ����): ";
                    cin >> r;
                    box.setRoasted(r == 1);
                    break;
                }
                case 5: {
                    string w;
                    cout << "�� ���� ����: ";
                    cin >> w;
                    box.setWoodType(w);
                    break;
                }
                case 6: {
                    string t;
                    cout << "�� ��ũ�� ����: ";
                    cin >> t;
                    box.setType(t);
                    break;
                }
                case 7: {
                    int p;
                    cout << "�� ���� �Ⱓ(��): ";
                    cin >> p;
                    box.setRipeningPeriod(p);
                    break;
                }
                case 8: {
                    double e;
                    cout << "�� ���߷�(%): ";
                    cin >> e;
                    box.setEvaporationRate(e);
                    break;
                }
                case 9: {
                    double temp;
                    cout << "�� �µ�(��): ";
                    cin >> temp;
                    box.setTemperature(temp);
                    break;
                }
                case 10: {
                    double h;
                    cout << "�� ����(%): ";
                    cin >> h;
                    box.setHumidity(h);
                    break;
                }
                case 11: {
                    string newId;
                    cout << "�� ��ũ�� ID: ";
                    cin >> newId;
                    box.setId(newId);
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

    cout << "�ش� ID�� ���� ��ũ���� �����ϴ�.\n";
}


void OakAgingManager::deleteOakBox() {
    string id;
    cout << "������ ��ũ�� ID �Է�: ";
    cin >> id;

    for (auto it = oakList.begin(); it != oakList.end(); ++it) {
        if (it->getId() == id) {
            oakList.erase(it);
            cout << "��ũ���� �����Ǿ����ϴ�.\n";
            return;
        }
    }
    cout << "�ش� ID�� ã�� �� �����ϴ�.\n";
}




