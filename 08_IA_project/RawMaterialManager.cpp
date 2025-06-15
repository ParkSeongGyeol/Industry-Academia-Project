// 원재료 관리 기능 정의 파일
#include "RawMaterialManager.h"
#include "UIUtils.h"
#include "StorageEnvironment.h"
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

// -----------------------------------------------------------------------------
// [유틸리티] 현재 시스템 날짜를 "YYYY-MM-DD" 형식으로 반환
// -----------------------------------------------------------------------------
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now); // (time_t)를 지역 시간 구조체 tm으로 변환
    char buf[11]; // 날짜 형식: "YYYY-MM-DD" (10자 + 널 종료)
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// -----------------------------------------------------------------------------
// [1] 데이터 입출력 (CSV 파일 로드/저장)
// -----------------------------------------------------------------------------

// CSV 파일에서 원재료 목록을 읽어 materials 벡터를 초기화
void RawMaterialManager::loadMaterialsFromCSV(const string& filename) {
    materials.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[경고] 원재료 CSV 파일을 열 수 없습니다: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // 헤더 스킵
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> fields;
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }
        if (fields.size() < 17) continue;
        RawMaterial m;
        m.setMaterialId(fields[0]);
        m.setName(fields[1]);
        m.setType(fields[2]);
        m.setOrigin(fields[3]);
        m.setWeightKg(stod(fields[4]));
        m.setStorageLocation(fields[5]);
        m.setStorageMethod(fields[6]);
        m.setExpiryDate(fields[7]);
        m.setEntryDate(fields[8]);
        m.setExitDate(fields[9]);
        m.setStatus(fields[10]);
        m.setUnit(fields[11]);
        m.setUnitPrice(stod(fields[12]));
        m.setEntryManager(fields[13]);
        m.setExitManager(fields[14]);
        m.setQualityCheck(fields[15]);
        m.setQualityCheckDate(fields[16]);
        materials.push_back(m);
    }
    file.close();
}

// CSV 파일로 전체 materials 저장
void RawMaterialManager::saveMaterialsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
         << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate\n";
    for (const auto& m : materials) {
        file << m.getMaterialId() << "," << m.getName() << "," << m.getType() << "," << m.getOrigin() << ","
             << m.getWeightKg() << "," << m.getStorageLocation() << "," << m.getStorageMethod() << ","
             << m.getExpiryDate() << "," << m.getEntryDate() << "," << m.getExitDate() << ","
             << m.getStatus() << "," << m.getUnit() << "," << m.getUnitPrice() << ","
             << m.getEntryManager() << "," << m.getExitManager() << ","
             << m.getQualityCheck() << "," << m.getQualityCheckDate() << "\n";
    }
    file.close();
}

// 사용된 원재료 목록을 지정된 파일 이름의 CSV 파일로 저장
bool RawMaterialManager::exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
        << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate,UsedDate\n";
    for (const auto& m : usedList) {
        file << m.getMaterialId() << "," << m.getName() << "," << m.getType() << "," << m.getOrigin() << ","
            << m.getWeightKg() << "," << m.getStorageLocation() << "," << m.getStorageMethod() << ","
            << m.getExpiryDate() << "," << m.getEntryDate() << "," << m.getExitDate() << ","
            << m.getStatus() << "," << m.getUnit() << "," << m.getUnitPrice() << ","
            << m.getEntryManager() << "," << m.getExitManager() << ","
            << m.getQualityCheck() << "," << m.getQualityCheckDate() << ","
            << getCurrentDate() << "\n";
    }
    file.close();
    return true;
}

// -----------------------------------------------------------------------------
// [2] 내부 연산 (재고 계산, 소모, 배치 처리 등)
// -----------------------------------------------------------------------------

// 특정 원재료 이름에 대해 현재 출고되지 않은 총 재고량을 반환
double RawMaterialManager::getStock(const string& materialId) const {
    double total = 0;
    for (const auto& m : materials) {
        if (m.getName() == materialId && m.getExitDate().empty())
            total += m.getWeightKg();
    }
    return total;
}

// 주어진 이름의 원재료에서 지정된 양을 차감하여 출고 처리
void RawMaterialManager::consumeMaterial(const string& name, double amount) {
    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            if (m.getWeightKg() >= amount) {
                m.setWeightKg(m.getWeightKg() - amount);
                if (m.getWeightKg() == 0)
                    m.setExitDate(getCurrentDate());
                break;
            } else {
                amount -= m.getWeightKg();
                m.setExitDate(getCurrentDate());
                m.setWeightKg(0);
            }
        }
    }
}

