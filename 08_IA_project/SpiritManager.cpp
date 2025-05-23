#include "SpiritManager.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// ���Ǹ� ������ ��� ������ ����� ��ȯ
string SpiritManager::getSummary() {
    size_t count = spirits.size();
    double totalYield = 0, totalAbv = 0;

    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }

    string result = "���Ǹ�: " + to_string(count) + "��";
    if (count > 0) {
        result += " / ��� ����: " + to_string(static_cast<int>(totalAbv / count)) + "%";
    }
    return result;
}


// ������ ��� ����: ���Ǹ� ��, �� ���귮, ��� ���� ��
vector<string> SpiritManager::getPageInfoLines() {
    vector<string> lines;

    size_t count = spirits.size();
    double totalYield = 0;
    double totalAbv = 0;

    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }

    lines.push_back("��ϵ� ���Ǹ� ��: " + to_string(count) + "��");
    if (count > 0) {
        lines.push_back("�� ���귮: " + to_string((int)totalYield) + "L");
        lines.push_back("��� ����: " + to_string((int)(totalAbv / count)) + "%");
    }
    else {
        lines.push_back("�� ���귮: 0L");
        lines.push_back("��� ����: -");
    }

    return lines;
}


// �ܼ� UI ����: ����� ���ÿ� ���� ��� ����
void SpiritManager::showSpiritPage() {
    int choice;
    do {
        system("cls");
        cout << "=== ���Ǹ� ���� �޴� ===\n\n";

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ���Ǹ� ��� ����",
            "[2] ���Ǹ� �߰�",
            "[3] ���Ǹ� ����",
            "[4] ���Ǹ� ����",
            "[5] CSV�� ����",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 5: exportSpiritsToCSV("spirits.csv"); break;
        case 0: cout << "�������� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}


// ���Ǹ� ���� �߰� �Է� ó��
void SpiritManager::addSpirit() {
    Spirit spirit;
    string input;
    double doubleInput;
    int intInput;

    cout << "\n=== ���Ǹ� �߰� ===\n";

    cout << "ID: ";
    cin >> input; spirit.setId(input);
    cin.ignore();

    cout << "���� ���: ";
    getline(cin, input); spirit.setStorageLocation(input);

    cout << "�̵� ���: ";
    getline(cin, input); spirit.setTransferHistory(input);

    cout << "���� ��¥ (YYYY-MM-DD): ";
    getline(cin, input); spirit.setProductionDate(input);

    cout << "����(%): ";
    cin >> doubleInput; spirit.setAlcoholPercentage(doubleInput);

    cout << "���귮(L): ";
    cin >> doubleInput; spirit.setYieldLiters(doubleInput);
    cin.ignore();

    cout << "����� �Է�: ";
    getline(cin, input); spirit.setRawMaterialRatio(input);

    cout << "��ȿ �Ⱓ(��): ";
    cin >> intInput; spirit.setFermentationDays(intInput);
    cin.ignore();

    cout << "�ʷ� ���� ����: ";
    getline(cin, input); spirit.setFirstCutTime(input);

    cout << "�ķ� �׸� ���� ����: ";
    getline(cin, input); spirit.setLastCutTime(input);

    cout << "���� Ƚ��: ";
    cin >> intInput; spirit.setDistillationCount(intInput);
    cin.ignore();

    cout << "���� ũ�� (��: �̺и�, 2mm ���� ��): ";
    getline(cin, input); spirit.setParticleSize(input);

    cout << "��� ��ġ ID: ";
    getline(cin, input); spirit.setBatchId(input);

    cout << "�ʷ�/�ķ�/���� ���淮 ���: ";
    getline(cin, input); spirit.setCutYield(input);

    spirits.push_back(spirit);
    cout << "���Ǹ� �߰� �Ϸ�!\n";
}


// Ư�� ID�� �������� ���Ǹ� ����
void SpiritManager::deleteSpirit() {
    string id;
    cout << "������ ���Ǹ� ID �Է�: ";
    cin >> id;

    auto it = remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.getId() == id;
        });

    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        cout << "���� �Ϸ�!\n";
    }
    else {
        cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
    }
}


