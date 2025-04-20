#include "MainPage.h"
#include "UIUtils.h"
#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include <algorithm>
#include <iostream>

using namespace UIUtils;

// 메인 루프 실행 함수
// 대시보드를 출력하고 사용자로부터 메뉴 번호를 입력받아 해당 기능을 수행
void MainPage::run() {
    int choice;

    do {
        displayDashboard();  // 시스템 요약 및 메뉴 출력
        std::cout << "\n메뉴 번호를 입력하세요 >> ";
        std::cin >> choice;
        std::cin.ignore();  // 입력 버퍼 정리

        handleSelection(choice);  // 선택 처리
    } while (choice != 0); // 0 입력 시 종료
}

// 대시보드 구성: 시스템 요약 정보 + 메뉴 출력
void MainPage::displayDashboard() {
    // 각 매니저 클래스에서 요약 정보 획득
    RawMaterialManager raw;
    BatchManager batch;
    SpiritManager spirit;
    OakAgingManager oak;
    BottledWhiskyManager bottle;

    // 정보 요약 라인 구성
    std::vector<std::string> infoLines = {
        batch.getSummary(),           // 예: "배치 수: 2개"
        raw.getSummary(),             // 예: "원재료: 3종 / 300kg"
        spirit.getSummary(),          // 예: "스피릿: 2종 / 평균 도수: 64%"
        oak.getSummary(),             // 예: "숙성통 수: 2개"
        bottle.getSummary()           // 예: "병입: 10병 / 평균가: 23000원"
    };

    // 메뉴 항목 구성
    std::vector<std::string> menu = {
        "[1] 원재료 관리",
        "[2] 배치 관리",
        "[3] 스피릿 관리",
        "[4] 오크통 숙성 관리",
        "[5] 병입 및 완성품 관리",
        "[0] 종료"
    };

    // 콘솔 화면 클리어 (OS에 따라 다르게 처리)
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "=== 위스키 생산 관리 시스템 ===\n\n";

    // UIUtils 유틸 함수로 대시보드 출력 (정보창, 메뉴창, 너비 72, 높이 30)
    UIUtils::drawDashboard(infoLines, menu, 72, 30);
}

// 메뉴 번호에 따라 해당 관리 페이지로 이동
void MainPage::handleSelection(int choice) {
    switch (choice) {
    case 1: {
        RawMaterialManager raw;
        raw.showRawMaterialPage();  // 원재료 관리 페이지
        break;
    }
    case 2: {
        BatchManager batch;
        batch.showBatchPage();      // 배치 관리 페이지
        break;
    }
    case 3: {
        SpiritManager spirit;
        spirit.showSpiritPage();    // 스피릿 관리 페이지
        break;
    }
    case 4: {
        OakAgingManager oak;
        oak.showOakAgingPage();     // 오크통 숙성 관리 페이지
        break;
    }
    case 5: {
        BottledWhiskyManager bottle;
        bottle.showBottledWhiskyPage();  // 병입 및 완성품 관리 페이지
        break;
    }
    case 0:
        std::cout << "프로그램을 종료합니다.\n";
        break;
    default:
        std::cout << "잘못된 선택입니다. 0~5 사이의 번호를 입력하세요.\n";
        pauseConsole(); // UIUtils의 사용자 입력 대기 함수 호출
        break;
    }
}
