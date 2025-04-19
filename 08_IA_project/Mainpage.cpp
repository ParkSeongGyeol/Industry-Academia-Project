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

void MainPage::run() {
    int choice;

    do {
        displayDashboard();
        std::cout << "\n�޴� ��ȣ�� �Է��ϼ��� >> ";
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
        "[0] ����"
    };

#ifdef _WIN32
    system("cls");  // Windows
#else
    system("clear");  // Linux/Mac
#endif

    std::cout << "=== ����Ű ���� ���� �ý��� ===\n\n";
    UIUtils::drawDashboard(infoLines, menu, 72, 30); // �ʺ�, ���� ����
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
        SpiritManager spirit;
        spirit.showSpiritPage();
        break;
    }
    case 4: {
        OakAgingManager oak;
        oak.showOakAgingPage();
        break;
    }
    case 5: {
        BottledWhiskyManager bottle;
        bottle.showBottledWhiskyPage();
        break;
    }
    case 0:
        std::cout << "���α׷��� �����մϴ�.\n";
        break;
    default:
        std::cout << " �߸��� �����Դϴ�. 0~5 ������ ��ȣ�� �Է��ϼ���.\n";
        pauseConsole(); // ����� �Է� ���
        break;
    }
}
