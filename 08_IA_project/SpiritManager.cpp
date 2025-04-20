#include "SpiritManager.h"
#include "UIUtils.h"
#include <iostream>
#include <algorithm>

std::string SpiritManager::getSummary() {
    size_t count = spirits.size();
    double totalYield = 0, totalAbv = 0;

    for (const auto& s : spirits) {
        totalYield += s.yield_liters;
        totalAbv += s.alcohol_percentage;
    }

    std::string result = "스피릿: " + std::to_string(count) + "종";
    if (count > 0) {
        result += " / 평균 도수: " + std::to_string(static_cast<int>(totalAbv / count)) + "%";
    }
    return result;
}

std::vector<std::string> SpiritManager::getPageInfoLines() {
    std::vector<std::string> lines;

    size_t count = spirits.size();
    double totalYield = 0;
    double totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.yield_liters;
        totalAbv += s.alcohol_percentage;
    }

    lines.push_back("등록된 스피릿 수: " + std::to_string(count) + "개");
    if (count > 0) {
        lines.push_back("총 생산량: " + std::to_string((int)totalYield) + "L");
        lines.push_back("평균 도수: " + std::to_string((int)(totalAbv / count)) + "%");
    }
    else {
        lines.push_back("총 생산량: 0L");
        lines.push_back("평균 도수: -");
    }

    return lines;
}

void SpiritManager::showSpiritPage() {
    int choice;
    do {
        system("cls");
        std::cout << "=== 스피릿 관리 메뉴 ===\n\n";

        std::vector<std::string> infoLines = getPageInfoLines();
        std::vector<std::string> menu = {
            "[1] 스피릿 목록 보기",
            "[2] 스피릿 추가",
            "[3] 스피릿 삭제",
            "[4] 스피릿 수정",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        std::cout << "\n입력 >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 0: std::cout << "메인으로 돌아갑니다...\n"; break;
        default: std::cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            std::cout << "\n계속하려면 Enter를 누르세요...";
            std::cin.ignore(); std::cin.get();
        }

    } while (choice != 0);
}

void SpiritManager::addSpirit() {
    Spirit spirit;
    std::cout << "\n=== 스피릿 추가 ===\n";
    std::cout << "ID: "; std::cin >> spirit.id;
    std::cin.ignore();
    std::cout << "보관 장소: "; std::getline(std::cin, spirit.storage_location);
    std::cout << "이동 기록: "; std::getline(std::cin, spirit.transfer_history);
    std::cout << "생산 날짜 (YYYY-MM-DD): "; std::getline(std::cin, spirit.production_date);
    std::cout << "도수(%): "; std::cin >> spirit.alcohol_percentage;
    std::cout << "생산량(L): "; std::cin >> spirit.yield_liters;
    std::cin.ignore();
    std::cout << "원재료 함량: "; std::getline(std::cin, spirit.raw_material_ratio);
    std::cout << "발효 기간(일): "; std::cin >> spirit.fermentation_days;
    std::cin.ignore();
    std::cout << "초류 끊은 시점: "; std::getline(std::cin, spirit.first_cut_time);
    std::cout << "후류 그만 받은 시점: "; std::getline(std::cin, spirit.last_cut_time);
    std::cout << "증류 횟수: "; std::cin >> spirit.distillation_count;

    spirits.push_back(spirit);
    std::cout << "? 스피릿 추가 완료!\n";
}

void SpiritManager::deleteSpirit() {
    std::string id;
    std::cout << "삭제할 스피릿 ID 입력: ";
    std::cin >> id;

    auto it = std::remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.id == id;
        });

    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        std::cout << "? 삭제 완료!\n";
    }
    else {
        std::cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
    }
}

void SpiritManager::displaySpirits() {
    std::cout << "\n=== 스피릿 목록 ===\n";
    for (const auto& s : spirits) {
        std::cout << "ID: " << s.id << "\n"
            << "보관 장소: " << s.storage_location << "\n"
            << "이동 기록: " << s.transfer_history << "\n"
            << "생산 날짜: " << s.production_date << "\n"
            << "도수: " << s.alcohol_percentage << "%\n"
            << "생산량: " << s.yield_liters << "L\n"
            << "원재료 함량: " << s.raw_material_ratio << "\n"
            << "발효 기간: " << s.fermentation_days << "일\n"
            << "초류 끊은 시점: " << s.first_cut_time << "\n"
            << "후류 종료 시점: " << s.last_cut_time << "\n"
            << "증류 횟수: " << s.distillation_count << "\n"
            << "--------------------------\n";
    }
}

void SpiritManager::updateSpirit() {
    std::string id;
    std::cout << "\n수정할 스피릿 ID 입력: ";
    std::cin >> id;

    for (auto& s : spirits) {
        if (s.id == id) {
            std::cin.ignore();
            std::cout << "=== 스피릿 정보 수정 ===\n";

            std::cout << "보관 장소 (" << s.storage_location << "): ";
            std::getline(std::cin, s.storage_location);

            std::cout << "이동 기록 (" << s.transfer_history << "): ";
            std::getline(std::cin, s.transfer_history);

            std::cout << "생산 날짜 (" << s.production_date << "): ";
            std::getline(std::cin, s.production_date);

            std::cout << "도수 (%) (" << s.alcohol_percentage << "): ";
            std::cin >> s.alcohol_percentage;

            std::cout << "생산량 (L) (" << s.yield_liters << "): ";
            std::cin >> s.yield_liters;

            std::cin.ignore();
            std::cout << "원재료 함량 (" << s.raw_material_ratio << "): ";
            std::getline(std::cin, s.raw_material_ratio);

            std::cout << "발효 기간 (일) (" << s.fermentation_days << "): ";
            std::cin >> s.fermentation_days;
            std::cin.ignore();

            std::cout << "초류 끊은 시점 (" << s.first_cut_time << "): ";
            std::getline(std::cin, s.first_cut_time);

            std::cout << "후류 종료 시점 (" << s.last_cut_time << "): ";
            std::getline(std::cin, s.last_cut_time);

            std::cout << "증류 횟수 (" << s.distillation_count << "): ";
            std::cin >> s.distillation_count;

            std::cout << "? 스피릿 수정 완료!\n";
            return;
        }
    }

    std::cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
}

