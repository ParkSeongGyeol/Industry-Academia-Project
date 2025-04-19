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
    return "배치 수: " + std::to_string(batches.size()) + "개";
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
    std::cout << "\n=== 발효 배치 목록 ===\n";
    for (const auto& batch : batches) {
        std::cout << "Batch ID: " << batch.batch_id << "\n";
        std::cout << "Start Date: " << batch.start_date << "\n";
        std::cout << "Particle Size: " << batch.particle_size << "\n";
        std::cout << "Yeast Type: " << batch.yeast_type << "\n";
        std::cout << "Ingredients: " << batch.ingredients << "\n";
        std::cout << "Amount (liters): " << batch.amount_liters << "\n";
        std::cout << "Temperature: " << batch.temperature << "°C\n";
        std::cout << "Humidity: " << batch.humidity << "%\n";
        std::cout << "Duration: " << batch.duration_hours << " hours\n";
        std::cout << "------------------------\n";
    }
    UIUtils::pauseConsole();
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
    std::cout << " 배치가 추가되었습니다.\n";
    UIUtils::pauseConsole();
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
            std::cin.ignore();
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

            std::cout << " 배치가 수정되었습니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    std::cout << " 해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
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
        std::cout << " 배치가 삭제되었습니다.\n";
    }
    else {
        std::cout << " 해당 ID의 배치를 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
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

    std::cout << " 해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

void BatchManager::predictBatchFermentation() {
    std::string id;
    std::cout << "예측할 배치 ID 입력: ";
    std::cin >> id;

    for (const auto& batch : batches) {
        if (batch.batch_id == id) {
            double prediction = predictFermentation(batch);
            std::cout << " 예측된 발효 정도: " << prediction << "% 입니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    std::cout << " 해당 ID의 배치를 찾을 수 없습니다.\n";
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
        std::cout << " 발효 상태가 성공적으로 CSV로 출력되었습니다.\n";
        std::cout << "저장 경로: " << std::filesystem::current_path() / filename << "\n";
    }
    else {
        std::cout << " CSV 출력 실패: 파일을 열 수 없습니다.\n";
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
        locations.insert("탱크");
    }
    return locations;
}

void BatchManager::showBatchPage() {
    batches = getDummyBatches();

    int choice;
    do {
        UIUtils::clearConsole();
        std::cout << "=== 발효 배치 관리 메뉴 ===\n\n";

        int total = static_cast<int>(batches.size());
        int fermenting = countFermentingBatches();
        int completed = total - fermenting;
        double avg = calculateAvgFermentationHours();
        auto locations = getStorageLocations();

        std::vector<std::string> infoLines = {
            "▶ 전체 배치 수: " + std::to_string(total) + "개",
            "▶ 상태 분포: 발효 중 " + std::to_string(fermenting) + "개 / 완료 " + std::to_string(completed) + "개",
            "▶ 평균 발효 시간: " + std::to_string(static_cast<int>(avg)) + "시간",
            "▶ 저장 위치: " + UIUtils::joinStrings(locations, " ")
        };

        std::vector<std::string> menu = {
            "[1] 배치 목록 조회",
            "[2] 새 배치 추가",
            "[3] 배치 정보 수정",
            "[4] 배치 삭제",
            "[5] 배치 검색",
            "[6] 발효 정도 예측",
            "[7] 발효 상태 CSV 출력",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);

        std::cout << "\n입력 >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 6: predictBatchFermentation(); break;
        case 7: exportFermentationStatusToCSV(); break;
        case 0: std::cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: UIUtils::printError("잘못된 입력입니다."); UIUtils::pauseConsole(); break;
        }

    } while (choice != 0);
}
