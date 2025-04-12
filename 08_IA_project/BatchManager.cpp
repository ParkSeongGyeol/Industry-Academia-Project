#include "BatchManager.h"
#include <iostream>
#include <algorithm>

std::string BatchManager::getSummary() {
    auto batches = getDummyBatches();
    return "배치 수: " + std::to_string(batches.size()) + "개";
}

std::vector<FermentationBatch> BatchManager::getDummyBatches() {
    return {
        {"B001", "2025-03-01", "medium", "saccharomyces", "rice, barley", 100.0, 22.5, 65, 72},
        {"B002", "2025-03-05", "fine", "brettanomyces", "corn, wheat", 85.0, 21.0, 70, 48}
    };
}

void BatchManager::displayBatches(const std::vector<FermentationBatch>& batches) {
    std::cout << "\n=== 발효 배치 목록 ===" << std::endl;
    for (const auto& batch : batches) {
        std::cout << "Batch ID: " << batch.batch_id << std::endl;
        std::cout << "Start Date: " << batch.start_date << std::endl;
        std::cout << "Particle Size: " << batch.particle_size << std::endl;
        std::cout << "Yeast Type: " << batch.yeast_type << std::endl;
        std::cout << "Ingredients: " << batch.ingredients << std::endl;
        std::cout << "Amount (liters): " << batch.amount_liters << std::endl;
        std::cout << "Temperature: " << batch.temperature << "°C" << std::endl;
        std::cout << "Humidity: " << batch.humidity << "%" << std::endl;
        std::cout << "Duration: " << batch.duration_hours << " hours" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

void BatchManager::addBatch() {
    FermentationBatch newBatch;
    std::cout << "=== 새 배치 추가 ===\n";
    std::cout << "Batch ID: "; std::cin >> newBatch.batch_id;
    std::cout << "Start Date (YYYY-MM-DD): "; std::cin >> newBatch.start_date;
    std::cout << "Particle Size: "; std::cin >> newBatch.particle_size;
    std::cout << "Yeast Type: "; std::cin >> newBatch.yeast_type;
    std::cin.ignore();
    std::cout << "Ingredients: "; std::getline(std::cin, newBatch.ingredients);
    std::cout << "Amount (liters): "; std::cin >> newBatch.amount_liters;
    std::cout << "Temperature (°C): "; std::cin >> newBatch.temperature;
    std::cout << "Humidity (%): "; std::cin >> newBatch.humidity;
    std::cout << "Duration (hours): "; std::cin >> newBatch.duration_hours;

    batches.push_back(newBatch);
    std::cout << "? 배치가 추가되었습니다.\n";
}

void BatchManager::updateBatch() {
    std::string id;
    std::cout << "수정할 배치 ID 입력: ";
    std::cin >> id;

    for (auto& batch : batches) {
        if (batch.batch_id == id) {
            std::cout << "=== 수정할 정보 입력 ===\n";
            std::cout << "Start Date (현재: " << batch.start_date << "): ";
            std::cin >> batch.start_date;

            std::cout << "Particle Size (현재: " << batch.particle_size << "): ";
            std::cin >> batch.particle_size;

            std::cout << "Yeast Type (현재: " << batch.yeast_type << "): ";
            std::cin >> batch.yeast_type;

            std::cin.ignore(); // getline 전에 입력 버퍼 초기화
            std::cout << "Ingredients (현재: " << batch.ingredients << "): ";
            std::getline(std::cin, batch.ingredients);

            std::cout << "Amount (liters) (현재: " << batch.amount_liters << "): ";
            std::cin >> batch.amount_liters;

            std::cout << "Temperature (현재: " << batch.temperature << "): ";
            std::cin >> batch.temperature;

            std::cout << "Humidity (현재: " << batch.humidity << "): ";
            std::cin >> batch.humidity;

            std::cout << "Duration (hours) (현재: " << batch.duration_hours << "): ";
            std::cin >> batch.duration_hours;

            std::cout << "? 배치가 모두 수정되었습니다.\n";
            return;
        }
    }

    std::cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
}

void BatchManager::deleteBatch() {
    std::string id;
    std::cout << "삭제할 배치 ID 입력: ";
    std::cin >> id;

    auto it = std::remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) {
        return b.batch_id == id;
        });

    if (it != batches.end()) {
        batches.erase(it, batches.end());
        std::cout << "? 배치가 삭제되었습니다.\n";
    }
    else {
        std::cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    }
}

void BatchManager::searchBatch() {
    std::string id;
    std::cout << "조회할 배치 ID 입력: ";
    std::cin >> id;

    for (const auto& batch : batches) {
        if (batch.batch_id == id) {
            std::vector<FermentationBatch> found = { batch };
            displayBatches(found);
            return;
        }
    }

    std::cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
}

void BatchManager::run() {
    batches = getDummyBatches();

    int choice;
    do {
        std::cout << "\n======= 발효 배치 관리 메뉴 =======\n";
        std::cout << "1. 배치 목록 조회\n";
        std::cout << "2. 새 배치 추가\n";
        std::cout << "3. 배치 수정\n";
        std::cout << "4. 배치 삭제\n";
        std::cout << "5. 배치 검색\n";
        std::cout << "0. 종료\n";
        std::cout << "선택 >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 0: std::cout << "프로그램을 종료합니다.\n"; break;
        default: std::cout << "잘못된 입력입니다.\n"; break;
        }

    } while (choice != 0);
}