// 발효 배치 생산을 위한 원재료 소모 처리 (보리 60%, 호밀 10%, 물 30%)
bool RawMaterialManager::processFermentationBatch(double totalBatchKg) {
    double requiredBarley = totalBatchKg * 0.6;
    double requiredRye = totalBatchKg * 0.1;
    double requiredWater = totalBatchKg * 0.3;
    double stockBarley = getStock("보리");
    double stockRye = getStock("호밀");
    double stockWater = getStock("물");
    if (stockBarley < requiredBarley) {
        cout << "보리 재고 부족 (" << stockBarley << "kg 보유 / " << requiredBarley << "kg 필요)\n";
        return false;
    }
    if (stockRye < requiredRye) {
        cout << "호밀 재고 부족 (" << stockRye << "kg 보유 / " << requiredRye << "kg 필요)\n";
        return false;
    }
    if (stockWater < requiredWater) {
        cout << "물 재고 부족 (" << stockWater << "kg 보유 / " << requiredWater << "kg 필요)\n";
        return false;
    }
    consumeMaterial("보리", requiredBarley);
    consumeMaterial("호밀", requiredRye);
    consumeMaterial("물", requiredWater);

    // 사용된 원재료 내역 생성 및 CSV 저장
    vector<RawMaterial> used;

    RawMaterial barley;
    barley.setMaterialId("USED001");
    barley.setName("보리");
    barley.setType("곡물");
    barley.setOrigin("스코틀랜드");
    barley.setWeightKg(requiredBarley);
    barley.setStorageLocation("창고 A");
    barley.setStorageMethod("저온 건조");
    barley.setExitDate(getCurrentDate());
    barley.setStatus("출고");
    barley.setUnit("kg");
    barley.setUnitPrice(950.0);
    barley.setExitManager("자동출고");
    barley.setQualityCheck("자동 검사");
    barley.setQualityCheckDate(getCurrentDate());
    used.push_back(barley);

    RawMaterial rye;
    rye.setMaterialId("USED002");
    rye.setName("호밀");
    rye.setType("곡물");
    rye.setOrigin("미국");
    rye.setWeightKg(requiredRye);
    rye.setStorageLocation("창고 A");
    rye.setStorageMethod("상온 보관");
    rye.setExitDate(getCurrentDate());
    rye.setStatus("출고");
    rye.setUnit("kg");
    rye.setUnitPrice(900.0);
    rye.setExitManager("자동출고");
    rye.setQualityCheck("자동 검사");
    rye.setQualityCheckDate(getCurrentDate());
    used.push_back(rye);

    RawMaterial water;
    water.setMaterialId("USED003");
    water.setName("물");
    water.setType("기타");
    water.setOrigin("제조 현장");
    water.setWeightKg(requiredWater);
    water.setStorageLocation("탱크 B");
    water.setStorageMethod("청결 밀봉");
    water.setExpiryDate("-");
    water.setExitDate(getCurrentDate());
    water.setStatus("출고");
    water.setUnit("L");
    water.setUnitPrice(100.0);
    water.setExitManager("자동출고");
    water.setQualityCheck("검사 없음");
    water.setQualityCheckDate("-");
    used.push_back(water);

    exportUsedMaterialsToCSV("used_raw_materials.csv", used);
    cout << "발효 배치용 원재료가 CSV에 저장되었습니다.\n";
    return true;
}

// -----------------------------------------------------------------------------
// [3] 정보 요약/조회/출력
// -----------------------------------------------------------------------------

// 전체 원재료 개수와 총 재고 무게 요약 문자열 반환
string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.getExitDate().empty())  // 출고되지 않은 경우
            totalKg += item.getWeightKg();

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
}

// 원재료 페이지 상단 정보 요약 (종류, 무게, 종류별 수량, 보관 위치 등)
vector<string> RawMaterialManager::getPageInfoLines() {
    int totalKinds = 0;
    double totalWeight = 0;
    set<string> storagePlaces;
    map<string, int> typeCount;

    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            ++totalKinds;
            totalWeight += m.getWeightKg();
            storagePlaces.insert(m.getStorageLocation());
            typeCount[m.getType()]++;
        }
    }

    vector<string> lines;
    lines.push_back("현재 보유 원재료: " + to_string(totalKinds) + "종 / " + to_string((int)totalWeight) + "kg");

    string typeSummary = "종류별 수량: ";
    for (const auto& pair : typeCount)
        typeSummary += pair.first + ": " + to_string(pair.second) + "개  ";
    lines.push_back(typeSummary);

    string storageSummary = "보관 위치: ";
    for (const auto& place : storagePlaces)
        storageSummary += place + "  ";
    lines.push_back(storageSummary);

    return lines;
}

