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
        std::cout << "\n�Է� >> ";
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
        batch.getSummary(),           // ex) "��ġ ��: 2��"
        raw.getSummary(),             // ex) "�����: 3�� / 300kg"
        spirit.getSummary(),          // ex) "���Ǹ�: 2�� / ��� ����: 64%"
        oak.getSummary(),             // ex) "������ ��: 2��"
        bottle.getSummary()           // ex) "����: 10�� / ��հ�: 23000��"
    };

    std::vector<std::string> menu = {
        "[1] ����� ����",
        "[2] ��ġ ����",
        "[3] ���Ǹ� ����",
        "[4] ��ũ�� ���� ����",
        "[5] ���� �� �ϼ�ǰ ����",
        "[6] ��ȿ ���� CSV ���",                
        "[7] ���� ��� ���� ����",
        "[0] ����"
    };

    system("cls"); // Windows
    std::cout << "=== ����Ű ���� ���� �ý��� ===\n\n";
    UIUtils::drawDashboard(infoLines, menu, 72, 30); // ���� ����
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
        std::cout << "���α׷��� �����մϴ�.\n";
        break;
    default:
        std::cout << "�߸��� �����Դϴ�.\n";
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
        std::cout << "��ȿ ���°� ���������� ��µǾ����ϴ�.\n";
        std::cout << "���� ���: " << std::filesystem::current_path() / outputFilename << "\n";
    }
    else {
        std::cout << "��� ����! ������ �� �� �����ϴ�.\n";
    }
}
void MainPage::showStorageEnvironmentTest() {
    std::vector<StorageEnvironment> storageList = {
        {"â�� A", 18.5f, 55.2f},
        {"���� �����", 12.0f, 70.0f},
        {"����� ������", 22.3f, 40.0f}
    };

    std::cout << "=== ���� ��� ȯ�� ���� ��ȸ ===\n";
    for (const auto& storage : storageList) {
        std::cout << "-----------------------------\n";
        storage.displayInfo();
    }
}

