#pragma once
#include <string>
#include <vector>

struct FermentationBatch {
    std::string batch_id;
    std::string start_date;
    double temperature;
    int humidity;
    int duration_hours;
};

class BatchManager {
public:
    //std::vector<FermentationBatch> loadBatchesFromFile(const std::string& filename);
    // 라이브러리 없이 통합된 시스템에서 개발 진행하기 위한 코드 변경
    std::vector<FermentationBatch> getDummyBatches();
    void displayBatches(const std::vector<FermentationBatch>& batches);
};

