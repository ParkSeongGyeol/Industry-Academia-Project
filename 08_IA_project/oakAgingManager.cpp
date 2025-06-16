#define WIN32_LEAN_AND_MEAN      // windows 헤더 최소화
#define NOMINMAX                 // min/max 매크로 충돌 방지

#include <windows.h>            // byte 충돌 발생하는 헤더
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include "OakAgingManager.h"
#include "UIUtils.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "CommonUtils.h"

using namespace std;

// ----------------------------- 상수 정의 -----------------------------
namespace {
    constexpr char OAKAGING_CSV[] = "oakaging_dummy.csv";
}

// ----------------------------- [1] 데이터 입출력 -----------------------------

// CSV에서 오크통 목록 로드
void OakAgingManager::loadOakBoxesFromCSV(const string& filename) {
    oakList.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[경고] 오크통 CSV 파일을 열 수 없습니다: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // 헤더 스킵
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string token;
        vector<string> fields;
        while (getline(iss, token, ',')) {
            fields.push_back(token);
        }
        if (fields.size() < 14) continue;
        OakBox box;
        box.setId(fields[0]);
        box.setName(fields[1]);
        box.setType(fields[2]);
        box.setOrigin(fields[3]);
        box.setRipeningPeriod(stoi(fields[4]));
        box.setAgingCount(stoi(fields[5]));
        box.setEvaporationRate(stod(fields[6]));
        box.setTemperature(stod(fields[7]));
        box.setHumidity(stod(fields[8]));
        box.setWaterAbsorptionTime(stoi(fields[9]));
        box.setWoodType(fields[10]);
        box.setSpiritId(fields[11]);
        box.setAgingStartDate(fields[12]);
        box.setAgingEndDate(fields[13]);
        // roasted 필드는 CSV에 없으면 false로 둠
        box.setRoasted(false);
        oakList.push_back(box);
    }
    file.close();
}

// 오크통 목록을 CSV로 저장
void OakAgingManager::saveOakBoxesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[오류] 오크통 CSV 저장 실패: " << filename << endl;
        return;
    }
    file << "OakID,Name,Type,Origin,AgingPeriod,RefillCount,EvaporationRate,Temperature,Humidity,WaterAbsorptionTime,WoodType,SpiritID,AgingStartDate,AgingEndDate\n";
    for (const auto& b : oakList) {
        file << b.getId() << ","
            << b.getName() << ","
            << b.getType() << ","
            << b.getOrigin() << ","
            << b.getRipeningPeriod() << ","
            << b.getAgingCount() << ","
            << b.getEvaporationRate() << ","
            << b.getTemperature() << ","
            << b.getHumidity() << ","
            << b.getWaterAbsorptionTime() << ","
            << b.getWoodType() << ","
            << b.getSpiritId() << ","
            << b.getAgingStartDate() << ","
            << b.getAgingEndDate() << "\n";
    }
    file.close();
}

// ----------------------------- [OakBox] Getter/Setter/ShowInfo 구현 -----------------------------

// Getter
std::string OakBox::getId() const { return boxId; }
std::string OakBox::getName() const { return name; }
std::string OakBox::getType() const { return type; }
std::string OakBox::getOrigin() const { return origin; }
std::string OakBox::getWoodType() const { return woodType; }
std::string OakBox::getSpiritId() const { return spiritId; }
std::string OakBox::getAgingStartDate() const { return agingStartDate; }
std::string OakBox::getAgingEndDate() const { return agingEndDate; }

int OakBox::getRipeningPeriod() const { return ripeningPeriod; }
int OakBox::getAgingCount() const { return agingCount; }
int OakBox::getWaterAbsorptionTime() const { return waterAbsorptionTime; }

double OakBox::getEvaporationRate() const { return evaporationRate; }
double OakBox::getTemperature() const { return temperature; }
double OakBox::getHumidity() const { return humidity; }

bool OakBox::isRoasted() const { return roasted; }

