#pragma once
#include <string>
#include <vector>

using namespace std;

// Ä¸½¶È­µÈ Spirit Å¬·¡½º
class Spirit {
private:
    string id;
    string storage_location;
    string transfer_history;
    string production_date;
    double alcohol_percentage = 0.0;
    double yield_liters = 0.0;
    string raw_material_ratio;
    int fermentation_days = 0;
    string first_cut_time;
    string last_cut_time;
    int distillation_count = 0;

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
};

// SpiritManager Å¬·¡½º
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
};
