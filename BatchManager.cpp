#include "BatchManager.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<FermentationBatch> BatchManager::loadBatchesFromFile(const std::string& filename) {
    std::vector<FermentationBatch> batches;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return batches;
    }

    json j;
    file >> j;

    for (const auto& item : j) {
        FermentationBatch batch;
        batch.batch_id = item["batch_id"];
        batch.start_date = item["start_date"];
        batch.temperature = item["temperature"];
        batch.humidity = item["humidity"];
        batch.duration_hours = item["duration_hours"];
        batches.push_back(batch);
    }

    return batches;
}

void BatchManager::displayBatches(const std::vector<FermentationBatch>& batches) {
    std::cout << "=== 발효 배치 목록 ===" << std::endl;
    for (const auto& batch : batches) {
        std::cout << "Batch ID: " << batch.batch_id << std::endl;
        std::cout << "Start Date: " << batch.start_date << std::endl;
        std::cout << "Temperature: " << batch.temperature << "°C" << std::endl;
        std::cout << "Humidity: " << batch.humidity << "%" << std::endl;
        std::cout << "Duration: " << batch.duration_hours << " hours" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}
