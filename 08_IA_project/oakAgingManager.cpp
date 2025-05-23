#include "OakAgingManager.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>

using namespace std;

// === OakBox Ŭ���� ���� ===
// ��ũ�� ������ �����ϴ� Ŭ����
OakBox::OakBox(string id, string name, string t, string o, string wood,
    string spiritId, string startDate, string endDate,
    int period, int count, int waterTime,
    double evarate, double temp, double hum,
    bool roast)
    : boxId(id), name(name), type(t), origin(o), woodType(wood),
    spiritId(spiritId), agingStartDate(startDate), agingEndDate(endDate),
    ripeningPeriod(period), agingCount(count), waterAbsorptionTime(waterTime),
    evaporationRate(evarate), temperature(temp), humidity(hum),
    roasted(roast) {
}


// ��ũ�� ���� ���
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
    cout << "�̸�: " << name << endl;
    cout << "���Ǹ� ID: " << spiritId << endl;
    cout << "���� ������: " << agingStartDate << endl;
    cout << "���� ������: " << agingEndDate << endl;
}

// Getter �Լ� - ��� ���� ���� ��ȯ
string OakBox::getId() const { return boxId; }
string OakBox::getType() const { return type; }
string OakBox::getOrigin() const { return origin; }
string OakBox::getWoodType() const { return woodType; }
string OakBox::getName() const { return name; }
string OakBox::getSpiritId() const { return spiritId; }
string OakBox::getAgingStartDate() const { return agingStartDate; }
string OakBox::getAgingEndDate() const { return agingEndDate; }

int OakBox::getRipeningPeriod() const { return ripeningPeriod; }
int OakBox::getAgingCount() const { return agingCount; }
int OakBox::getWaterAbsorptionTime() const { return waterAbsorptionTime; }

double OakBox::getEvaporationRate() const { return evaporationRate; }
double OakBox::getTemperature() const { return temperature; }
double OakBox::getHumidity() const { return humidity; }

bool OakBox::isRoasted() const { return roasted; }


// Setter �Լ� - ��� ���� ���� ����
void OakBox::setOrigin(string o) { origin = o; }
void OakBox::setWoodType(string wood) { woodType = wood; }
void OakBox::setType(string t) { type = t; }
void OakBox::setId(string id) { boxId = id; }
void OakBox::setName(string n) { name = n; }
void OakBox::setSpiritId(string s) { spiritId = s; }
void OakBox::setAgingStartDate(string d) { agingStartDate = d; }
void OakBox::setAgingEndDate(string d) { agingEndDate = d; }

void OakBox::setAgingCount(int count) { agingCount = count; }
void OakBox::setWaterAbsorptionTime(int t) { waterAbsorptionTime = t; }
void OakBox::setRipeningPeriod(int p) { ripeningPeriod = p; }

void OakBox::setEvaporationRate(double e) { evaporationRate = e; }
void OakBox::setTemperature(double t) { temperature = t; }
void OakBox::setHumidity(double h) { humidity = h; }

void OakBox::setRoasted(bool r) { roasted = r; }


// ��ũ�� ���� �������� ǥ��
void OakAgingManager::showOakAgingPage() {
    int choice;

    do {
        system("cls");
        cout << "=== ��ũ�� ���� ���� �޴� ===\n\n";

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ��ũ�� ��� ����",
            "[2] ��ũ�� �߰�",
            "[3] ��ũ�� ����",
            "[4] ��ũ�� ����",
            "[5] CSV�� ����",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 5: exportOakBoxesToCSV("oak_boxes.csv"); break;
        case 0: cout << "�������� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}
// ��ũ�� ��� ���� ��ȯ
string OakAgingManager::getSummary() {
    // ����� ���� 2�� ����
    return "������ ��: 2��";
}

// ��ú��忡 ǥ���� ���� ����
vector<string> OakAgingManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = oakList.size();
    int totalPeriod = 0;
    double totalEvap = 0;

    // ��ũ���� ��� ���� �Ⱓ�� ���߷� ���
    for (const auto& box : oakList) {
        totalPeriod += box.getRipeningPeriod();
        totalEvap += box.getEvaporationRate();
    }

    lines.push_back("��ϵ� ��ũ�� ��: " + to_string(count));
    if (count > 0) {
        lines.push_back("��� ���� �Ⱓ: " + to_string(totalPeriod / count) + "��");
        lines.push_back("��� ���߷�: " + to_string((int)(totalEvap / count)) + "%");
    }
    else {
        lines.push_back("��� ���� �Ⱓ: -");
        lines.push_back("��� ���߷�: -");
    }

    return lines;
}

// ��ϵ� ��ũ�� ��� ���
void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "��ϵ� ��ũ���� �����ϴ�.\n";
        return;
    }
    for (const auto& oak : oakList) {
        oak.ShowInfo();
    }
}

