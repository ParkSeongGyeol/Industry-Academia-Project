// -----------------------------------------------------------------------------
// RawMaterialManager.cpp
// 원재료 관리 기능 정의 파일 (레시피 연동 포함)
// -----------------------------------------------------------------------------

#include "RawMaterialManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
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
// CSV 파일 필드 개수 및 파일명 상수 정의
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

// 안전한 string 입력 함수
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// CSV 헤더를 파일에 출력
void writeCSVHeader(ofstream& file, bool withUsedDate = false) {
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
         << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate";
    if (withUsedDate) file << ",UsedDate";
    file << "\n";
}

// RawMaterial 객체의 정보를 CSV 한 줄로 파일에 출력
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

// ID로 원재료 검색 (출고되지 않은 것만 반환)
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

// ----------------------------- [2-1] 레시피 기반 배치 생산 기능 -----------------------------
/**
 * 레시피 기반 배치 생산
 * - 사용자가 레시피 ID를 입력하면, 해당 레시피를 RecipeManager에서 불러와서
 *   재고 검증 후, produceBatch를 호출하여 배치 생산을 처리한다.
 * - 재고 부족 시 안내 메시지 출력, 성공 시 배치ID 출력 및 재고 CSV 저장
 */
void RawMaterialManager::produceBatchByRecipe(RecipeManager& recipeMgr) {
    // 1. 레시피 목록 출력 및 선택
    recipeMgr.listRecipes();
    string recipeId = inputString("\n사용할 레시피 ID 입력: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        return;
    }

    // 2. 배치 생산량 입력
    double batchSize = inputDouble("생산할 배치량(kg): ");

    // 3. 재고 검증
    if (!recipe.validateRawMaterialStock(*this, batchSize)) {
        cout << "재고가 부족하여 배치 생산이 불가합니다.\n";
        return;
    }

    // 4. 배치 생산(원재료 차감 및 배치ID 생성)
    string batchId = recipe.produceBatch(*this, batchSize);
    saveMaterialsToCSV(RAW_MATERIAL_CSV);

    cout << "배치 생산 완료! 배치ID: " << batchId << "\n";
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

// 전체 원재료 요약 정보(종류/총량) 반환
string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.getExitDate().empty())
            totalKg += item.getWeightKg();

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
}

// 대시보드/메뉴용 정보 요약 라인 반환
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

// 현재 보유 원재료(출고되지 않은 것만) 목록 출력
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

// 전체 원재료 입출고 이력 출력
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

// 보관 장소 환경 정보 출력
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

// 출고된(사용된) 원재료를 CSV로 내보내기
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

// 출고되지 않은(보유 중인) 원재료를 CSV로 내보내기
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

// 원재료 신규 추가
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

// 원재료 정보 수정(이름으로 검색, 출고되지 않은 것만)
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

// 원재료 출고 처리(이름으로 검색, 출고일 입력)
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

// 원재료 이름으로 검색 및 상세 정보 출력
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

// 원재료 관리 메인 메뉴 (레시피 연동 포함)
void RawMaterialManager::showRawMaterialPage() {
    loadMaterialsFromCSV(RAW_MATERIAL_CSV);

    // 레시피 매니저 객체 생성 (실제 구현에서는 싱글턴/전역 등으로 관리 가능)
    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv"); // 레시피 목록 CSV에서 불러오기

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
            "[8] 레시피 기반 배치 생산",
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
        case 8:
            produceBatchByRecipe(recipeMgr);
            break;
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

