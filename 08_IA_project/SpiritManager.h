#pragma once
#include <string>
#include <vector>

using namespace std;

// ĸ��ȭ�� Spirit Ŭ����
class Spirit {
private:
    string id;                    // ���Ǹ� ID (���� �ĺ���)
    string storage_location;     // ���� ���
    string transfer_history;     // �̵� ���
    string production_date;      // ���� ��¥ (���Ǹ� ���� �Ϸ���)
    double alcohol_percentage = 0.0; // ���� (%)
    double yield_liters = 0.0;    // ��ü ������ (L)
    string raw_material_ratio;   // ����� �Է� ���� (��ġ ���)
    int fermentation_days = 0;   // ��ȿ �Ⱓ (�ϼ�)
    string first_cut_time;       // �ʷ� ���� ����
    string last_cut_time;        // �ķ� ���� ����
    int distillation_count = 0;  // ���� Ƚ�� (�Ϲ������� 2ȸ)
    string particle_size;        // ���� ũ�� (�м� ���� - ������)
    string batch_id;             // ��� ��ġ ID (����� ��ġ �ĺ���)
    string cut_yield;            // �ʷ�/�ķ�/���� �� (������ ���淮 ���)

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

// SpiritManager Ŭ����
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
