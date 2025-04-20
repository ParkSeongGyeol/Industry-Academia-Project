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

// ���� ���� ���� �Լ�
// ��ú��带 ����ϰ� ����ڷκ��� �޴� ��ȣ�� �Է¹޾� �ش� ����� ����
void MainPage::run() {
    int choice;

    do {
        displayDashboard();  // �ý��� ��� �� �޴� ���
        std::cout << "\n�޴� ��ȣ�� �Է��ϼ��� >> ";
        std::cin >> choice;
        std::cin.ignore();  // �Է� ���� ����

        handleSelection(choice);  // ���� ó��
    } while (choice != 0); // 0 �Է� �� ����
}

// ��ú��� ����: �ý��� ��� ���� + �޴� ���
void MainPage::displayDashboard() {
    // �� �Ŵ��� Ŭ�������� ��� ���� ȹ��
    RawMaterialManager raw;
    BatchManager batch;
    SpiritManager spirit;
    OakAgingManager oak;
    BottledWhiskyManager bottle;

    // ���� ��� ���� ����
    std::vector<std::string> infoLines = {
        batch.getSummary(),           // ��: "��ġ ��: 2��"
        raw.getSummary(),             // ��: "�����: 3�� / 300kg"
        spirit.getSummary(),          // ��: "���Ǹ�: 2�� / ��� ����: 64%"
        oak.getSummary(),             // ��: "������ ��: 2��"
        bottle.getSummary()           // ��: "����: 10�� / ��հ�: 23000��"
    };

    // �޴� �׸� ����
    std::vector<std::string> menu = {
        "[1] ����� ����",
        "[2] ��ġ ����",
        "[3] ���Ǹ� ����",
        "[4] ��ũ�� ���� ����",
        "[5] ���� �� �ϼ�ǰ ����",
        "[0] ����"
    };

    // �ܼ� ȭ�� Ŭ���� (OS�� ���� �ٸ��� ó��)
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "=== ����Ű ���� ���� �ý��� ===\n\n";

    // UIUtils ��ƿ �Լ��� ��ú��� ��� (����â, �޴�â, �ʺ� 72, ���� 30)
    UIUtils::drawDashboard(infoLines, menu, 72, 30);
}

// �޴� ��ȣ�� ���� �ش� ���� �������� �̵�
void MainPage::handleSelection(int choice) {
    switch (choice) {
    case 1: {
        RawMaterialManager raw;
        raw.showRawMaterialPage();  // ����� ���� ������
        break;
    }
    case 2: {
        BatchManager batch;
        batch.showBatchPage();      // ��ġ ���� ������
        break;
    }
    case 3: {
        SpiritManager spirit;
        spirit.showSpiritPage();    // ���Ǹ� ���� ������
        break;
    }
    case 4: {
        OakAgingManager oak;
        oak.showOakAgingPage();     // ��ũ�� ���� ���� ������
        break;
    }
    case 5: {
        BottledWhiskyManager bottle;
        bottle.showBottledWhiskyPage();  // ���� �� �ϼ�ǰ ���� ������
        break;
    }
    case 0:
        std::cout << "���α׷��� �����մϴ�.\n";
        break;
    default:
        std::cout << "�߸��� �����Դϴ�. 0~5 ������ ��ȣ�� �Է��ϼ���.\n";
        pauseConsole(); // UIUtils�� ����� �Է� ��� �Լ� ȣ��
        break;
    }
}
