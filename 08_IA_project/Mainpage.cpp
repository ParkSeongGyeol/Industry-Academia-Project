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
    // [1] ��� �Ŵ��� �ν��Ͻ� ����(����)
    RawMaterialManager rawMgr;
    BatchManager batchMgr;
    SpiritManager spiritMgr;
    OakAgingManager oakMgr;
    BottledWhiskyManager bottleMgr;
    RecipeManager recipeMgr;

    // [2] CSV ������ �ε�
    rawMgr.loadMaterialsFromCSV("rawmaterial_dummy.csv");
    batchMgr.loadBatchesFromCSV("batch_dummy.csv");
    spiritMgr.loadSpiritsFromCSV("spirit_dummy.csv");
    oakMgr.loadOakBoxesFromCSV("oakaging_dummy.csv");
    bottleMgr.loadInventoryFromCSV("bottledwhisky_dummy.csv");
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        displayDashboard(batchMgr, rawMgr, spiritMgr, oakMgr, bottleMgr);
        cout << "\n�޴� ��ȣ�� �Է��ϼ��� >> ";
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
        "[1] ����� ����",
        "[2] ��ġ ����",
        "[3] ���Ǹ� ����",
        "[4] ��ũ�� ���� ����",
        "[5] ���� �� �ϼ�ǰ ����",
        "[0] ����"
    };
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    cout << "=== ����Ű ���� ���� �ý��� ===\n\n";
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
        cout << "���α׷��� �����մϴ�.\n";
        break;
    default:
        cout << "�߸��� �����Դϴ�. 0~5 ������ ��ȣ�� �Է��ϼ���.\n";
        pauseConsole();
        break;
    }
}
