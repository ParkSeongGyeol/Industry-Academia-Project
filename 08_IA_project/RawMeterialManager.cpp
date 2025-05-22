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

using namespace std;

// 현재 시스템 날짜를 "YYYY-MM-DD" 형식으로 반환
string getCurrentDate() {
    time_t now = time(nullptr); // 현재 시간(초 단위) 가져오기
    tm t;
    localtime_s(&t, &now); // (time_t)를 지역 시간 구조체 tm으로 변환
	char buf[11]; // 날짜 형식: "YYYY-MM-DD" (10자 + 널 종료)
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// 특정 원재료 이름에 대해 현재 출고되지 않은 총 재고량을 계산하여 반환하는 함수
double RawMaterialManager::getStock(const string& name) {
    double total = 0;
    for (const auto& m : materials) {
        if (m.name == name && m.exit_date.empty())
            total += m.weight_kg;
    }
    return total;
}

// 주어진 이름의 원재료에서 지정된 양을 차감하여 출고하는 함수
void RawMaterialManager::consumeMaterial(const string& name, double amount) {
    for (auto& m : materials) {
        if (m.name == name && m.exit_date.empty()) {
            if (m.weight_kg >= amount) {
                m.weight_kg -= amount;
                if (m.weight_kg == 0)
                    m.exit_date = getCurrentDate();
                break;
            }
            else {
                amount -= m.weight_kg;
                m.exit_date = getCurrentDate();
                m.weight_kg = 0;
            }
        }
    }
}

// 사용된 원재료 목록을 지정된 파일 이름의 CSV 파일로 저장하는 함수
bool RawMaterialManager::exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // 전체 필드에 대한 CSV 헤더 작성
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
        << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate,UsedDate\n";

    // 각 원재료 항목을 한 줄씩 출력
    for (const auto& m : usedList) {
        file << m.material_id << "," << m.name << "," << m.type << "," << m.origin << ","
            << m.weight_kg << "," << m.storage_location << "," << m.storage_method << ","
            << m.expiry_date << "," << m.entry_date << "," << m.exit_date << ","
            << m.status << "," << m.unit << "," << m.unit_price << ","
            << m.entry_manager << "," << m.exit_manager << ","
            << m.quality_check << "," << m.quality_check_date << ","
            << getCurrentDate() << "\n";  // UsedDate
    }

    file.close();
    return true;
}

// 발효 배치 생산을 위한 원재료 소모 처리 함수
// totalBatchKg: 총 배치 생산량(kg)
// 필요한 원재료 비율: 보리 60%, 호밀 10%, 물 30%
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

    vector<RawMaterial> used = {
    {
        "USED001", "보리", "곡물", "스코틀랜드", requiredBarley,
        "창고 A", "저온 건조", "", "", getCurrentDate(),
        "출고", "kg", 950.0, "", "자동출고", "자동 검사", getCurrentDate()
    },
    {
        "USED002", "호밀", "곡물", "미국", requiredRye,
        "창고 A", "상온 보관", "", "", getCurrentDate(),
        "출고", "kg", 900.0, "", "자동출고", "자동 검사", getCurrentDate()
    },
    {
        "USED003", "물", "기타", "제조 현장", requiredWater,
        "탱크 B", "청결 밀봉", "-", "", getCurrentDate(),
        "출고", "L", 100.0, "", "자동출고", "검사 없음", "-"
    }
    };


    exportUsedMaterialsToCSV("used_raw_materials.csv", used);

    cout << "발효 배치용 원재료가 CSV에 저장되었습니다.\n";
    return true;
}

// 현재 재고 요약 문자열 생성
string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.exit_date.empty())  // 출고되지 않은 경우
            totalKg += item.weight_kg;

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
}

