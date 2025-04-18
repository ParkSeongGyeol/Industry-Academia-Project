#include "FileExporter.h"
#include "BatchManager.h" // struct FermentationBatch 정의 포함
#include <fstream>
#include <iostream>
#include <filesystem> // C++17 이상 필요

// CSV 출력 기능
bool FileExporter::exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // CSV 헤더
    file << "BatchID,StartDate,ParticleSize,YeastType,Ingredients,Amount(L),Temperature,Humidity,Duration(H)\n";
    for (const auto& batch : batches) {
        file << batch.toCSV() << "\n";
    }

    file.close();
    return true;
}
