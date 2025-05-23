#include "BatchManager.h"
#include "FileExporter.h"
#include "UIUtils.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <set>

using namespace std;

//  ��ü ��ġ �� ����� ��ȯ
string BatchManager::getSummary() {
    auto batches = getDummyBatches();
    return "��ġ ��: " + to_string(batches.size()) + "��";
}


//  ��ġ ������ CSV ���� ���ڿ��� ��ȯ
string FermentationBatch::toCSV() const {
    return getBatchId() + "," + getStartDate() + "," + getParticleSize() + "," +
        getYeastType() + "," + getIngredients() + "," +
        to_string(getAmountLiters()) + "," +
        to_string(getTemperature()) + "," +
        to_string(getHumidity()) + "," +
        to_string(getDurationHours());
}


//  ���ÿ� ���� ��ġ ��� ��ȯ
vector<FermentationBatch> BatchManager::getDummyBatches() {
    FermentationBatch b1, b2;

    b1.setBatchId("B001");
    b1.setStartDate("2025-03-01");
    b1.setParticleSize("medium");
    b1.setYeastType("saccharomyces");
    b1.setIngredients("rice, barley");
    b1.setAmountLiters(100.0);
    b1.setTemperature(22.5);
    b1.setHumidity(65);
    b1.setDurationHours(72);

    b2.setBatchId("B002");
    b2.setStartDate("2025-03-05");
    b2.setParticleSize("fine");
    b2.setYeastType("brettanomyces");
    b2.setIngredients("corn, wheat");
    b2.setAmountLiters(85.0);
    b2.setTemperature(21.0);
    b2.setHumidity(70);
    b2.setDurationHours(48);

    return { b1, b2 };
}


//  ��ġ ��� ���
void BatchManager::displayBatches(const vector<FermentationBatch>& batches) {
    cout << "\n=== ��ȿ ��ġ ��� ===\n";
    for (const auto& b : batches) {
        cout << "��ġ ID: " << b.getBatchId() << "\n"
            << "������: " << b.getStartDate() << "\n"
            << "���� ũ��: " << b.getParticleSize() << "\n"
            << "ȿ�� ����: " << b.getYeastType() << "\n"
            << "���� ���: " << b.getIngredients() << "\n"
            << "�� (����): " << b.getAmountLiters() << "L\n"
            << "�µ�: " << b.getTemperature() << "��C\n"
            << "����: " << b.getHumidity() << "%\n"
            << "��ȿ �ð�: " << b.getDurationHours() << "�ð�\n"
            << "------------------------\n";
    }
    UIUtils::pauseConsole();
}



//  �� ��ġ �߰�
void BatchManager::addBatch() {
    FermentationBatch newBatch;
    string input;
    double dInput;
    int iInput;

    cout << "=== �� ��ȿ ��ġ �߰� ===\n";
    cout << "��ġ ID: ";
    cin >> input; newBatch.setBatchId(input);

    cout << "������ (YYYY-MM-DD): ";
    cin >> input; newBatch.setStartDate(input);

    cout << "���� ũ�� (��: fine, medium, coarse): ";
    cin >> input; newBatch.setParticleSize(input);

    cout << "ȿ�� ����: ";
    cin >> input; newBatch.setYeastType(input);

    cin.ignore();
    cout << "���� ��� (��: ����, ������): ";
    getline(cin, input); newBatch.setIngredients(input);

    cout << "�� (����): ";
    cin >> dInput; newBatch.setAmountLiters(dInput);

    cout << "�µ� (��C): ";
    cin >> dInput; newBatch.setTemperature(dInput);

    cout << "���� (%): ";
    cin >> iInput; newBatch.setHumidity(iInput);

    cout << "��ȿ �ð� (�ð�): ";
    cin >> iInput; newBatch.setDurationHours(iInput);

    batches.push_back(newBatch);
    cout << "\n��ȿ ��ġ�� �߰��Ǿ����ϴ�.\n";
    UIUtils::pauseConsole();
}