// 원재료 페이지 상단 정보 요약 (종류, 무게, 종류별 수량, 보관 위치 등)
vector<string> RawMaterialManager::getPageInfoLines() {
    int totalKinds = 0;
    double totalWeight = 0;
    set<string> storagePlaces;
    map<string, int> typeCount;

    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            ++totalKinds;
            totalWeight += m.weight_kg;
            storagePlaces.insert(m.storage_location);
            typeCount[m.type]++;
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

// 더미 데이터 초기화 (샘플 원재료 입력)
void RawMaterialManager::initializeDummyData() {
    materials = {
        {
            "RM001", "보리", "곡물", "스코틀랜드", 1200,
            "창고 A", "저온 건조", "2025-12-01", "2025-03-01", "",
            "정상", "kg", 950.0, "김입고", "", "통과", "2025-03-01"
        },
        {
            "RM002", "호밀", "곡물", "미국", 800,
            "창고 A", "상온 보관", "2025-11-15", "2025-03-03", "",
            "정상", "kg", 900.0, "박입고", "", "통과", "2025-03-03"
        },
        {
            "RM003", "물", "기타", "제조 현장", 5000,
            "탱크 B", "청결 밀봉", "-", "2025-03-02", "",
            "정상", "L", 100.0, "최입고", "", "검사 없음", "-"
        },
        {
            "RM004", "보리", "곡물", "스코틀랜드", 900,
            "창고 A", "저온 건조", "2025-10-10", "2025-01-10", "2025-03-01",
            "출고", "kg", 950.0, "김입고", "이출고", "통과", "2025-01-10"
        }
    };
}


// 출고되지 않은 원재료만 출력
void RawMaterialManager::showInventory() {
    cout << "\n=== 현재 보유 원재료 목록 ===\n";
    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            cout << "ID: " << m.material_id << "\n"
                << "이름: " << m.name << "\n"
                << "종류: " << m.type << "\n"
                << "원산지: " << m.origin << "\n"
                << "재고: " << m.weight_kg << " " << m.unit << "\n"
                << "단가: " << m.unit_price << "원/" << m.unit << "\n"
                << "보관 위치: " << m.storage_location << "\n"
                << "보관 방법: " << m.storage_method << "\n"
                << "유통기한: " << m.expiry_date << "\n"
                << "입고일: " << m.entry_date << "\n"
                << "입고 담당자: " << m.entry_manager << "\n"
                << "품질 검사: " << m.quality_check << " (" << m.quality_check_date << ")\n"
                << "-----------------------------\n";
        }
    }
}


// 전체 이력 출력 (출고된 항목 포함)
void RawMaterialManager::showAllMaterials() {
    cout << "\n=== 전체 원재료 입출고 이력 ===\n";
    for (const auto& m : materials) {
        cout << "ID: " << m.material_id << "\n"
            << "이름: " << m.name << "\n"
            << "종류: " << m.type << "\n"
            << "원산지: " << m.origin << "\n"
            << "무게: " << m.weight_kg << " " << m.unit << "\n"
            << "단가: " << m.unit_price << "원/" << m.unit << "\n"
            << "보관 위치: " << m.storage_location << "\n"
            << "보관 방법: " << m.storage_method << "\n"
            << "유통기한: " << m.expiry_date << "\n"
            << "입고일: " << m.entry_date << "\n"
            << "출고일: " << (m.exit_date.empty() ? "-" : m.exit_date) << "\n"
            << "상태: " << m.status << "\n"
            << "입고 담당자: " << m.entry_manager << "\n"
            << "출고 담당자: " << (m.exit_manager.empty() ? "-" : m.exit_manager) << "\n"
            << "품질 검사: " << m.quality_check << " (" << m.quality_check_date << ")\n"
            << "-----------------------------\n";
    }
}


// 원재료 관리 메뉴 루프
void RawMaterialManager::showRawMaterialPage() {
    initializeDummyData();  // 더미 데이터 로드

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

// 원재료 추가
void RawMaterialManager::addMaterial() {
    RawMaterial newItem;
    cin.ignore();  // 입력 버퍼 초기화
    cout << "\n=== 원재료 추가 ===\n";
    cout << "이름: "; getline(cin, newItem.name);
    cout << "종류: "; getline(cin, newItem.type);
    cout << "출신 지역: "; getline(cin, newItem.origin);
    cout << "무게(kg): "; cin >> newItem.weight_kg; cin.ignore();
    cout << "보관 위치: "; getline(cin, newItem.storage_location);
    cout << "보관 방법: "; getline(cin, newItem.storage_method);
    cout << "유통기한: "; getline(cin, newItem.expiry_date);
    cout << "입고일(YYYY-MM-DD): "; getline(cin, newItem.entry_date);
    newItem.exit_date = "";

    materials.push_back(newItem);
    cout << "원재료가 추가되었습니다.\n";
}

// 원재료 수정
void RawMaterialManager::updateMaterial() {
    string name;
    cin.ignore();
    cout << "\n수정할 원재료 이름 입력: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.name == name && m.exit_date.empty()) {
            cout << "=== 원재료 수정 ===\n";

            cout << "보관 위치 (" << m.storage_location << "): ";
            getline(cin, m.storage_location);

            cout << "보관 방법 (" << m.storage_method << "): ";
            getline(cin, m.storage_method);

            cout << "유통기한 (" << m.expiry_date << "): ";
            getline(cin, m.expiry_date);

            cout << "무게(kg) (" << m.weight_kg << "): ";
            cin >> m.weight_kg; cin.ignore();

            cout << "수정 완료.\n";
            return;
        }
    }
    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