// Setter
void OakBox::setId(const std::string& id) { boxId = id; }
void OakBox::setName(const std::string& n) { name = n; }
void OakBox::setType(const std::string& t) { type = t; }
void OakBox::setOrigin(const std::string& o) { origin = o; }
void OakBox::setWoodType(const std::string& wood) { woodType = wood; }
void OakBox::setSpiritId(const std::string& id) { spiritId = id; }
void OakBox::setAgingStartDate(const std::string& date) { agingStartDate = date; }
void OakBox::setAgingEndDate(const std::string& date) { agingEndDate = date; }

void OakBox::setAgingCount(int count) { agingCount = count; }
void OakBox::setWaterAbsorptionTime(int t) { waterAbsorptionTime = t; }
void OakBox::setRipeningPeriod(int p) { ripeningPeriod = p; }

void OakBox::setEvaporationRate(double e) { evaporationRate = e; }
void OakBox::setTemperature(double t) { temperature = t; }
void OakBox::setHumidity(double h) { humidity = h; }

void OakBox::setRoasted(bool r) { roasted = r; }

// 정보 출력
void OakBox::ShowInfo() const {
    std::cout << "오크통 ID: " << boxId << "\n"
        << "별칭: " << name << "\n"
        << "종류: " << type << "\n"
        << "출신지역: " << origin << "\n"
        << "나무 종류: " << woodType << "\n"
        << "스피릿 ID: " << spiritId << "\n"
        << "숙성 시작일: " << agingStartDate << "\n"
        << "숙성 종료일: " << agingEndDate << "\n"
        << "숙성 기간: " << ripeningPeriod << "일\n"
        << "숙성 횟수: " << agingCount << "\n"
        << "물을 머금은 시간: " << waterAbsorptionTime << "시간\n"
        << "증발률: " << evaporationRate << "%\n"
        << "온도: " << temperature << "℃\n"
        << "습도: " << humidity << "%\n"
        << "로스팅 여부: " << (roasted ? "있음" : "없음") << "\n";
}

// ----------------------------- [2] 레시피 기반 오크 숙성 -----------------------------
/**
 * 레시피 기반 오크 숙성
 * - 레시피 목록을 보여주고, 사용자가 레시피 ID와 숙성 조건을 입력
 * - 숙성 공정(증발률, 기간 등) 입력 후, OakBox 객체로 기록
 */
void OakAgingManager::produceOakAgingByRecipe(RecipeManager& recipeMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n오크 숙성에 사용할 레시피 ID 입력: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        UIUtils::pauseConsole();
        return;
    }

    // 숙성 증발률/기간/횟수 등 입력
    double evaporationRate = inputDouble("숙성 증발률(%): ");
    int agingMonths = inputInt("숙성 기간(월): ");
    int refillCount = inputInt("숙성 횟수(리필 포함): ");
    string oakType = inputString("오크통 종류: ");
    string woodType = inputString("나무 종류: ");
    string origin = inputString("오크통 출신 지역: ");
    double temp = inputDouble("숙성 온도(℃): ");
    double hum = inputDouble("숙성 습도(%): ");
    int waterAbsorption = inputInt("물을 머금은 시간(시간): ");
    string spiritId = recipe.spiritId.empty() ? inputString("스피릿 ID: ") : recipe.spiritId;
    string startDate = getCurrentDate();
    string endDate = inputString("숙성 종료일 (YYYY-MM-DD): ");
    string name = inputString("오크통 별칭: ");

    // 레시피 숙성 정보 기록
    recipe.oakType = oakType;
    recipe.agingMonths = agingMonths;
    recipe.ageSpirit(evaporationRate);

    OakBox box;
    box.setId("OAK_" + to_string(oakList.size() + 1));
    box.setName(name);
    box.setType(oakType);
    box.setOrigin(origin);
    box.setRipeningPeriod(agingMonths * 30); // 월 → 일 환산
    box.setAgingCount(refillCount);
    box.setEvaporationRate(evaporationRate);
    box.setTemperature(temp);
    box.setHumidity(hum);
    box.setWaterAbsorptionTime(waterAbsorption);
    box.setWoodType(woodType);
    box.setSpiritId(spiritId);
    box.setAgingStartDate(startDate);
    box.setAgingEndDate(endDate);
    box.setRoasted(false);

    oakList.push_back(box);
    saveOakBoxesToCSV(OAKAGING_CSV);

    cout << "레시피 기반 오크 숙성 정보가 등록되었습니다. (오크통ID: " << box.getId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

