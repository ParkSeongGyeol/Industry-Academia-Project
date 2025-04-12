#include "MainPage.h"
#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include <iostream>

void MainPage::run() {
    int choice;

    do {
        displayMenu();
        std::cout << "��ȣ�� �����ϼ���: ";
        std::cin >> choice;
        std::cin.ignore(); // ���� �Է� ����

        handleSelection(choice);
    } while (choice != 0);
}

void MainPage::displayMenu() {
    std::cout << "\n=== ����Ű ���� ���� �ý��� ===\n";
    std::cout << "1. ����� �׸� ����\n";
    std::cout << "2. ��ġ ����\n";
    std::cout << "3. ���Ǹ� ����\n";
    std::cout << "4. ��ũ�� ���� ����\n";
    std::cout << "5. ���� �� �ϼ�ǰ ����\n";
    std::cout << "0. ����\n";
}

void MainPage::handleSelection(int choice) {
    switch (choice) {
    case 1: {
        RawMaterialManager rawManager;
        rawManager.showRawMaterials();
        break;
    }
    case 2: {
        BatchManager manager;
        auto batches = manager.getDummyBatches();
        manager.displayBatches(batches);
        break;
    }
    case 3: {
        SpiritManager spiritManager;
        spiritManager.run();
        break;
    }
    case 4: {
        OakAgingManager oakManager;
        oakManager.OakListRun();
        break;
    }
    case 5: {
        BottledWhiskyManager whiskyManager;
        whiskyManager.BottledListRun();
        break;
    }
    case 0:
        std::cout << "\n���α׷��� �����մϴ�.\n";
        break;
    default:
        std::cout << "\n�߸��� �����Դϴ�. �ٽ� �Է����ּ���.\n";
        break;
    }
}
