#include "BatchManager.h"
#include "FileExporter.h"
#include "UIUtils.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <set>

using namespace std;

std::string BatchManager::getSummary() {
    auto batches = getDummyBatches();
    return "��ġ ��: " + std::to_string(batches.size()) + "��";
}

std::string FermentationBatch::toCSV() const {
    return batch_id + "," + start_date + "," + particle_size + "," +
        yeast_type + "," + ingredients + "," +
        std::to_string(amount_liters) + "," +
        std::to_string(temperature) + "," +
        std::to_string(humidity) + "," +
        std::to_string(duration_hours);
}

std::vector<FermentationBatch> BatchManager::getDummyBatches() {
    return {
        {"B001", "2025-03-01", "medium", "saccharomyces", "rice, barley", 100.0, 22.5, 65, 72},
        {"B002", "2025-03-05", "fine", "brettanomyces", "corn, wheat", 85.0, 21.0, 70, 48}
    };
}

void BatchManager::displayBatches(const std::vector<FermentationBatch>& batches) {
    std::cout << "\n=== ��ȿ ��ġ ��� ===\n";
    for (const auto& batch : batches) {
        std::cout << "Batch ID: " << batch.batch_id << "\n";
        std::cout << "Start Date: " << batch.start_date << "\n";
        std::cout << "Particle Size: " << batch.particle_size << "\n";
        std::cout << "Yeast Type: " << batch.yeast_type << "\n";
        std::cout << "Ingredients: " << batch.ingredients << "\n";
        std::cout << "Amount (liters): " << batch.amount_liters << "\n";
        std::cout << "Temperature: " << batch.temperature << "��C\n";
        std::cout << "Humidity: " << batch.humidity << "%\n";
        std::cout << "Duration: " << batch.duration_hours << " hours\n";
        std::cout << "------------------------\n";
    }
    UIUtils::pauseConsole();
}

void BatchManager::addBatch() {
    FermentationBatch newBatch;
    std::cout << "=== �� ��ġ �߰� ===\n";
    std::cout << "Batch ID: "; std::cin >> newBatch.batch_id;
    std::cout << "Start Date (YYYY-MM-DD): "; std::cin >> newBatch.start_date;
    std::cout << "Particle Size: "; std::cin >> newBatch.particle_size;
    std::cout << "Yeast Type: "; std::cin >> newBatch.yeast_type;
    std::cin.ignore();
    std::cout << "Ingredients: "; std::getline(std::cin, newBatch.ingredients);
    std::cout << "Amount (liters): "; std::cin >> newBatch.amount_liters;
    std::cout << "Temperature (��C): "; std::cin >> newBatch.temperature;
    std::cout << "Humidity (%): "; std::cin >> newBatch.humidity;
    std::cout << "Duration (hours): "; std::cin >> newBatch.duration_hours;

    batches.push_back(newBatch);
    std::cout << " ��ġ�� �߰��Ǿ����ϴ�.\n";
    UIUtils::pauseConsole();
}

