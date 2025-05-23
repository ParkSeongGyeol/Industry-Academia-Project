#pragma once
#include <string>
#include <vector>
#include <set>
using namespace std;

// FermentationBatch 클래스 (기존 struct → 클래스 캡슐화)
class FermentationBatch {
private:
    string batch_id;
    string start_date;
    string particle_size;
    string yeast_type;
    string ingredients;
    double amount_liters = 0.0;
    double temperature = 0.0;
    int humidity = 0;
    int duration_hours = 0;

public:
    // 기본 생성자
    FermentationBatch() = default;

    // Getter
    string getBatchId() const { return batch_id; }
    string getStartDate() const { return start_date; }
    string getParticleSize() const { return particle_size; }
    string getYeastType() const { return yeast_type; }
    string getIngredients() const { return ingredients; }
    double getAmountLiters() const { return amount_liters; }
    double getTemperature() const { return temperature; }
    int getHumidity() const { return humidity; }
    int getDurationHours() const { return duration_hours; }

    // Setter
    void setBatchId(const string& val) { batch_id = val; }
    void setStartDate(const string& val) { start_date = val; }
    void setParticleSize(const string& val) { particle_size = val; }
    void setYeastType(const string& val) { yeast_type = val; }
    void setIngredients(const string& val) { ingredients = val; }
    void setAmountLiters(double val) { amount_liters = val; }
    void setTemperature(double val) { temperature = val; }
    void setHumidity(int val) { humidity = val; }
    void setDurationHours(int val) { duration_hours = val; }

    // CSV 출력
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
