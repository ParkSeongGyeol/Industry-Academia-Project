#include "MainPage.h"
#include "UIUtils.h"
#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include "RecipeManager.h"
#include <iostream>

using namespace std;
using namespace UIUtils;

void MainPage::run() {
    // [1] 모든 매니저 인스턴스 생성(공유)
    RawMaterialManager rawMgr;
    BatchManager batchMgr;
    SpiritManager spiritMgr;
    OakAgingManager oakMgr;
    BottledWhiskyManager bottleMgr;
    RecipeManager recipeMgr;

    // [2] CSV 데이터 로드
    rawMgr.loadMaterialsFromCSV("rawmaterial_dummy.csv");
    batchMgr.loadBatchesFromCSV("batch_dummy.csv");
    spiritMgr.loadSpiritsFromCSV("spirit_dummy.csv");
    oakMgr.loadOakBoxesFromCSV("oakaging_dummy.csv");
    bottleMgr.loadInventoryFromCSV("bottledwhisky_dummy.csv");
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        displayDashboard(batchMgr, rawMgr, spiritMgr, oakMgr, bottleMgr);
        cout << "\n메뉴 번호를 입력하세요 >> ";
        cin >> choice;
        cin.ignore();
        handleSelection(choice, rawMgr, batchMgr, spiritMgr, oakMgr, bottleMgr, recipeMgr);
    } while (choice != 0);
}

void MainPage::displayDashboard(BatchManager& batch, RawMaterialManager& raw, SpiritManager& spirit, OakAgingManager& oak, BottledWhiskyManager& bottle) {
    vector<string> infoLines = {
        batch.getSummary(),
        raw.getSummary(),
        spirit.getSummary(),
        oak.getSummary(),
        bottle.getSummary()
    };
    vector<string> menu = {
        "[1] 원재료 관리",
        "[2] 배치 관리",
        "[3] 스피릿 관리",
        "[4] 오크통 숙성 관리",
        "[5] 병입 및 완성품 관리",
        "[0] 종료"
    };
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    cout << "=== 위스키 생산 관리 시스템 ===\n\n";
    UIUtils::drawDashboard(infoLines, menu, 72, 30);
}

void MainPage::handleSelection(int choice, RawMaterialManager& rawMgr, BatchManager& batchMgr, SpiritManager& spiritMgr, OakAgingManager& oakMgr, BottledWhiskyManager& bottleMgr, RecipeManager& recipeMgr) {
    switch (choice) {
    case 1:
        rawMgr.showRawMaterialPage(recipeMgr);
        break;
    case 2:
        batchMgr.showBatchPage(recipeMgr, rawMgr);
        break;
    case 3:
        spiritMgr.showSpiritPage(recipeMgr);
        break;
    case 4:
        oakMgr.showOakAgingPage(recipeMgr);
        break;
    case 5:
        bottleMgr.showBottledWhiskyPage(recipeMgr);
        break;
    case 0:
        cout << "프로그램을 종료합니다.\n";
        break;
    default:
        cout << "잘못된 선택입니다. 0~5 사이의 번호를 입력하세요.\n";
        pauseConsole();
        break;
    }
}
