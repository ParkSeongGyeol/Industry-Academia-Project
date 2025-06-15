#define WIN32_LEAN_AND_MEAN      // windows 헤더 최소화
#define NOMINMAX                 // min/max 매크로 충돌 방지
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "BottledWhiskyManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// ----------------------------- 상수 정의 -----------------------------
namespace {
    constexpr char BOTTLED_CSV[] = "bottledwhisky_dummy.csv";
}

// ----------------------------- 유틸리티 함수 -----------------------------

// 현재 시스템 날짜를 "YYYY-MM-DD" 형식으로 반환
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// 안전한 double 입력 함수
double inputDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "숫자를 입력하세요.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 안전한 int 입력 함수
int inputInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "정수를 입력하세요.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 안전한 string 입력 함수
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// ----------------------------- [1] 데이터 입출력 -----------------------------

// CSV에서 병입 위스키 목록 로드
void BottledWhiskyManager::loadInventoryFromCSV(const string& filename) {
    inventory.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[경고] 병입 CSV 파일을 열 수 없습니다: " << filename << endl;
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
        if (fields.size() < 11) continue;
        BottledWhisky w;
        w.setProductId(fields[0]);
        w.setName(fields[1]);
        w.setBottleCount(stoi(fields[2]));
        w.setTotalVolume(stod(fields[3]));
        w.setPricePerBottle(stod(fields[4]));
        w.setLabelName(fields[5]);
        w.setOakId(fields[6]);
        w.setExportTarget(fields[7]);
        w.setShipmentDate(fields[8]);
        w.setSerialNumber(fields[9]);
        w.setBottlingManager(fields[10]);
        inventory.push_back(w);
    }
    file.close();
}

// 병입 위스키 목록을 CSV로 저장
void BottledWhiskyManager::saveInventoryToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[오류] 병입 CSV 저장 실패: " << filename << endl;
        return;
    }
    file << "ProductID,ProductName,Quantity,TotalVolume,BottlePrice,LabelName,OakID,ReleaseTarget,ReleaseDate,ManufactureNumber,BottlingManager\n";
    for (const auto& w : inventory) {
        file << w.getProductId() << ","
            << w.getName() << ","
            << w.getBottleCount() << ","
            << w.getTotalVolume() << ","
            << w.getPricePerBottle() << ","
            << w.getLabelName() << ","
            << w.getOakId() << ","
            << w.getExportTarget() << ","
            << w.getShipmentDate() << ","
            << w.getSerialNumber() << ","
            << w.getBottlingManager() << "\n";
    }
    file.close();
}

// ----------------------------- [2] 레시피 기반 병입 생산 -----------------------------
/**
 * 레시피 기반 병입 생산
 * - 레시피 목록을 보여주고, 사용자가 레시피 ID와 병입 조건을 입력
 * - 병입 공정(병 수, 용량, 가격 등) 입력 후, BottledWhisky 객체로 기록
 */
void BottledWhiskyManager::produceBottledByRecipe(RecipeManager& recipeMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n병입에 사용할 레시피 ID 입력: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        UIUtils::pauseConsole();
        return;
    }

    // 병입 정보 입력
    int bottleCount = inputInt("병 수: ");
    double bottleVolume = inputDouble("병당 용량(ml): ");
    double bottlePrice = inputDouble("병당 가격(원): ");
    string labelName = inputString("라벨명: ");
    string oakId = inputString("오크통 ID: ");
    string exportTarget = inputString("출고 대상: ");
    string shipmentDate = getCurrentDate();
    string serialNumber = inputString("제조 번호: ");
    string bottlingManager = inputString("병입 담당자: ");

    // 레시피 병입 정보 기록
    recipe.bottleCount = bottleCount;
    recipe.bottleVolume = bottleVolume;
    recipe.bottlePrice = bottlePrice;
    recipe.bottledName = labelName;
    recipe.bottleProduct();

    BottledWhisky w;
    w.setProductId("P" + to_string(inventory.size() + 1));
    w.setName(recipe.name);
    w.setBottleCount(bottleCount);
    w.setTotalVolume(recipe.totalBottledVolume * 1000); // L → ml
    w.setPricePerBottle(bottlePrice);
    w.setLabelName(labelName);
    w.setOakId(oakId);
    w.setExportTarget(exportTarget);
    w.setShipmentDate(shipmentDate);
    w.setSerialNumber(serialNumber);
    w.setBottlingManager(bottlingManager);
    w.setLabeled(true);

    inventory.push_back(w);
    saveInventoryToCSV(BOTTLED_CSV);

    cout << "레시피 기반 병입 위스키가 등록되었습니다. (제품ID: " << w.getProductId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

