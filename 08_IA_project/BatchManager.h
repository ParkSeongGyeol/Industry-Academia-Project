#pragma once
#include <string>
#include <vector>

struct FermentationBatch {
    std::string batch_id{};
    std::string start_date{};
    std::string particle_size{};
    std::string yeast_type{};
    std::string ingredients{};
    double amount_liters = 0.0;
    double temperature = 0.0;
    int humidity = 0;
    int duration_hours = 0;

    std::string toCSV() const;
};

class BatchManager {
private:
    std::vector<FermentationBatch> batches;

public:
    std::string getSummary();                           // 대시보드용 요약
    std::vector<FermentationBatch> getDummyBatches();   // 테스트용 배치
    void displayBatches(const std::vector<FermentationBatch>& batches);

    void showBatchPage();                               // 배치 관리 메뉴 실행
    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    double predictFermentation(const FermentationBatch& batch);  // 발효 정도 예측 계산
    void predictBatchFermentation();                             // 사용자 선택 기반 예측

    void exportFermentationStatusToCSV();  //  CSV 출력 기능
};
