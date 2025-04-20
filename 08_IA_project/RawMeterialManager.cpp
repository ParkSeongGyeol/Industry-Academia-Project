#include "RawMaterialManager.h"
#include "UIUtils.h"
#include "StorageEnvironment.h"
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>

using namespace std;

std::string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.exit_date.empty())
            totalKg += item.weight_kg;

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
}

std::vector<std::string> RawMaterialManager::getPageInfoLines() {
    int totalKinds = 0;
    double totalWeight = 0;
    std::set<std::string> storagePlaces;
    std::map<std::string, int> typeCount;

    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            ++totalKinds;
            totalWeight += m.weight_kg;
            storagePlaces.insert(m.storage_location);
            typeCount[m.type]++;
        }
    }

    std::vector<std::string> lines;
    lines.push_back("현재 보유 원재료: " + std::to_string(totalKinds) + "종 / " + std::to_string((int)totalWeight) + "kg");

    std::string typeSummary = "종류별 수량: ";
    for (const auto& pair : typeCount) {
        typeSummary += pair.first + ": " + std::to_string(pair.second) + "개  ";
    }
    lines.push_back(typeSummary);

    std::string storageSummary = "보관 위치: ";
    for (const auto& place : storagePlaces) {
        storageSummary += place + "  ";
    }
    lines.push_back(storageSummary);

    return lines;
}

void RawMaterialManager::initializeDummyData() {
    materials = {
        {"보리", "곡물", "스코틀랜드", 1200, "창고 A", "저온 건조", "2025-12-01", "2025-03-01", ""},
        {"호밀", "곡물", "미국", 800, "창고 A", "상온 보관", "2025-11-15", "2025-03-03", ""},
        {"물", "기타", "제조 현장", 5000, "탱크 B", "청결 밀봉", "-", "2025-03-02", ""},
        {"보리", "곡물", "스코틀랜드", 900, "창고 A", "저온 건조", "2025-10-10", "2025-01-10", "2025-03-01"}
    };
}

void RawMaterialManager::showInventory() {
    cout << "\n=== 현재 보유 원재료 목록 ===\n";
    for (const auto& m : materials) {
        if (m.exit_date.empty()) {
            cout << "이름: " << m.name << "\n"
                << "종류: " << m.type << "\n"
                << "출신: " << m.origin << "\n"
                << "재고: " << m.weight_kg << "kg\n"
                << "보관 위치: " << m.storage_location << "\n"
                << "보관 방법: " << m.storage_method << "\n"
                << "유통기한: " << m.expiry_date << "\n"
                << "입고일: " << m.entry_date << "\n"
                << "-----------------------------\n";
        }
    }
}

void RawMaterialManager::showAllMaterials() {
    cout << "\n=== 전체 원재료 입출고 이력 ===\n";
    for (const auto& m : materials) {
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
    }
}

void RawMaterialManager::showRawMaterialPage() {
    initializeDummyData();

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
        case 0: cout << "메인으로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}

void RawMaterialManager::addMaterial() {
    RawMaterial newItem;
    cin.ignore(); // 버퍼 초기화
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

    if (!found) {
        cout << "해당 이름의 원재료를 찾을 수 없습니다.\n";
    }
}

void RawMaterialManager::showStorageEnvironment() {
    std::vector<StorageEnvironment> storageList = {
        {"창고 A", 18.5f, 55.2f},
        {"지하 저장고", 12.0f, 70.0f},
        {"실험실 보관소", 22.3f, 40.0f}
    };

    std::cout << "\n=== 보관 장소 환경 정보 조회 ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }

    std::cout << "\n계속하려면 Enter를 누르세요...";
    std::cin.ignore(); std::cin.get();
}
