#pragma once

#include <vector>
#include <string>
#include <filesystem> // C++17 이상 필요

#include "BatchManager.h"

// 파일로 발효 배치 리스트를 CSV 형식으로 출력하는 유틸리티
class FileExporter {
public:
    // 배치 데이터를 CSV 파일로 저장
    static bool exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename);
};