//  ��ġ ����
void BatchManager::updateBatch() {
    string id, input;
    double dInput;
    int iInput;

    cout << "������ ��ġ ID �Է�: ";
    cin >> id;

    for (auto& batch : batches) {
        if (batch.getBatchId() == id) {
            cout << "\n=== ��ġ ���� ���� ===\n";

            cout << "������ (" << batch.getStartDate() << "): ";
            cin >> input; batch.setStartDate(input);

            cout << "���� ũ�� (" << batch.getParticleSize() << "): ";
            cin >> input; batch.setParticleSize(input);

            cout << "ȿ�� ���� (" << batch.getYeastType() << "): ";
            cin >> input; batch.setYeastType(input);

            cin.ignore();
            cout << "���� ��� (" << batch.getIngredients() << "): ";
            getline(cin, input); batch.setIngredients(input);

            cout << "�� (����) (" << batch.getAmountLiters() << "): ";
            cin >> dInput; batch.setAmountLiters(dInput);

            cout << "�µ� (��C) (" << batch.getTemperature() << "): ";
            cin >> dInput; batch.setTemperature(dInput);

            cout << "���� (%) (" << batch.getHumidity() << "): ";
            cin >> iInput; batch.setHumidity(iInput);

            cout << "��ȿ �ð� (�ð�) (" << batch.getDurationHours() << "): ";
            cin >> iInput; batch.setDurationHours(iInput);

            cout << "\n ��ġ�� ���������� �����Ǿ����ϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}


//  ��ġ ����
void BatchManager::deleteBatch() {
    string id;
    cout << "������ ��ġ ID �Է�: ";
    cin >> id;

    auto it = remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) {
        return b.getBatchId() == id;
        });

    if (it != batches.end()) {
        batches.erase(it, batches.end());
        cout << "\n��ġ�� �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "\n�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}


//  ��ġ �˻�
void BatchManager::searchBatch() {
    string id;
    cout << "��ȸ�� ��ġ ID �Է�: ";
    cin >> id;

    for (const auto& batch : batches) {
        if (batch.getBatchId() == id) {
            vector<FermentationBatch> found = { batch };
            displayBatches(found);
            return;
        }
    }

    cout << "\n�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}


//  ��ȿ ����
void BatchManager::predictBatchFermentation() {
    string id;
    cout << "������ ��ġ ID �Է�: ";
    cin >> id;

    for (const auto& batch : batches) {
        if (batch.getBatchId() == id) {
            double prediction = predictFermentation(batch);
            cout << "\n������ ��ȿ ����: " << prediction << "% �Դϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    cout << "\n�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}


//  ��ȿ ���� �˰��� (������ ����ġ ��� ���)
double BatchManager::predictFermentation(const FermentationBatch& batch) {
    double size_factor = 1.0;
    string size = batch.getParticleSize();

    if (size == "fine") size_factor = 1.2;
    else if (size == "medium") size_factor = 1.0;
    else if (size == "coarse") size_factor = 0.8;

    double score = (batch.getTemperature() * 0.8) +
        (batch.getHumidity() * 0.2) +
        (batch.getDurationHours() * 1.5);
    score *= size_factor;

    return clamp(score, 0.0, 100.0);
}


//  CSV�� ��ȿ ���� ����
void BatchManager::exportFermentationStatusToCSV() {
    string filename = "fermentation_status.csv";
    if (FileExporter::exportToCSV(batches, filename)) {
        cout << "\n��ȿ ���°� CSV ���Ϸ� ���������� ����Ǿ����ϴ�.\n";
        cout << "���� ���: " << filesystem::current_path() / filename << "\n";
    }
    else {
        cout << "\nCSV ���� ����: ������ �� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}


//  ��ȿ ���� ��ġ �� ��� (72�ð� �̸��� ���)
int BatchManager::countFermentingBatches() {
    return static_cast<int>(count_if(batches.begin(), batches.end(), [](const FermentationBatch& b) {
        return b.getDurationHours() < 72;
        }));
}

//  ��� ��ȿ �ð� ���
double BatchManager::calculateAvgFermentationHours() {
    if (batches.empty()) return 0.0;
    int total = 0;
    for (const auto& b : batches)
        total += b.getDurationHours();
    return static_cast<double>(total) / static_cast<double>(batches.size());
}


//  ���� ��ġ �������� (����� "��ũ"�� ������)
set<string> BatchManager::getStorageLocations() {
    set<string> locations;
    for (const auto& b : batches) {
        locations.insert("��ũ"); // ���� batch.getStorageLocation() ���� Ȯ�� ����
    }
    return locations;
}


//  ��ġ ���� ������ ���� ����
void BatchManager::showBatchPage() {
    batches = getDummyBatches(); // ���� �� ���� ������ �ʱ�ȭ

    int choice;
    do {
        UIUtils::clearConsole();
        cout << "=== ��ȿ ��ġ ���� �޴� ===\n\n";

        int total = static_cast<int>(batches.size());
        int fermenting = countFermentingBatches();
        int completed = total - fermenting;
        double avg = calculateAvgFermentationHours();
        auto locations = getStorageLocations();

        vector<string> infoLines = {
            "��ü ��ġ ��: " + to_string(total) + "��",
            "���� ����: ��ȿ �� " + to_string(fermenting) + "�� / �Ϸ� " + to_string(completed) + "��",
            "��� ��ȿ �ð�: " + to_string(static_cast<int>(avg)) + "�ð�",
            "���� ��ġ: " + UIUtils::joinStrings(locations, " ")
        };

        vector<string> menu = {
            "[1] ��ġ ��� ��ȸ",
            "[2] �� ��ġ �߰�",
            "[3] ��ġ ���� ����",
            "[4] ��ġ ����",
            "[5] ��ġ �˻�",
            "[6] ��ȿ ���� ����",
            "[7] ��ȿ ���� CSV ���",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);

        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 6: predictBatchFermentation(); break;
        case 7: exportFermentationStatusToCSV(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: UIUtils::printError("�߸��� �Է��Դϴ�."); UIUtils::pauseConsole(); break;
        }

    } while (choice != 0);
}