// 출고되지 않은 원재료만 출력
void RawMaterialManager::showInventory() {
    cout << "\n=== 현재 보유 원재료 목록 ===\n";
    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            cout << "ID: " << m.getMaterialId() << "\n"
                << "이름: " << m.getName() << "\n"
                << "종류: " << m.getType() << "\n"
                << "원산지: " << m.getOrigin() << "\n"
                << "재고: " << m.getWeightKg() << " " << m.getUnit() << "\n"
                << "단가: " << m.getUnitPrice() << "원/" << m.getUnit() << "\n"
                << "보관 위치: " << m.getStorageLocation() << "\n"
                << "보관 방법: " << m.getStorageMethod() << "\n"
                << "유통기한: " << m.getExpiryDate() << "\n"
                << "입고일: " << m.getEntryDate() << "\n"
                << "입고 담당자: " << m.getEntryManager() << "\n"
                << "품질 검사: " << m.getQualityCheck() << " (" << m.getQualityCheckDate() << ")\n"
                << "-----------------------------\n";
        }
    }
}

// 전체 원재료 입출고 이력 출력 (출고된 항목 포함)
void RawMaterialManager::showAllMaterials() {
    cout << "\n=== 전체 원재료 입출고 이력 ===\n";
    for (const auto& m : materials) {
        cout << "ID: " << m.getMaterialId() << "\n"
            << "이름: " << m.getName() << "\n"
            << "종류: " << m.getType() << "\n"
            << "원산지: " << m.getOrigin() << "\n"
            << "무게: " << m.getWeightKg() << " " << m.getUnit() << "\n"
            << "단가: " << m.getUnitPrice() << "원/" << m.getUnit() << "\n"
            << "보관 위치: " << m.getStorageLocation() << "\n"
            << "보관 방법: " << m.getStorageMethod() << "\n"
            << "유통기한: " << m.getExpiryDate() << "\n"
            << "입고일: " << m.getEntryDate() << "\n"
            << "출고일: " << (m.getExitDate().empty() ? "-" : m.getExitDate()) << "\n"
            << "상태: " << m.getStatus() << "\n"
            << "입고 담당자: " << m.getEntryManager() << "\n"
            << "출고 담당자: " << (m.getExitManager().empty() ? "-" : m.getExitManager()) << "\n"
            << "품질 검사: " << m.getQualityCheck() << " (" << m.getQualityCheckDate() << ")\n"
            << "-----------------------------\n";
    }
}

