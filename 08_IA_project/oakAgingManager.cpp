#include "OakAgingManager.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>

using namespace std;


// 오크통 정보 출력
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
    cout << "이름: " << name << endl;
    cout << "스피릿 ID: " << spiritId << endl;
    cout << "숙성 시작일: " << agingStartDate << endl;
    cout << "숙성 종료일: " << agingEndDate << endl;
}

// Getter 함수 - 멤버 변수 값을 반환
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


// Setter 함수 - 멤버 변수 값을 설정
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


// 오크통 관리 페이지를 표시
void OakAgingManager::showOakAgingPage() {
    int choice;

    do {
        system("cls");
        cout << "=== 오크통 숙성 관리 메뉴 ===\n\n";

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 오크통 목록 보기",
            "[2] 오크통 추가",
            "[3] 오크통 수정",
            "[4] 오크통 삭제",
            "[5] CSV로 저장",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 5: exportOakBoxesToCSV("oak_boxes.csv"); break;
        case 0: cout << "메인으로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}
// 오크통 요약 정보 반환
string OakAgingManager::getSummary() {
    // 현재는 더미 2개 고정
    return "숙성통 수: 2개";
}

// 대시보드에 표시할 정보 구성
vector<string> OakAgingManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = oakList.size();
    int totalPeriod = 0;
    double totalEvap = 0;

    // 오크통의 평균 숙성 기간과 증발률 계산
    for (const auto& box : oakList) {
        totalPeriod += box.getRipeningPeriod();
        totalEvap += box.getEvaporationRate();
    }

    lines.push_back("등록된 오크통 수: " + to_string(count));
    if (count > 0) {
        lines.push_back("평균 숙성 기간: " + to_string(totalPeriod / count) + "일");
        lines.push_back("평균 증발률: " + to_string((int)(totalEvap / count)) + "%");
    }
    else {
        lines.push_back("평균 숙성 기간: -");
        lines.push_back("평균 증발률: -");
    }

    return lines;
}

// 등록된 오크통 목록 출력
void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "등록된 오크통이 없습니다.\n";
        return;
    }
    for (const auto& oak : oakList) {
        oak.ShowInfo();
    }
}

// 새로운 오크통 추가
void OakAgingManager::addOakBox() {
    string id, type, origin, wood;
    string name, spiritId, startDate, endDate;
    int period, count, water;
    int roastInt;
    double evap, temp, hum;
    bool roasted;

    cout << "1. 오크통 ID: "; cin >> id;
    cout << "2. 오크통 이름(별칭): "; cin >> name;
    cout << "3. 종류: "; cin >> type;
    cout << "4. 출신 지역: "; cin >> origin;
    cout << "5. 숙성 기간(일): "; cin >> period;
    cout << "6. 숙성 횟수: "; cin >> count;
    cout << "7. 증발률(%): "; cin >> evap;
    cout << "8. 온도(℃): "; cin >> temp;
    cout << "9. 습도(%): "; cin >> hum;
    cout << "10. 물을 머금은 시간(시간): "; cin >> water;
    cout << "11. 나무 종류: "; cin >> wood;
    cout << "12. 스피릿 ID: "; cin >> spiritId;
    cout << "13. 숙성 시작일 (YYYY-MM-DD): "; cin >> startDate;
    cout << "14. 숙성 종료일 (YYYY-MM-DD): "; cin >> endDate;
    cout << "15. 로스팅 여부 (1: 있음, 0: 없음): "; cin >> roastInt;
    roasted = roastInt == 1;

    OakBox newBox;
    newBox.setId(id);
    newBox.setName(name);
    newBox.setType(type);
    newBox.setOrigin(origin);
    newBox.setRipeningPeriod(period);
    newBox.setAgingCount(count);
    newBox.setEvaporationRate(evap);
    newBox.setTemperature(temp);
    newBox.setHumidity(hum);
    newBox.setWaterAbsorptionTime(water);
    newBox.setWoodType(wood);
    newBox.setSpiritId(spiritId);
    newBox.setAgingStartDate(startDate);
    newBox.setAgingEndDate(endDate);
    newBox.setRoasted(roasted);

    oakList.push_back(newBox);

    cout << "오크통이 추가되었습니다!\n";
}

// 오크통 정보 수정
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
                cout << "[12] 오크통 이름\n";
                cout << "[13] 스피릿 ID\n";
                cout << "[14] 숙성 시작일\n";
                cout << "[15] 숙성 종료일\n";
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
                case 12: {
                    string n;
                    cout << "새 오크통 이름: ";
                    cin >> n;
                    box.setName(n);
                    break;
                }
                case 13: {
                    string sid;
                    cout << "새 스피릿 ID: ";
                    cin >> sid;
                    box.setSpiritId(sid);
                    break;
                }
                case 14: {
                    string date;
                    cout << "새 숙성 시작일 (YYYY-MM-DD): ";
                    cin >> date;
                    box.setAgingStartDate(date);
                    break;
                }
                case 15: {
                    string date;
                    cout << "새 숙성 종료일 (YYYY-MM-DD): ";
                    cin >> date;
                    box.setAgingEndDate(date);
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

// 오크통 삭제
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

// 오크통 정보를 CSV 파일로 내보내기
void OakAgingManager::exportOakBoxesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "CSV 파일을 열 수 없습니다.\n";
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
    cout << "[ " << filename << " ] 파일로 저장 완료!\n";
}