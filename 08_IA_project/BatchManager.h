#pragma once
#include <string>
#include <vector>

struct FermentationBatch {
    std::string batch_id;
    std::string start_date;
    std::string particle_size;
    std::string yeast_type;
    std::string ingredients;
    double amount_liters;
    double temperature;
    int humidity;
    int duration_hours;
};

class BatchManager {
private:
    std::vector<FermentationBatch> batches;

public:
    std::vector<FermentationBatch> getDummyBatches();
    void displayBatches(const std::vector<FermentationBatch>& batches);
    void run();  // 전체 시스템 메뉴 실행

    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();
};