// 출고 처리 (exit_date 설정)
void RawMaterialManager::deleteMaterial() {
    string name;
    cin.ignore();
    cout << "\n출고 처리할 원재료 이름 입력: ";
    getline(cin, name);

    for (auto& m : materials) {
        if (m.name == name && m.exit_date.empty()) {
            cout << "출고일 입력 (YYYY-MM-DD): ";
            getline(cin, m.exit_date);
            cout << "출고 처리 완료.\n";
            return;
        }
    }

    cout << "해당 이름의 원재료(재고)를 찾을 수 없습니다.\n";
}

// 이름으로 검색
void RawMaterialManager::searchMaterial() {
    string name;
    cin.ignore();
    cout << "\n검색할 원재료 이름 입력: ";
    getline(cin, name);

    bool found = false;
    for (const auto& m : materials) {
        if (m.name == name) {
            cout << "이름: " << m.name << "\n"
                << "종류: " << m.type << "\n"
                << "출신: " << m.origin << "\n"
                << "무게: " << m.weight_kg << "kg\n"
                << "보관 위치: " << m.storage_location << "\n"
                << "보관 방법: " << m.storage_method << "\n"
                << "유통기한: " << m.expiry_date << "\n"
                << "입고일: " << m.entry_date << "\n"
                << "출고일: " << (m.exit_date.empty() ? "-" : m.exit_date) << "\n"
                << "-----------------------------\n";
            found = true;
        }
    }

    if (!found)
        cout << "해당 이름의 원재료를 찾을 수 없습니다.\n";
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
        storage.displayInfo();  // StorageEnvironment 객체의 정보 출력
    }

    cout << "\n계속하려면 Enter를 누르세요...";
    cin.ignore(); cin.get();
}

// 사용된 원재료 목록을 CSV 파일로 내보내기
void RawMaterialManager::exportUsedInventoryToCSV() {
    vector<RawMaterial> usedList;

    for (const auto& m : materials) {
        if (!m.exit_date.empty()) {
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

// 출고되지 않은 원재료 목록을 CSV 파일로 내보내기
void RawMaterialManager::exportRemainingStockToCSV() {
    vector<RawMaterial> stockList;

    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
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

// 품질 검사 미완료 원재료 목록 출력
void RawMaterialManager::showUninspectedMaterials() {
    cout << "\n=== 품질 검사 미완료 원재료 목록 ===\n";
    bool found = false;

    for (const auto& m : materials) {
        if (m.quality_check.empty() || m.quality_check == "검사 안함") {
            found = true;
            cout << "ID: " << m.material_id << "\n"
                << "이름: " << m.name << "\n"
                << "입고일: " << m.entry_date << "\n"
                << "보관 위치: " << m.storage_location << "\n"
                << "입고 담당자: " << m.entry_manager << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "모든 원재료는 품질 검사를 완료했습니다.\n";
    }
}

void RawMaterialManager::showMaterialsByManager() {
    string manager;
    cin.ignore();
    cout << "\n조회할 담당자 이름 입력: ";
    getline(cin, manager);

    bool found = false;
    cout << "\n=== " << manager << " 담당 입출고 이력 ===\n";

    for (const auto& m : materials) {
        if (m.entry_manager == manager || m.exit_manager == manager) {
            found = true;
            cout << "ID: " << m.material_id << "\n"
                << "이름: " << m.name << "\n"
                << "입고일: " << m.entry_date << " (입고 담당: " << m.entry_manager << ")\n"
                << "출고일: " << (m.exit_date.empty() ? "-" : m.exit_date)
                << " (출고 담당: " << (m.exit_manager.empty() ? "-" : m.exit_manager) << ")\n"
                << "상태: " << m.status << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "해당 담당자의 입출고 기록이 없습니다.\n";
    }
}