// ��ü ���Ǹ� ��� ���
void SpiritManager::displaySpirits() {
    cout << "\n=== ���Ǹ� ��� ===\n";
    for (const auto& s : spirits) {
        cout << "ID: " << s.getId() << "\n"
            << "���� ���: " << s.getStorageLocation() << "\n"
            << "�̵� ���: " << s.getTransferHistory() << "\n"
            << "���� ��¥: " << s.getProductionDate() << "\n"
            << "����: " << s.getAlcoholPercentage() << "%\n"
            << "���귮: " << s.getYieldLiters() << "L\n"
            << "����� �Է�: " << s.getRawMaterialRatio() << "\n"
            << "��ȿ �Ⱓ: " << s.getFermentationDays() << "��\n"
            << "�ʷ� ���� ����: " << s.getFirstCutTime() << "\n"
            << "�ķ� ���� ����: " << s.getLastCutTime() << "\n"
            << "���� Ƚ��: " << s.getDistillationCount() << "\n"
            << "���� ũ��: " << s.getParticleSize() << "\n"
            << "��� ��ġ ID: " << s.getBatchId() << "\n"
            << "�ʷ�/�ķ�/���� ���淮: " << s.getCutYield() << "\n"
            << "--------------------------\n";
    }
}



// ���Ǹ� ���� ����
void SpiritManager::updateSpirit() {
    string id;
    cout << "\n������ ���Ǹ� ID �Է�: ";
    cin >> id;

    for (auto& s : spirits) {
        if (s.getId() == id) {
            cin.ignore();
            cout << "=== ���Ǹ� ���� ���� ===\n";

            string input;
            double doubleInput;
            int intInput;

            cout << "���� ��� (" << s.getStorageLocation() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setStorageLocation(input);

            cout << "�̵� ��� (" << s.getTransferHistory() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setTransferHistory(input);

            cout << "���� ��¥ (" << s.getProductionDate() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setProductionDate(input);

            cout << "���� (%) (" << s.getAlcoholPercentage() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setAlcoholPercentage(stod(input));

            cout << "���귮 (L) (" << s.getYieldLiters() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setYieldLiters(stod(input));

            cout << "����� �Է� (" << s.getRawMaterialRatio() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setRawMaterialRatio(input);

            cout << "��ȿ �Ⱓ (��) (" << s.getFermentationDays() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setFermentationDays(stoi(input));

            cout << "�ʷ� ���� ���� (" << s.getFirstCutTime() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setFirstCutTime(input);

            cout << "�ķ� ���� ���� (" << s.getLastCutTime() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setLastCutTime(input);

            cout << "���� Ƚ�� (" << s.getDistillationCount() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setDistillationCount(stoi(input));

            cout << "���� ũ�� (" << s.getParticleSize() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setParticleSize(input);

            cout << "��� ��ġ ID (" << s.getBatchId() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setBatchId(input);

            cout << "�ʷ�/�ķ�/���� ���淮 (" << s.getCutYield() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setCutYield(input);

            cout << "���Ǹ� ���� �Ϸ�!\n";
            return;
        }
    }

    cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
}

void SpiritManager::exportSpiritsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "CSV ������ �� �� �����ϴ�.\n";
        return;
    }

    // ���
    file << "ID,StorageLocation,TransferHistory,ProductionDate,AlcoholPercentage,YieldLiters,"
        << "RawMaterialRatio,FermentationDays,FirstCutTime,LastCutTime,DistillationCount,"
        << "ParticleSize,BatchId,CutYield\n";

    for (const auto& s : spirits) {
        file << s.getId() << ","
            << s.getStorageLocation() << ","
            << s.getTransferHistory() << ","
            << s.getProductionDate() << ","
            << s.getAlcoholPercentage() << ","
            << s.getYieldLiters() << ","
            << s.getRawMaterialRatio() << ","
            << s.getFermentationDays() << ","
            << s.getFirstCutTime() << ","
            << s.getLastCutTime() << ","
            << s.getDistillationCount() << ","
            << s.getParticleSize() << ","
            << s.getBatchId() << ","
            << s.getCutYield() << "\n";
    }

    file.close();
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
}