// 품질 검사 미완료 원재료 목록 출력
void RawMaterialManager::showUninspectedMaterials() {
    cout << "\n=== 품질 검사 미완료 원재료 목록 ===\n";
    bool found = false;

    for (const auto& m : materials) {
        if (m.getQualityCheck().empty() || m.getQualityCheck() == "검사 안함") {
            found = true;
            cout << "ID: " << m.getMaterialId() << "\n"
                << "이름: " << m.getName() << "\n"
                << "입고일: " << m.getEntryDate() << "\n"
                << "보관 위치: " << m.getStorageLocation() << "\n"
                << "입고 담당자: " << m.getEntryManager() << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "모든 원재료는 품질 검사를 완료했습니다.\n";
    }
}

// 담당자별 입출고 이력 출력
void RawMaterialManager::showMaterialsByManager() {
    string manager;
    cin.ignore();
    cout << "\n조회할 담당자 이름 입력: ";
    getline(cin, manager);

    bool found = false;
    cout << "\n=== " << manager << " 담당 입출고 이력 ===\n";

    for (const auto& m : materials) {
        if (m.getEntryManager() == manager || m.getExitManager() == manager) {
            found = true;
            cout << "ID: " << m.getMaterialId() << "\n"
                << "이름: " << m.getName() << "\n"
                << "입고일: " << m.getEntryDate() << " (입고 담당: " << m.getEntryManager() << ")\n"
                << "출고일: " << (m.getExitDate().empty() ? "-" : m.getExitDate())
                << " (출고 담당: " << (m.getExitManager().empty() ? "-" : m.getExitManager()) << ")\n"
                << "상태: " << m.getStatus() << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "해당 담당자의 입출고 기록이 없습니다.\n";
    }
}

// 보관 장소의 환경 정보 출력
void RawMaterialManager::showStorageEnvironment() {
    vector<StorageEnvironment> storageList = {
        {"창고 A", 18.5f, 55.2f},
        {"지하 저장고", 12.0f, 70.0f},
        {"실험실 보관소", 22.3f, 40.0f}
    };

    cout << "\n=== 보관 장소 환경 정보 조회 ===\n";
    for (const auto& storage : storageList) {
        cout << "-----------------------------\n";
        storage.displayInfo();
    }

    cout << "\n계속하려면 Enter를 누르세요...";
    cin.ignore(); cin.get();
}

// -----------------------------------------------------------------------------
// [4] CSV 내보내기(출고/재고)
// -----------------------------------------------------------------------------

// 출고된(사용된) 원재료 목록을 CSV 파일로 내보내기
void RawMaterialManager::exportUsedInventoryToCSV() {
    vector<RawMaterial> usedList;

    for (const auto& m : materials) {
        if (!m.getExitDate().empty()) {
            usedList.push_back(m);
        }
    }

    if (usedList.empty()) {
        cout << "\n출고된 원재료가 없습니다.\n";
        return;
    }

    if (exportUsedMaterialsToCSV("used_raw_materials.csv", usedList)) {
        cout << "\n[used_raw_materials.csv] 파일로 저장 완료!\n";
    }
    else {
        cout << "\nCSV 저장에 실패했습니다.\n";
    }
}

// 출고되지 않은(보유 중인) 원재료 목록을 CSV 파일로 내보내기
void RawMaterialManager::exportRemainingStockToCSV() {
    vector<RawMaterial> stockList;

    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            stockList.push_back(m);
        }
    }

    if (stockList.empty()) {
        cout << "\n보유 중인 원재료가 없습니다.\n";
        return;
    }

    if (exportUsedMaterialsToCSV("stock_raw_materials.csv", stockList)) {
        cout << "\n[stock_raw_materials.csv] 파일로 저장 완료!\n";
    }
    else {
        cout << "\nCSV 저장에 실패했습니다.\n";
    }
}

// -----------------------------------------------------------------------------
// [5] 입력/수정/삭제/검색
// -----------------------------------------------------------------------------

// 원재료 추가(입력)
void RawMaterialManager::addMaterial() {
    RawMaterial newItem;
    cin.ignore();  // 입력 버퍼 초기화
    cout << "\n=== 원재료 추가 ===\n";

    string input;
    double doubleInput;

    cout << "ID: ";
    getline(cin, input);
    newItem.setMaterialId(input);

    cout << "이름: ";
    getline(cin, input);
    newItem.setName(input);

    cout << "종류: ";
    getline(cin, input);
    newItem.setType(input);

    cout << "출신 지역: ";
    getline(cin, input);
    newItem.setOrigin(input);

    cout << "무게(kg): ";
    cin >> doubleInput;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newItem.setWeightKg(doubleInput);

    cout << "단위 (예: kg, L): ";
    getline(cin, input);
    newItem.setUnit(input);

    cout << "단가 (" << input << "당): ";
    cin >> doubleInput;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newItem.setUnitPrice(doubleInput);

    cout << "보관 위치: ";
    getline(cin, input);
    newItem.setStorageLocation(input);

    cout << "보관 방법: ";
    getline(cin, input);
    newItem.setStorageMethod(input);

    cout << "유통기한: ";
    getline(cin, input);
    newItem.setExpiryDate(input);

    cout << "입고일(YYYY-MM-DD): ";
    getline(cin, input);
    newItem.setEntryDate(input);

    cout << "입고 담당자: ";
    getline(cin, input);
    newItem.setEntryManager(input);

    cout << "품질 검사 결과 (예: 통과, 불량, 검사 안함): ";
    getline(cin, input);
    newItem.setQualityCheck(input);

    cout << "품질 검사 일자 (YYYY-MM-DD): ";
    getline(cin, input);
    newItem.setQualityCheckDate(input);

    newItem.setExitDate("");
    newItem.setExitManager("");
    newItem.setStatus("정상");

    materials.push_back(newItem);
    saveMaterialsToCSV("rawmaterial_dummy.csv");
    cout << "원재료가 추가되었습니다.\n";
}

