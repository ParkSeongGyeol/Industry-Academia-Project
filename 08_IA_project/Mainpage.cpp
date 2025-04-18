#include "MainPage.h"
#include "UIUtils.h"
#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include "StorageEnvironment.h"
#include "FileExporter.h"
#include <algorithm>
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
        "[6] 발효 상태 CSV 출력",                
        "[7] 보관 장소 정보 보기",
        "[0] 종료"
    };

    system("cls"); // Windows
    std::cout << "=== 위스키 생산 관리 시스템 ===\n\n";
    UIUtils::drawDashboard(infoLines, menu, 72, 30); // 넓이 고정
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
        batch.showBatchPage();
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
    case 6:
        exportCSVTest();
        break;
    case 7:
        showStorageEnvironmentTest();
        break;
    case 0:
        std::cout << "프로그램을 종료합니다.\n";
        break;
    default:
        std::cout << "잘못된 선택입니다.\n";
        break;
    }
}
void MainPage::exportCSVTest() {
    std::vector<FermentationBatch> batches = {
        {"F001", "2025-04-15", "medium", "saccharomyces", "rice, barley", 100.0, 25.4, 60, 72},
        {"F002", "2025-04-14", "fine", "brettanomyces", "corn, wheat", 85.0, 26.1, 58, 48}
    };

    std::string outputFilename = "fermentation_status.csv";
    if (FileExporter::exportToCSV(batches, outputFilename)) {
        std::cout << "발효 상태가 성공적으로 출력되었습니다.\n";
        std::cout << "저장 경로: " << std::filesystem::current_path() / outputFilename << "\n";
    }
    else {
        std::cout << "출력 실패! 파일을 열 수 없습니다.\n";
    }
}
void MainPage::showStorageEnvironmentTest() {
    std::vector<StorageEnvironment> storageList = {
        {"창고 A", 18.5f, 55.2f},
        {"지하 저장고", 12.0f, 70.0f},
        {"실험실 보관소", 22.3f, 40.0f}
    };

    std::cout << "=== 보관 장소 환경 정보 조회 ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }
}

