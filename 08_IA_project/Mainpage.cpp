#include "MainPage.h"
#include "UIUtils.h"
#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include <iostream>

using namespace UIUtils;

void MainPage::run() {
    int choice;

    do {
        displayDashboard();
        std::cout << "\n입력 >> ";
        std::cin >> choice;
        std::cin.ignore();

        handleSelection(choice);
    } while (choice != 0);
}

void MainPage::displayDashboard() {
    RawMaterialManager raw;
    BatchManager batch;
    SpiritManager spirit;
    OakAgingManager oak;
    BottledWhiskyManager bottle;

    std::vector<std::string> infoLines = {
        batch.getSummary(),           // ex) "배치 수: 2개"
        raw.getSummary(),             // ex) "원재료: 3종 / 300kg"
        spirit.getSummary(),          // ex) "스피릿: 2종 / 평균 도수: 64%"
        oak.getSummary(),             // ex) "숙성통 수: 2개"
        bottle.getSummary()           // ex) "병입: 10병 / 평균가: 23000원"
    };

    std::vector<std::string> menu = {
        "[1] 원재료 관리",
        "[2] 배치 관리",
        "[3] 스피릿 관리",
        "[4] 오크통 숙성 관리",
        "[5] 병입 및 완성품 관리",
        "[0] 종료"
    };

    system("cls"); // Windows
    std::cout << "=== 위스키 생산 관리 시스템 ===\n\n";
    UIUtils::drawDashboard(infoLines, menu, 72, 30); // 넓이 고정

    //std::cout << "\n입력 >> ";
}




void MainPage::handleSelection(int choice) {
    switch (choice) {
    case 1: {
        RawMaterialManager raw;
        raw.showRawMaterialPage();
        break;
    }
    case 2: {
        BatchManager batch;
        auto list = batch.getDummyBatches();
        batch.displayBatches(list);
        break;
    }
    case 3: {
        SpiritManager s;
        s.showSpiritPage();
        break;
    }
    case 4: {
        OakAgingManager oak;
        oak.showOakAgingPage();
        break;
    }
    case 5: {
        BottledWhiskyManager b;
        b.showBottledWhiskyPage();
        break;
    }
    case 0:
        std::cout << "프로그램을 종료합니다.\n";
        break;
    default:
        std::cout << "잘못된 선택입니다.\n";
        break;
    }
}