// 원재료 정보 수정
void RawMaterialManager::updateMaterial() {
    string name;
    cin.ignore();
    cout << "\n수정할 원재료 이름 입력: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            cout << "=== 원재료 수정 ===\n";

            string input;
            double doubleInput;

            cout << "보관 위치 (" << m.getStorageLocation() << "): ";
            getline(cin, input);
            if (!input.empty()) m.setStorageLocation(input);

            cout << "보관 방법 (" << m.getStorageMethod() << "): ";
            getline(cin, input);
            if (!input.empty()) m.setStorageMethod(input);

            cout << "유통기한 (" << m.getExpiryDate() << "): ";
            getline(cin, input);
            if (!input.empty()) m.setExpiryDate(input);

            cout << "무게(kg) (" << m.getWeightKg() << "): ";
            string weightStr;
            getline(cin, weightStr);
            if (!weightStr.empty()) {
                try {
                    doubleInput = stod(weightStr);
                    m.setWeightKg(doubleInput);
                }
                catch (...) {}
            }

            saveMaterialsToCSV("rawmaterial_dummy.csv");
            cout << "수정 완료.\n";
            return;
        }
    }

    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

// 원재료 출고 처리 (exit_date 설정)
void RawMaterialManager::deleteMaterial() {
    string name;
    cin.ignore();
    cout << "\n출고 처리할 원재료 이름 입력: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            string date;
            cout << "출고일 입력 (YYYY-MM-DD): ";
            getline(cin, date);
            m.setExitDate(date);
            saveMaterialsToCSV("rawmaterial_dummy.csv");
            cout << "출고 처리 완료.\n";
            return;
        }
    }

    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

// 이름으로 원재료 검색
void RawMaterialManager::searchMaterial() {
    string name;
    cin.ignore();
    cout << "\n검색할 원재료 이름 입력: ";
    getline(cin, name);

    bool found = false;
    for (const auto& m : materials) {
        if (m.getName() == name) {
            cout << "이름: " << m.getName() << "\n"
                << "종류: " << m.getType() << "\n"
                << "출신: " << m.getOrigin() << "\n"
                << "무게: " << m.getWeightKg() << "kg\n"
                << "보관 위치: " << m.getStorageLocation() << "\n"
                << "보관 방법: " << m.getStorageMethod() << "\n"
                << "유통기한: " << m.getExpiryDate() << "\n"
                << "입고일: " << m.getEntryDate() << "\n"
                << "출고일: " << (m.getExitDate().empty() ? "-" : m.getExitDate()) << "\n"
                << "-----------------------------\n";
            found = true;
        }
    }

    if (!found)
        cout << "해당 이름의 원재료를 찾을 수 없습니다.\n";
}

// -----------------------------------------------------------------------------
// [6] 메인 메뉴 루프
// -----------------------------------------------------------------------------

// 원재료 관리 콘솔 메뉴 루프
void RawMaterialManager::showRawMaterialPage() {
    loadMaterialsFromCSV("rawmaterial_dummy.csv");

    int choice;
    do {
        system("cls");
        cout << "=== 원재료 관리 메뉴 ===\n\n";

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 현재 보유 재고 보기",
            "[2] 전체 이력 보기",
            "[3] 원재료 추가",
            "[4] 원재료 정보 수정",
            "[5] 원재료 삭제 (출고 처리)",
            "[6] 원재료 검색",
            "[7] 보관 장소 환경정보 보기",
            "[8] 발효 배치용 원재료 사용",
            "[9] 사용된 원재료 CSV 확인/내보내기",
            "[10] 출고되지 않은 재고 CSV 내보내기",
            "[11] 품질 검사 미완료 재료 보기",
            "[12] 담당자별 입출고 이력 보기",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: showAllMaterials(); break;
        case 3: addMaterial(); break;
        case 4: updateMaterial(); break;
        case 5: deleteMaterial(); break;
        case 6: searchMaterial(); break;
        case 7: showStorageEnvironment(); break;
        case 8: {
            double totalKg;
            cout << "\n생산할 총 발효 배치량 입력 (kg): ";
            cin >> totalKg;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            processFermentationBatch(totalKg);
            break;
        }
        case 9:
            exportUsedInventoryToCSV();
            break;
        case 10:
            exportRemainingStockToCSV();
            break;
        case 11:
            showUninspectedMaterials();
            break;
        case 12:
            showMaterialsByManager();
            break;
        case 0: cout << "메인으로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}

