#pragma once
#include <string>
#include <vector>

struct Spirit {
    std::string id;
    std::string storage_location;
    std::string transfer_history;
    std::string production_date;
    double alcohol_percentage;
    double yield_liters;
    std::string raw_material_ratio;
    int fermentation_days;
    std::string first_cut_time;
    std::string last_cut_time;
    int distillation_count;
};

class SpiritManager {
private:
    std::vector<Spirit> spirits;

public:
    std::string getSummary(); // 요약 정보 반환
    void run();
    void addSpirit();
    void deleteSpirit();
    void displaySpirits();
    void updateSpirit();  
};