// 전체 오크통 요약 정보(개수 등) 반환
string OakAgingManager::getSummary() {
    return "숙성통 수: " + to_string(oakList.size()) + "개";
}

// 대시보드/메뉴용 정보 요약 라인 반환
vector<string> OakAgingManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = oakList.size();
    int totalPeriod = 0;
    double totalEvap = 0;
    for (const auto& box : oakList) {
        totalPeriod += box.getRipeningPeriod();
        totalEvap += box.getEvaporationRate();
    }
    lines.push_back("등록된 오크통 수: " + to_string(count));
    if (count > 0) {
        lines.push_back("평균 숙성 기간: " + to_string(totalPeriod / (int)count) + "일");
        lines.push_back("평균 증발률: " + to_string((int)(totalEvap / count)) + "%");
    }
    else {
        lines.push_back("평균 숙성 기간: -");
        lines.push_back("평균 증발률: -");
    }
    return lines;
}

// 전체 오크통 목록 출력
void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "등록된 오크통이 없습니다.\n";
        return;
    }
    for (const auto& oak : oakList) {
        oak.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV 내보내기 -----------------------------

// 오크통 목록을 CSV로 내보내기
void OakAgingManager::exportOakBoxesToCSV(const string& filename) {
    saveOakBoxesToCSV(filename);
    cout << "[ " << filename << " ] 파일로 저장 완료!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

// 오크통 신규 추가
void OakAgingManager::addOakBox() {
    OakBox box;
    box.setId(inputString("오크통 ID: "));
    box.setName(inputString("오크통 별칭: "));
    box.setType(inputString("종류: "));
    box.setOrigin(inputString("출신 지역: "));
    box.setRipeningPeriod(inputInt("숙성 기간(일): "));
    box.setAgingCount(inputInt("숙성 횟수: "));
    box.setEvaporationRate(inputDouble("증발률(%): "));
    box.setTemperature(inputDouble("온도(℃): "));
    box.setHumidity(inputDouble("습도(%): "));
    box.setWaterAbsorptionTime(inputInt("물을 머금은 시간(시간): "));
    box.setWoodType(inputString("나무 종류: "));
    box.setSpiritId(inputString("스피릿 ID: "));
    box.setAgingStartDate(inputString("숙성 시작일 (YYYY-MM-DD): "));
    box.setAgingEndDate(inputString("숙성 종료일 (YYYY-MM-DD): "));
    box.setRoasted(inputInt("로스팅 여부 (1: 있음, 0: 없음): ") == 1);

    oakList.push_back(box);
    saveOakBoxesToCSV(OAKAGING_CSV);
    cout << "오크통이 추가되었습니다!\n";
    UIUtils::pauseConsole();
}

// 오크통 정보 수정
void OakAgingManager::updateOakBox() {
    string id = inputString("수정할 오크통 ID 입력: ");
    for (auto& box : oakList) {
        if (box.getId() == id) {
            cout << "=== 오크통 정보 수정 ===\n";
            box.setOrigin(inputString("출신 지역 (" + box.getOrigin() + "): "));
            box.setAgingCount(inputInt("숙성 횟수 (" + to_string(box.getAgingCount()) + "): "));
            box.setWaterAbsorptionTime(inputInt("물을 머금은 시간 (" + to_string(box.getWaterAbsorptionTime()) + "): "));
            box.setRoasted(inputInt("로스팅 여부 (" + string(box.isRoasted() ? "1" : "0") + "): ") == 1);
            box.setWoodType(inputString("나무 종류 (" + box.getWoodType() + "): "));
            box.setType(inputString("종류 (" + box.getType() + "): "));
            box.setRipeningPeriod(inputInt("숙성 기간(일) (" + to_string(box.getRipeningPeriod()) + "): "));
            box.setEvaporationRate(inputDouble("증발률(%) (" + to_string(box.getEvaporationRate()) + "): "));
            box.setTemperature(inputDouble("온도(℃) (" + to_string(box.getTemperature()) + "): "));
            box.setHumidity(inputDouble("습도(%) (" + to_string(box.getHumidity()) + "): "));
            box.setId(inputString("오크통 ID (" + box.getId() + "): "));
            box.setName(inputString("오크통 별칭 (" + box.getName() + "): "));
            box.setSpiritId(inputString("스피릿 ID (" + box.getSpiritId() + "): "));
            box.setAgingStartDate(inputString("숙성 시작일 (" + box.getAgingStartDate() + "): "));
            box.setAgingEndDate(inputString("숙성 종료일 (" + box.getAgingEndDate() + "): "));
            saveOakBoxesToCSV(OAKAGING_CSV);
            cout << "수정 완료.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 ID를 가진 오크통이 없습니다.\n";
    UIUtils::pauseConsole();
}

// 오크통 삭제
void OakAgingManager::deleteOakBox() {
    string id = inputString("삭제할 오크통 ID 입력: ");
    auto it = remove_if(oakList.begin(), oakList.end(), [&](const OakBox& b) { return b.getId() == id; });
    if (it != oakList.end()) {
        oakList.erase(it, oakList.end());
        saveOakBoxesToCSV(OAKAGING_CSV);
        cout << "오크통이 삭제되었습니다.\n";
    }
    else {
        cout << "해당 ID를 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [6] 메인 메뉴 루프 -----------------------------

// 오크통 숙성 관리 메인 메뉴 (레시피 연동 포함)
void OakAgingManager::showOakAgingPage() {
    loadOakBoxesFromCSV(OAKAGING_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 오크통 목록 보기",
            "[2] 오크통 추가",
            "[3] 오크통 수정",
            "[4] 오크통 삭제",
            "[5] CSV로 저장",
            "[6] 레시피 기반 오크 숙성",
            "[7] ESP32 환경 데이터 수신",
            "[0] 메인으로 돌아가기"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 5: exportOakBoxesToCSV(OAKAGING_CSV); break;
        case 6: produceOakAgingByRecipe(recipeMgr); break;
        case 7: receiveOakBoxFromESP32(); break;
        case 0: cout << "메인으로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.get();
        }
    } while (choice != 0);
}

// ----------------------------- [7] ESP32 연동 및 환경 센서 수신 -----------------------------

// ESP32로부터 오크통 환경 데이터(TCP) 수신 및 등록
void OakAgingManager::receiveOakBoxFromESP32() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup 실패" << endl;
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000); // OakAging 전용 포트
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "bind 실패" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    listen(serverSocket, 1);
    cout << "[TCP 서버] ESP32 연결 대기 중...\n";

    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cout << "accept 실패" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        string data(buffer);
        cout << "[수신 데이터] " << data << endl;

        istringstream ss(data);
        string token;
        vector<string> fields;

        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

        if (fields.size() == 14) {
            OakBox box;
            box.setId(fields[0]);
            box.setName(fields[1]);
            box.setType(fields[2]);
            box.setOrigin(fields[3]);
            box.setRipeningPeriod(stoi(fields[4]));
            box.setAgingCount(stoi(fields[5]));
            box.setEvaporationRate(stod(fields[6]));
            box.setTemperature(stod(fields[7]));
            box.setHumidity(stod(fields[8]));
            box.setWaterAbsorptionTime(stoi(fields[9]));
            box.setWoodType(fields[10]);
            box.setSpiritId(fields[11]);
            box.setAgingStartDate(fields[12]);
            box.setAgingEndDate(fields[13]);
            box.setRoasted(false);

            oakList.push_back(box);
            cout << "오크통 환경 데이터 저장 완료!\n";
            saveOakBoxesToCSV(OAKAGING_CSV);
        }
        else {
            cout << "필드 수 오류: " << fields.size() << "개" << endl;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}