void BatchManager::updateBatch() {
    std::string id;
    std::cout << "������ ��ġ ID �Է�: ";
    std::cin >> id;

    for (auto& batch : batches) {
        if (batch.batch_id == id) {
            std::cout << "=== ������ ���� �Է� ===\n";
            std::cout << "Start Date (����: " << batch.start_date << "): ";
            std::cin >> batch.start_date;
            std::cout << "Particle Size (����: " << batch.particle_size << "): ";
            std::cin >> batch.particle_size;
            std::cout << "Yeast Type (����: " << batch.yeast_type << "): ";
            std::cin >> batch.yeast_type;
            std::cin.ignore();
            std::cout << "Ingredients (����: " << batch.ingredients << "): ";
            std::getline(std::cin, batch.ingredients);
            std::cout << "Amount (liters) (����: " << batch.amount_liters << "): ";
            std::cin >> batch.amount_liters;
            std::cout << "Temperature (����: " << batch.temperature << "): ";
            std::cin >> batch.temperature;
            std::cout << "Humidity (����: " << batch.humidity << "): ";
            std::cin >> batch.humidity;
            std::cout << "Duration (hours) (����: " << batch.duration_hours << "): ";
            std::cin >> batch.duration_hours;

            std::cout << " ��ġ�� �����Ǿ����ϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    std::cout << " �ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

void BatchManager::deleteBatch() {
    std::string id;
    std::cout << "������ ��ġ ID �Է�: ";
    std::cin >> id;

    auto it = std::remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) {
        return b.batch_id == id;
        });

    if (it != batches.end()) {
        batches.erase(it, batches.end());
        std::cout << " ��ġ�� �����Ǿ����ϴ�.\n";
    }
    else {
        std::cout << " �ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

void BatchManager::searchBatch() {
    std::string id;
    std::cout << "��ȸ�� ��ġ ID �Է�: ";
    std::cin >> id;

    for (const auto& batch : batches) {
        if (batch.batch_id == id) {
            std::vector<FermentationBatch> found = { batch };
            displayBatches(found);
            return;
        }
    }

    std::cout << " �ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

void BatchManager::predictBatchFermentation() {
    std::string id;
    std::cout << "������ ��ġ ID �Է�: ";
    std::cin >> id;

    for (const auto& batch : batches) {
        if (batch.batch_id == id) {
            double prediction = predictFermentation(batch);
            std::cout << " ������ ��ȿ ����: " << prediction << "% �Դϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    std::cout << " �ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

double BatchManager::predictFermentation(const FermentationBatch& batch) {
    double size_factor = 1.0;
    if (batch.particle_size == "fine") size_factor = 1.2;
    else if (batch.particle_size == "medium") size_factor = 1.0;
    else if (batch.particle_size == "coarse") size_factor = 0.8;

    double score = (batch.temperature * 0.8) + (batch.humidity * 0.2) + (batch.duration_hours * 1.5);
    score *= size_factor;

    return std::clamp(score, 0.0, 100.0);
}

void BatchManager::exportFermentationStatusToCSV() {
    std::string filename = "fermentation_status.csv";
    if (FileExporter::exportToCSV(batches, filename)) {
        std::cout << " ��ȿ ���°� ���������� CSV�� ��µǾ����ϴ�.\n";
        std::cout << "���� ���: " << std::filesystem::current_path() / filename << "\n";
    }
    else {
        std::cout << " CSV ��� ����: ������ �� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

int BatchManager::countFermentingBatches() {
    return static_cast<int>(std::count_if(batches.begin(), batches.end(), [](const FermentationBatch& b) {
        return b.duration_hours < 72;
        }));
}

double BatchManager::calculateAvgFermentationHours() {
    if (batches.empty()) return 0.0;
    int total = 0;
    for (const auto& b : batches) total += b.duration_hours;
    return static_cast<double>(total) / static_cast<double>(batches.size());
}

std::set<std::string> BatchManager::getStorageLocations() {
    std::set<std::string> locations;
    for (const auto& b : batches) {
        locations.insert("��ũ");
    }
    return locations;
}

void BatchManager::showBatchPage() {
    batches = getDummyBatches();

    int choice;
    do {
        UIUtils::clearConsole();
        std::cout << "=== ��ȿ ��ġ ���� �޴� ===\n\n";

        int total = static_cast<int>(batches.size());
        int fermenting = countFermentingBatches();
        int completed = total - fermenting;
        double avg = calculateAvgFermentationHours();
        auto locations = getStorageLocations();

        std::vector<std::string> infoLines = {
            "�� ��ü ��ġ ��: " + std::to_string(total) + "��",
            "�� ���� ����: ��ȿ �� " + std::to_string(fermenting) + "�� / �Ϸ� " + std::to_string(completed) + "��",
            "�� ��� ��ȿ �ð�: " + std::to_string(static_cast<int>(avg)) + "�ð�",
            "�� ���� ��ġ: " + UIUtils::joinStrings(locations, " ")
        };

        std::vector<std::string> menu = {
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

        std::cout << "\n�Է� >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 6: predictBatchFermentation(); break;
        case 7: exportFermentationStatusToCSV(); break;
        case 0: std::cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: UIUtils::printError("�߸��� �Է��Դϴ�."); UIUtils::pauseConsole(); break;
        }

    } while (choice != 0);
}
