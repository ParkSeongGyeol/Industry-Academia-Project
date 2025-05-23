#pragma once
#include <string>
#include <vector>

using namespace std;

// 캡슐화된 Spirit 클래스
class Spirit {
private:
    string id;                    // 스피릿 ID (고유 식별자)
    string storage_location;     // 보관 장소
    string transfer_history;     // 이동 기록
    string production_date;      // 생산 날짜 (스피릿 생산 완료일)
    double alcohol_percentage = 0.0; // 도수 (%)
    double yield_liters = 0.0;    // 전체 증류량 (L)
    string raw_material_ratio;   // 원재료 함량 비율 (배치 기반)
    int fermentation_days = 0;   // 발효 기간 (일수)
    string first_cut_time;       // 초류 끊은 시점
    string last_cut_time;        // 후류 종료 시점
    int distillation_count = 0;  // 증류 횟수 (일반적으로 2회)
    string particle_size;        // 입자 크기 (분쇄 정도 - 선택적)
    string batch_id;             // 재료 배치 ID (연결된 배치 식별자)
    string cut_yield;            // 초류/후류/본류 양 (구간별 수득량 요약)

public:
    Spirit() = default;

    // Getter
    string getId() const { return id; }
    string getStorageLocation() const { return storage_location; }
    string getTransferHistory() const { return transfer_history; }
    string getProductionDate() const { return production_date; }
    double getAlcoholPercentage() const { return alcohol_percentage; }
    double getYieldLiters() const { return yield_liters; }
    string getRawMaterialRatio() const { return raw_material_ratio; }
    int getFermentationDays() const { return fermentation_days; }
    string getFirstCutTime() const { return first_cut_time; }
    string getLastCutTime() const { return last_cut_time; }
    int getDistillationCount() const { return distillation_count; }

    string getParticleSize() const { return particle_size; }
    string getBatchId() const { return batch_id; }
    string getCutYield() const { return cut_yield; }

    // Setter
    void setId(const string& val) { id = val; }
    void setStorageLocation(const string& val) { storage_location = val; }
    void setTransferHistory(const string& val) { transfer_history = val; }
    void setProductionDate(const string& val) { production_date = val; }
    void setAlcoholPercentage(double val) { alcohol_percentage = val; }
    void setYieldLiters(double val) { yield_liters = val; }
    void setRawMaterialRatio(const string& val) { raw_material_ratio = val; }
    void setFermentationDays(int val) { fermentation_days = val; }
    void setFirstCutTime(const string& val) { first_cut_time = val; }
    void setLastCutTime(const string& val) { last_cut_time = val; }
    void setDistillationCount(int val) { distillation_count = val; }

    void setParticleSize(const string& val) { particle_size = val; }
    void setBatchId(const string& val) { batch_id = val; }
    void setCutYield(const string& val) { cut_yield = val; }
};

// SpiritManager 클래스
class SpiritManager {
private:
    vector<Spirit> spirits;

public:
    string getSummary();
    vector<string> getPageInfoLines();

    void showSpiritPage();
    void addSpirit();
    void deleteSpirit();
    void displaySpirits();
    void updateSpirit();
    void exportSpiritsToCSV(const string& filename);

};
