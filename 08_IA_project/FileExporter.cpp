#include "FileExporter.h"
#include <fstream>
#include <iostream>

// CSV 출력 기능
bool FileExporter::exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "BatchID,StartDate,Temperature,Humidity,Progress\n";
    for (const auto& batch : batches) {
        file << batch.toCSV() << "\n";
    }

    file.close();
    return true;
}

//  테스트용 main 함수 추가 (원래 main.cpp 내용)
#ifdef _DEBUG || TEST_MODE

int main() {
    std::vector<FermentationBatch> batches = {
        FermentationBatch("F001", "2025-04-15", 25.4, 60.2, 75),
        FermentationBatch("F002", "2025-04-14", 26.1, 58.0, 90)
    };

    if (FileExporter::exportToCSV(batches, "fermentation_status.csv")) {
        std::cout << "발효 상태가 성공적으로 출력되었습니다." << std::endl;
    }
    else {
        std::cout << "출력 실패!" << std::endl;
    }

    return 0;
}

#endif
