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
#include <stdexcept>

using namespace std;

// ----------------------------- 상수 정의 -----------------------------
namespace {
    constexpr int CSV_FIELD_COUNT = 17;
    constexpr char RAW_MATERIAL_CSV[] = "rawmaterial_dummy.csv";
    constexpr char USED_MATERIAL_CSV[] = "used_raw_materials.csv";
    constexpr char STOCK_MATERIAL_CSV[] = "stock_raw_materials.csv";
}

// ----------------------------- 유틸리티 함수 -----------------------------

// 현재 시스템 날짜를 "YYYY-MM-DD" 형식으로 반환
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now); // (time_t)를 지역 시간 구조체 tm으로 변환
    char buf[11]; // 날짜 형식: "YYYY-MM-DD" (10자 + 널 종료)
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// 안전한 double 입력
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

// 안전한 string 입력
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// CSV 헤더 출력
void writeCSVHeader(ofstream& file, bool withUsedDate = false) {
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
         << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate";
    if (withUsedDate) file << ",UsedDate";
    file << "\n";
}

// CSV 한 줄 출력
void writeCSVRow(ofstream& file, const RawMaterial& m, bool withUsedDate = false) {
    file << m.getMaterialId() << "," << m.getName() << "," << m.getType() << "," << m.getOrigin() << ","
         << m.getWeightKg() << "," << m.getStorageLocation() << "," << m.getStorageMethod() << ","
         << m.getExpiryDate() << "," << m.getEntryDate() << "," << m.getExitDate() << ","
         << m.getStatus() << "," << m.getUnit() << "," << m.getUnitPrice() << ","
         << m.getEntryManager() << "," << m.getExitManager() << ","
         << m.getQualityCheck() << "," << m.getQualityCheckDate();
    if (withUsedDate) file << "," << getCurrentDate();
    file << "\n";
}

// ID로 원재료 검색 (출고되지 않은 것만)
RawMaterial* findMaterialById(vector<RawMaterial>& materials, const string& id) {
    for (auto& m : materials) {
        if (m.getMaterialId() == id && m.getExitDate().empty())
            return &m;
    }
    return nullptr;
}

// ----------------------------- [1] 데이터 입출력 -----------------------------

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
        if (fields.size() < CSV_FIELD_COUNT) continue;
        try {
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
        } catch (...) {
            cout << "[경고] CSV 파싱 오류: " << line << endl;
        }
    }
    file.close();
}

void RawMaterialManager::saveMaterialsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[오류] 파일 저장 실패: " << filename << endl;
        return;
    }
    writeCSVHeader(file);
    for (const auto& m : materials) {
        writeCSVRow(file, m);
    }
    file.close();
}

bool RawMaterialManager::exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    writeCSVHeader(file, true);
    for (const auto& m : usedList) {
        writeCSVRow(file, m, true);
    }
    file.close();
    return true;
}

// ----------------------------- [2] 내부 연산 -----------------------------

double RawMaterialManager::getStock(const string& materialName) const {
    double total = 0;
    for (const auto& m : materials) {
        if (m.getName() == materialName && m.getExitDate().empty())
            total += m.getWeightKg();
    }
    return total;
}

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

    exportUsedMaterialsToCSV(USED_MATERIAL_CSV, used);
    cout << "발효 배치용 원재료가 CSV에 저장되었습니다.\n";
    return true;
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.getExitDate().empty())
            totalKg += item.getWeightKg();

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
}

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

void RawMaterialManager::showMaterialsByManager() {
    cin.ignore();
    string manager = inputString("\n조회할 담당자 이름 입력: ");

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

// ----------------------------- [4] CSV 내보내기 -----------------------------

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

    if (exportUsedMaterialsToCSV(USED_MATERIAL_CSV, usedList)) {
        cout << "\n[" << USED_MATERIAL_CSV << "] 파일로 저장 완료!\n";
    }
    else {
        cout << "\nCSV 저장에 실패했습니다.\n";
    }
}

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

    if (exportUsedMaterialsToCSV(STOCK_MATERIAL_CSV, stockList)) {
        cout << "\n[" << STOCK_MATERIAL_CSV << "] 파일로 저장 완료!\n";
    }
    else {
        cout << "\nCSV 저장에 실패했습니다.\n";
    }
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

void RawMaterialManager::addMaterial() {
    cin.ignore();
    RawMaterial newItem;
    cout << "\n=== 원재료 추가 ===\n";

    newItem.setMaterialId(inputString("ID: "));
    newItem.setName(inputString("이름: "));
    newItem.setType(inputString("종류: "));
    newItem.setOrigin(inputString("출신 지역: "));
    newItem.setWeightKg(inputDouble("무게(kg): "));
    newItem.setUnit(inputString("단위 (예: kg, L): "));
    newItem.setUnitPrice(inputDouble("단가 (" + newItem.getUnit() + "당): "));
    newItem.setStorageLocation(inputString("보관 위치: "));
    newItem.setStorageMethod(inputString("보관 방법: "));
    newItem.setExpiryDate(inputString("유통기한: "));
    newItem.setEntryDate(inputString("입고일(YYYY-MM-DD): "));
    newItem.setEntryManager(inputString("입고 담당자: "));
    newItem.setQualityCheck(inputString("품질 검사 결과 (예: 통과, 불량, 검사 안함): "));
    newItem.setQualityCheckDate(inputString("품질 검사 일자 (YYYY-MM-DD): "));

    newItem.setExitDate("");
    newItem.setExitManager("");
    newItem.setStatus("정상");

    materials.push_back(newItem);
    saveMaterialsToCSV(RAW_MATERIAL_CSV);
    cout << "원재료가 추가되었습니다.\n";
}

void RawMaterialManager::updateMaterial() {
    cin.ignore();
    string name = inputString("\n수정할 원재료 이름 입력: ");

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            cout << "=== 원재료 수정 ===\n";

            string input;
            input = inputString("보관 위치 (" + m.getStorageLocation() + "): ");
            if (!input.empty()) m.setStorageLocation(input);

            input = inputString("보관 방법 (" + m.getStorageMethod() + "): ");
            if (!input.empty()) m.setStorageMethod(input);

            input = inputString("유통기한 (" + m.getExpiryDate() + "): ");
            if (!input.empty()) m.setExpiryDate(input);

            input = inputString("무게(kg) (" + to_string(m.getWeightKg()) + "): ");
            if (!input.empty()) {
                try {
                    m.setWeightKg(stod(input));
                } catch (...) {}
            }

            saveMaterialsToCSV(RAW_MATERIAL_CSV);
            cout << "수정 완료.\n";
            return;
        }
    }

    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

void RawMaterialManager::deleteMaterial() {
    cin.ignore();
    string name = inputString("\n출고 처리할 원재료 이름 입력: ");

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            string date = inputString("출고일 입력 (YYYY-MM-DD): ");
            m.setExitDate(date);
            saveMaterialsToCSV(RAW_MATERIAL_CSV);
            cout << "출고 처리 완료.\n";
            return;
        }
    }

    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

void RawMaterialManager::searchMaterial() {
    cin.ignore();
    string name = inputString("\n검색할 원재료 이름 입력: ");

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

// ----------------------------- [6] 메인 메뉴 루프 -----------------------------

void RawMaterialManager::showRawMaterialPage() {
    loadMaterialsFromCSV(RAW_MATERIAL_CSV);

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
            double totalKg = inputDouble("\n생산할 총 발효 배치량 입력 (kg): ");
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