// 전체 병입 위스키 요약 정보(개수 등) 반환
string BottledWhiskyManager::getSummary() {
    int totalCount = 0;
    double totalPrice = 0;
    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalPrice += w.getPricePerBottle();
    }
    string result = "병입: " + to_string(totalCount) + "병";
    if (!inventory.empty()) {
        result += " / 평균가: " + to_string(static_cast<int>(totalPrice / inventory.size())) + "원";
    }
    return result;
}

// 대시보드/메뉴용 정보 요약 라인 반환
vector<string> BottledWhiskyManager::getPageInfoLines() {
    vector<string> lines;
    int totalCount = 0;
    double totalVolume = 0, totalValue = 0;
    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalVolume += w.getTotalVolume();
        totalValue += w.getBottleCount() * w.getPricePerBottle();
    }
    lines.push_back("전체 병 수량: " + to_string(totalCount) + "병");
    lines.push_back("전체 용량: " + to_string((int)totalVolume) + "ml");
    if (totalCount > 0) {
        lines.push_back("평균 병당 가격: " + to_string((int)(totalValue / totalCount)) + "원");
    }
    else {
        lines.push_back("평균 병당 가격: -");
    }
    return lines;
}

// 전체 병입 위스키 재고 출력
void BottledWhiskyManager::showInventory() {
    if (inventory.empty()) {
        cout << "현재 재고가 없습니다.\n";
        return;
    }
    cout << "\n[병입 위스키 재고 목록]\n";
    for (const auto& whisky : inventory) {
        whisky.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV 내보내기 -----------------------------

// 병입 위스키 재고를 CSV 파일로 내보내기
void BottledWhiskyManager::exportInventoryToCSV(const string& filename) {
    saveInventoryToCSV(filename);
    cout << "[ " << filename << " ] 파일로 저장 완료!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

// 병입 위스키 신규 추가
void BottledWhiskyManager::addWhisky() {
    BottledWhisky w;
    w.setProductId(inputString("제품 ID: "));
    w.setName(inputString("제품명: "));
    w.setLabelName(inputString("라벨명: "));
    w.setBatchNumber(inputString("배치 번호: "));
    w.setExportTarget(inputString("출고 대상: "));
    w.setOakId(inputString("오크통 ID: "));
    w.setShipmentDate(inputString("출고 일자 (YYYY-MM-DD): "));
    w.setSerialNumber(inputString("제조 번호: "));
    w.setBottlingManager(inputString("병입 담당자: "));
    w.setBottleCount(inputInt("수량(병): "));
    w.setTotalVolume(inputDouble("총 용량(ml): "));
    w.setPricePerBottle(inputDouble("병당 가격: "));
    w.setLabeled(inputInt("라벨 부착 여부 (1: O, 0: X): ") == 1);

    inventory.push_back(w);
    saveInventoryToCSV(BOTTLED_CSV);
    cout << "제품이 추가되었습니다.\n";
    UIUtils::pauseConsole();
}

// 제품 정보 수정
void BottledWhiskyManager::updateWhisky() {
    string name = inputString("수정할 제품명 입력: ");
    for (auto& w : inventory) {
        if (w.getName() == name) {
            cout << "=== 제품 정보 수정 ===\n";
            w.setName(inputString("제품명 (" + w.getName() + "): "));
            w.setLabelName(inputString("라벨명 (" + w.getLabelName() + "): "));
            w.setBatchNumber(inputString("배치 번호 (" + w.getBatchNumber() + "): "));
            w.setExportTarget(inputString("출고 대상 (" + w.getExportTarget() + "): "));
            w.setBottleCount(inputInt("수량(병) (" + to_string(w.getBottleCount()) + "): "));
            w.setTotalVolume(inputDouble("총 용량(ml) (" + to_string(w.getTotalVolume()) + "): "));
            w.setPricePerBottle(inputDouble("병당 가격 (" + to_string(w.getPricePerBottle()) + "): "));
            w.setLabeled(inputInt("라벨 부착 여부 (" + string(w.isLabeled() ? "1" : "0") + "): ") == 1);
            w.setOakId(inputString("오크통 ID (" + w.getOakId() + "): "));
            w.setShipmentDate(inputString("출고 일자 (" + w.getShipmentDate() + "): "));
            w.setSerialNumber(inputString("제조 번호 (" + w.getSerialNumber() + "): "));
            w.setBottlingManager(inputString("병입 담당자 (" + w.getBottlingManager() + "): "));
            w.setProductId(inputString("제품 ID (" + w.getProductId() + "): "));
            saveInventoryToCSV(BOTTLED_CSV);
            cout << "수정 완료.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 제품을 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// 제품 삭제
void BottledWhiskyManager::deleteWhisky() {
    string name = inputString("삭제할 제품명 입력: ");
    auto it = remove_if(inventory.begin(), inventory.end(), [&](const BottledWhisky& w) { return w.getName() == name; });
    if (it != inventory.end()) {
        inventory.erase(it, inventory.end());
        saveInventoryToCSV(BOTTLED_CSV);
        cout << "삭제되었습니다.\n";
    }
    else {
        cout << "해당 제품을 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}

// 출고 기록 생성 및 재고 감소
void BottledWhiskyManager::shipWhisky() {
    string name = inputString("출고할 제품명: ");
    int qty = inputInt("출고 수량(병): ");
    string date = inputString("출고 날짜 (YYYY-MM-DD): ");

    for (auto& whisky : inventory) {
        if (whisky.getName() == name) {
            if (qty > whisky.getBottleCount()) {
                cout << "출고 수량이 재고보다 많습니다.\n";
                UIUtils::pauseConsole();
                return;
            }
            double totalPrice = qty * whisky.getPricePerBottle();

            ShipmentRecord record;
            record.setProductName(name);
            record.setDate(date);
            record.setQuantity(qty);
            record.setTotalPrice(totalPrice);
            shipmentLog.push_back(record);

            whisky.decreaseStock(qty);

            saveInventoryToCSV(BOTTLED_CSV);
            cout << "출고 기록이 등록되었습니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 제품을 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// 출고 기록 출력
void BottledWhiskyManager::showShipmentLog() {
    if (shipmentLog.empty()) {
        cout << "출고 기록이 없습니다.\n";
        return;
    }
    cout << "\n[출고 기록 목록]\n";
    for (const auto& record : shipmentLog) {
        record.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [6] 메인 메뉴 루프 -----------------------------

// 병입 위스키 관리 메인 메뉴 (레시피 연동 포함)
void BottledWhiskyManager::showBottledWhiskyPage() {
    loadInventoryFromCSV(BOTTLED_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 재고 목록 보기",
            "[2] 제품 추가",
            "[3] 제품 출고",
            "[4] 출고 기록 보기",
            "[5] 완제품 수정",
            "[6] 완제품 삭제",
            "[7] CSV로 저장",
            "[8] 레시피 기반 병입 생산",
            "[9] ESP32 데이터 수신",
            "[0] 메인 메뉴로 돌아가기"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 7: exportInventoryToCSV(BOTTLED_CSV); break;
        case 8: produceBottledByRecipe(recipeMgr); break;
        case 9: receiveWhiskyFromESP32(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.get();
        }
    } while (choice != 0);
}

// ----------------------------- [7] ESP32 연동 및 환경 센서 수신 -----------------------------

// ESP32로부터 병입 위스키 데이터(TCP) 수신 및 등록
void BottledWhiskyManager::receiveWhiskyFromESP32() {
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
    serverAddr.sin_port = htons(5001); // BottledWhisky 전용 포트
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

        if (fields.size() == 13) {
            BottledWhisky whisky;
            whisky.setProductId(fields[0]);
            whisky.setName(fields[1]);
            whisky.setLabelName(fields[2]);
            whisky.setBatchNumber(fields[3]);
            whisky.setExportTarget(fields[4]);
            whisky.setOakId(fields[5]);
            whisky.setShipmentDate(fields[6]);
            whisky.setSerialNumber(fields[7]);
            whisky.setBottlingManager(fields[8]);
            whisky.setBottleCount(stoi(fields[9]));
            whisky.setTotalVolume(stod(fields[10]));
            whisky.setPricePerBottle(stod(fields[11]));
            whisky.setLabeled(fields[12] == "1" || fields[12] == "Yes");

            inventory.push_back(whisky);
            cout << "병입 위스키 저장 완료!\n";
            saveInventoryToCSV(BOTTLED_CSV);
        }
        else {
            cout << "필드 수 오류: " << fields.size() << "개" << endl;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}