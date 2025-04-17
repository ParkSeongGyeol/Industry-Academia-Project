#include "RawMaterialManager.h"
#include <iostream>
#include "UIUtils.h"

using namespace std;

std::string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.exit_date.empty()) // 출고되지 않은 재고만
            totalKg += item.weight_kg;

    return "원재료: " + to_string(materials.size()) + "종 / " + to_string((int)totalKg) + "kg";
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

        vector<string> infoLines = { getSummary() };
        vector<string> menu = {
            "[1] 현재 보유 재고 보기",
            "[2] 전체 이력 보기",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: showAllMaterials(); break;
        case 0: cout << "메인으로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}
