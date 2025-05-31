#pragma once
#include <string>
#include <vector>
#include <set>
using namespace std;

// FermentationBatch 클래스 정의
class FermentationBatch {
private:
    string batch_id;           // 배치 ID
    string start_date;         // 발효 시작일
    string end_date;           // 발효 완료일
    string expiry_date;        // 유통기한
    string particle_size;      // 입자 크기
    string yeast_type;         // 효모 타입
    string ingredient_info;    // 원재료 조성 (예: RM001:60%, RM002:40%)
    double amount_liters = 0.0; // 총 양 (리터)
    double temperature = 0.0;   // 온도
    int humidity = 0;           // 습도
    int duration_hours = 0;     // 발효 시간

public:
    FermentationBatch() = default;

    // Getter
    string getBatchId() const { return batch_id; }
    string getStartDate() const { return start_date; }
    string getEndDate() const { return end_date; }
    string getExpiryDate() const { return expiry_date; }
    string getParticleSize() const { return particle_size; }
    string getYeastType() const { return yeast_type; }
    string getIngredientInfo() const { return ingredient_info; }
    double getAmountLiters() const { return amount_liters; }
    double getTemperature() const { return temperature; }
    int getHumidity() const { return humidity; }
    int getDurationHours() const { return duration_hours; }

    // Setter
    void setBatchId(const string& val) { batch_id = val; }
    void setStartDate(const string& val) { start_date = val; }
    void setEndDate(const string& val) { end_date = val; }
    void setExpiryDate(const string& val) { expiry_date = val; }
    void setParticleSize(const string& val) { particle_size = val; }
    void setYeastType(const string& val) { yeast_type = val; }
    void setIngredientInfo(const string& val) { ingredient_info = val; }
    void setAmountLiters(double val) { amount_liters = val; }
    void setTemperature(double val) { temperature = val; }
    void setHumidity(int val) { humidity = val; }
    void setDurationHours(int val) { duration_hours = val; }

    // CSV 저장용
    string toCSV() const;
};


// 발효 배치 전체를 관리하는 BatchManager 클래스
class BatchManager {
private:
    vector<FermentationBatch> batches;

public:
    string getSummary();
    vector<FermentationBatch> getDummyBatches();
    void displayBatches(const vector<FermentationBatch>& batches);

    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    double predictFermentation(const FermentationBatch& batch);
    void predictBatchFermentation();

    void exportFermentationStatusToCSV();

    int countFermentingBatches();
    double calculateAvgFermentationHours();
    set<string> getStorageLocations();

    void showBatchPage();
};
