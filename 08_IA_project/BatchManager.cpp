#include "BatchManager.h"
#include <iostream>
#include <algorithm>

std::string BatchManager::getSummary() {
    auto batches = getDummyBatches();
    return "��ġ ��: " + std::to_string(batches.size()) + "��";
}

std::vector<FermentationBatch> BatchManager::getDummyBatches() {
    return {
        {"B001", "2025-03-01", "medium", "saccharomyces", "rice, barley", 100.0, 22.5, 65, 72},
        {"B002", "2025-03-05", "fine", "brettanomyces", "corn, wheat", 85.0, 21.0, 70, 48}
    };
}

void BatchManager::displayBatches(const std::vector<FermentationBatch>& batches) {
    std::cout << "\n=== ��ȿ ��ġ ��� ===" << std::endl;
    for (const auto& batch : batches) {
        std::cout << "Batch ID: " << batch.batch_id << std::endl;
        std::cout << "Start Date: " << batch.start_date << std::endl;
        std::cout << "Particle Size: " << batch.particle_size << std::endl;
        std::cout << "Yeast Type: " << batch.yeast_type << std::endl;
        std::cout << "Ingredients: " << batch.ingredients << std::endl;
        std::cout << "Amount (liters): " << batch.amount_liters << std::endl;
        std::cout << "Temperature: " << batch.temperature << "��C" << std::endl;
        std::cout << "Humidity: " << batch.humidity << "%" << std::endl;
        std::cout << "Duration: " << batch.duration_hours << " hours" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
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
    std::cout << "? ��ġ�� �߰��Ǿ����ϴ�.\n";
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

            std::cin.ignore(); // getline ���� �Է� ���� �ʱ�ȭ
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

            std::cout << "? ��ġ�� ��� �����Ǿ����ϴ�.\n";
            return;
        }
    }

    std::cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
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
        std::cout << "? ��ġ�� �����Ǿ����ϴ�.\n";
    }
    else {
        std::cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    }
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

    std::cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
}

void BatchManager::run() {
    batches = getDummyBatches();

    int choice;
    do {
        std::cout << "\n======= ��ȿ ��ġ ���� �޴� =======\n";
        std::cout << "1. ��ġ ��� ��ȸ\n";
        std::cout << "2. �� ��ġ �߰�\n";
        std::cout << "3. ��ġ ����\n";
        std::cout << "4. ��ġ ����\n";
        std::cout << "5. ��ġ �˻�\n";
        std::cout << "0. ����\n";
        std::cout << "���� >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 0: std::cout << "���α׷��� �����մϴ�.\n"; break;
        default: std::cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

    } while (choice != 0);
}
