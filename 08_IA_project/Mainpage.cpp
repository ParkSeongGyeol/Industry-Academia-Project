#include "MainPage.h"
#include "BatchManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include <iostream>

void MainPage::run() {
    int choice;

    do {
        displayMenu();
        std::cout << "번호를 선택하세요: ";
        std::cin >> choice;
        std::cin.ignore(); // 엔터 입력 무시

        handleSelection(choice);
    } while (choice != 0);
}

void MainPage::displayMenu() {
    std::cout << "\n=== 위스키 생산 관리 시스템 ===\n";
    std::cout << "1. 원재료 항목 관리\n";
    std::cout << "2. 배치 관리\n";
    std::cout << "3. 스피릿 관리\n";
    std::cout << "4. 오크통 숙성 관리\n";
    std::cout << "5. 병입 및 완성품 관리\n";
    std::cout << "0. 종료\n";
}

void MainPage::handleSelection(int choice) {
    switch (choice) {
    case 1:
        std::cout << "\n[원재료 항목 관리 기능은 구현 예정입니다]\n";
        break;
    case 2: {
        BatchManager manager;
        auto batches = manager.getDummyBatches();
        manager.displayBatches(batches);
        break;
    }
    case 3:
        std::cout << "\n[스피릿 관리 기능은 구현 예정입니다]\n";
        break;
    case 4: {
        OakAgingManager oakManager;
        oakManager.OakListRun();
        break;
    }
    case 5: {
        BottledWhiskyManager WhiskyManager;
        WhiskyManager.BottledListRun();
        break;
    }
    case 0:
        std::cout << "\n프로그램을 종료합니다.\n";
        break;
    default:
        std::cout << "\n잘못된 선택입니다. 다시 입력해주세요.\n";
        break;
    }
}
