#pragma once
#include <string>
#include <vector>
#include <set>

// 발효 배치 구조체
struct FermentationBatch {
    std::string batch_id;
    std::string start_date;
    std::string particle_size;
    std::string yeast_type;
    std::string ingredients;
    double amount_liters = 0.0;
    double temperature = 0.0;
    int humidity = 0;
    int duration_hours = 0;

    std::string toCSV() const;
};

// 배치 관리 클래스
class BatchManager {
private:
    std::vector<FermentationBatch> batches;

public:
    // 요약 정보 반환 (메인 대시보드용)
    std::string getSummary();

    // 더미 데이터
    std::vector<FermentationBatch> getDummyBatches();

    // 기본 출력
    void displayBatches(const std::vector<FermentationBatch>& batches);

    // CRUD
    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    // 발효 예측
    double predictFermentation(const FermentationBatch& batch);
    void predictBatchFermentation();

    // CSV 출력
    void exportFermentationStatusToCSV();

    // 통계용 메서드
    int countFermentingBatches();
    double calculateAvgFermentationHours();
    std::set<std::string> getStorageLocations();

    // 메인 메뉴
    void showBatchPage();
};

// 유틸: set<string> → 한 줄로 변환
std::string joinStrings(const std::set<std::string>& items, const std::string& delimiter = " ");