// ���ο� ��ũ�� �߰�
void OakAgingManager::addOakBox() {
    string id, type, origin, wood;
    string name, spiritId, startDate, endDate;
    int period, count, water;
    int roastInt;
    double evap, temp, hum;
    bool roasted;

    cout << "1. ��ũ�� ID: "; cin >> id;
    cout << "2. ��ũ�� �̸�(��Ī): "; cin >> name;
    cout << "3. ����: "; cin >> type;
    cout << "4. ��� ����: "; cin >> origin;
    cout << "5. ���� �Ⱓ(��): "; cin >> period;
    cout << "6. ���� Ƚ��: "; cin >> count;
    cout << "7. ���߷�(%): "; cin >> evap;
    cout << "8. �µ�(��): "; cin >> temp;
    cout << "9. ����(%): "; cin >> hum;
    cout << "10. ���� �ӱ��� �ð�(�ð�): "; cin >> water;
    cout << "11. ���� ����: "; cin >> wood;
    cout << "12. ���Ǹ� ID: "; cin >> spiritId;
    cout << "13. ���� ������ (YYYY-MM-DD): "; cin >> startDate;
    cout << "14. ���� ������ (YYYY-MM-DD): "; cin >> endDate;
    cout << "15. �ν��� ���� (1: ����, 0: ����): "; cin >> roastInt;
    roasted = roastInt == 1;

    // ���ο� ��ũ�� ��ü ���� �� ��Ͽ� �߰�
    OakBox newBox(id, name, type, origin, wood, spiritId, startDate, endDate, period, count, water, evap, temp, hum, roasted);

    oakList.push_back(newBox);
    cout << "��ũ���� �߰��Ǿ����ϴ�!\n";
}

// ��ũ�� ���� ����
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
                cout << "[12] ��ũ�� �̸�\n";
                cout << "[13] ���Ǹ� ID\n";
                cout << "[14] ���� ������\n";
                cout << "[15] ���� ������\n";
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
                case 12: {
                    string n;
                    cout << "�� ��ũ�� �̸�: ";
                    cin >> n;
                    box.setName(n);
                    break;
                }
                case 13: {
                    string sid;
                    cout << "�� ���Ǹ� ID: ";
                    cin >> sid;
                    box.setSpiritId(sid);
                    break;
                }
                case 14: {
                    string date;
                    cout << "�� ���� ������ (YYYY-MM-DD): ";
                    cin >> date;
                    box.setAgingStartDate(date);
                    break;
                }
                case 15: {
                    string date;
                    cout << "�� ���� ������ (YYYY-MM-DD): ";
                    cin >> date;
                    box.setAgingEndDate(date);
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

// ��ũ�� ����
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

// ��ũ�� ������ CSV ���Ϸ� ��������
void OakAgingManager::exportOakBoxesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "CSV ������ �� �� �����ϴ�.\n";
        return;
    }

    file << "ID,Name,Type,Origin,WoodType,SpiritID,AgingStart,AgingEnd,"
        << "RipeningPeriod,AgingCount,WaterAbsorptionTime,"
        << "EvaporationRate,Temperature,Humidity,Roasted\n";

    for (const auto& b : oakList) {
        file << b.getId() << ","
            << b.getName() << ","
            << b.getType() << ","
            << b.getOrigin() << ","
            << b.getWoodType() << ","
            << b.getSpiritId() << ","
            << b.getAgingStartDate() << ","
            << b.getAgingEndDate() << ","
            << b.getRipeningPeriod() << ","
            << b.getAgingCount() << ","
            << b.getWaterAbsorptionTime() << ","
            << b.getEvaporationRate() << ","
            << b.getTemperature() << ","
            << b.getHumidity() << ","
            << (b.isRoasted() ? "Yes" : "No") << "\n";
    }

    file.close();
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
}