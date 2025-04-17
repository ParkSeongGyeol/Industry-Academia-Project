#include "OakAgingManager.h"
#include <iostream>

using namespace std;

// 클래스 항목 헤더 파일로 이동

OakBox::OakBox(string id, string t, string o, string wood,
    int period, int count, int waterTime,
    double evarate, double temp, double hum,
    bool roast)
    : boxId(id), type(t), origin(o), woodType(wood), 
    ripeningPeriod(period), agingCount(count), waterAbsorptionTime(waterTime),
    evaporationRate(evarate), temperature(temp), humidity(hum),
    roasted(roast) {}


void OakBox::ShowInfo() const {
    cout << "오크통 ID : " << boxId << endl;
    cout << "종류 : " << type << endl;
    cout << "출신 지역 : " << origin << endl;
    cout << "숙성 기간: " << ripeningPeriod << "일" << endl;
    cout << "숙성 횟수: " << agingCount << "차 숙성" << endl;
    cout << "증발률: " << evaporationRate << "%" << endl;
    cout << "온도: " << temperature << "℃" << endl;
    cout << "습도: " << humidity << "%" << endl;
    cout << "물을 머금은 시간: " << waterAbsorptionTime << "시간" << endl;
    cout << "나무 종류: " << woodType << endl;
    cout << "로스팅 여부: " << (roasted ? "로스팅됨" : "로스팅 안됨") << endl;
}

// Getter / Setter 정의
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
        cout << "\n[오크통 숙성 관리 메뉴]\n";
        cout << "1. 오크통 목록 보기\n";
        cout << "2. 오크통 추가\n";
        cout << "3. 오크통 수정\n";
        cout << "4. 오크통 삭제\n";
        cout << "0. 뒤로 가기\n";
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n";
        }
    } while (choice != 0);
}


// OakAgingManager 기능 정의

std::string OakAgingManager::getSummary() {
    // 현재는 더미 2개 고정
    return "숙성통 수: 2개";
}


void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "등록된 오크통이 없습니다.\n";
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

    cout << "[오크통 추가]\n";
    cout << "1. 오크통 ID: "; cin >> id;
    cout << "2. 종류: "; cin >> type;
    cout << "3. 출신 지역: "; cin >> origin;
    cout << "4. 숙성 기간(일): "; cin >> period;
    cout << "5. 숙성 횟수: "; cin >> count;
    cout << "6. 증발률(%): "; cin >> evap;
    cout << "7. 온도(℃): "; cin >> temp;
    cout << "8. 습도(%): "; cin >> hum;
    cout << "9. 물을 머금은 시간(시간): "; cin >> water;
    cout << "10. 나무 종류: "; cin >> wood;
    int roastInt; // 입력 받고 확인하기 위한 int roastInt
    cout << "11. 로스팅 여부 (1: 있음, 0: 없음): "; cin >> roastInt;
    roasted = roastInt == 1;

    OakBox newBox(id, type, origin, wood, period, count, water, evap, temp, hum, roasted);
    oakList.push_back(newBox);
    cout << "오크통이 추가되었습니다!\n";
}

void OakAgingManager::updateOakBox() {
    string id;
    cout << "수정할 오크통 ID 입력: ";
    cin >> id;

    for (auto& box : oakList) {
        if (box.getId() == id) {
            while (true) {
                int choice;
                cout << "\n[수정할 항목 선택]\n";
                cout << "[1] 출신 지역\n";
                cout << "[2] 숙성 횟수\n";
                cout << "[3] 물 머금은 시간\n";
                cout << "[4] 로스팅 여부\n";
                cout << "[5] 나무 종류\n";
                cout << "[6] 종류\n";
                cout << "[7] 숙성 기간(일)\n";
                cout << "[8] 증발률(%)\n";
                cout << "[9] 온도(℃)\n";
                cout << "[10] 습도(%)\n";
                cout << "[11] 오크통 ID\n";
                cout << "[0] 수정을 완료하고 나가기\n";
                cout << "선택: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    string o;
                    cout << "새 출신 지역: ";
                    cin >> o;
                    box.setOrigin(o);
                    break;
                }
                case 2: {
                    int c;
                    cout << "새 숙성 횟수: ";
                    cin >> c;
                    box.setAgingCount(c);
                    break;
                }
                case 3: {
                    int t;
                    cout << "새 물 머금은 시간(시간): ";
                    cin >> t;
                    box.setWaterAbsorptionTime(t);
                    break;
                }
                case 4: {
                    int r;
                    cout << "로스팅 여부 (1: 있음, 0: 없음): ";
                    cin >> r;
                    box.setRoasted(r == 1);
                    break;
                }
                case 5: {
                    string w;
                    cout << "새 나무 종류: ";
                    cin >> w;
                    box.setWoodType(w);
                    break;
                }
                case 6: {
                    string t;
                    cout << "새 오크통 종류: ";
                    cin >> t;
                    box.setType(t);
                    break;
                }
                case 7: {
                    int p;
                    cout << "새 숙성 기간(일): ";
                    cin >> p;
                    box.setRipeningPeriod(p);
                    break;
                }
                case 8: {
                    double e;
                    cout << "새 증발률(%): ";
                    cin >> e;
                    box.setEvaporationRate(e);
                    break;
                }
                case 9: {
                    double temp;
                    cout << "새 온도(℃): ";
                    cin >> temp;
                    box.setTemperature(temp);
                    break;
                }
                case 10: {
                    double h;
                    cout << "새 습도(%): ";
                    cin >> h;
                    box.setHumidity(h);
                    break;
                }
                case 11: {
                    string newId;
                    cout << "새 오크통 ID: ";
                    cin >> newId;
                    box.setId(newId);
                    break;
                }
                case 0:
                    cout << "수정을 완료했습니다.\n";
                    return;
                default:
                    cout << "잘못된 선택입니다.\n";
                }
            }
        }
    }

    cout << "해당 ID를 가진 오크통이 없습니다.\n";
}


void OakAgingManager::deleteOakBox() {
    string id;
    cout << "삭제할 오크통 ID 입력: ";
    cin >> id;

    for (auto it = oakList.begin(); it != oakList.end(); ++it) {
        if (it->getId() == id) {
            oakList.erase(it);
            cout << "오크통이 삭제되었습니다.\n";
            return;
        }
    }
    cout << "해당 ID를 찾을 수 없습니다.\n";
}




