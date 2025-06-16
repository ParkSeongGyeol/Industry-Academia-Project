#pragma once
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// [Spirit] ������(���Ǹ�) ���� Ŭ����
// -----------------------------------------------------------------------------
class Spirit {
private:
    std::string id;                    // ���Ǹ� ID (���� �ĺ���)
    std::string storage_location;      // ���� ���
    std::string transfer_history;      // �̵� ���
    std::string production_date;       // ���� ��¥ (���Ǹ� ���� �Ϸ���)
    double alcohol_percentage = 0.0;   // ���� (%)
    double yield_liters = 0.0;         // ��ü ������ (L)
    std::string raw_material_ratio;    // ����� �Է� ���� (��ġ ���)
    int fermentation_days = 0;         // ��ȿ �Ⱓ (�ϼ�)
    std::string first_cut_time;        // �ʷ� ���� ����
    std::string last_cut_time;         // �ķ� ���� ����
    int distillation_count = 0;        // ���� Ƚ�� (�Ϲ������� 2ȸ)
    std::string particle_size;         // ���� ũ�� (�м� ���� - ������)
    std::string batch_id;              // ��� ��ġ ID (����� ��ġ �ĺ���)
    std::string cut_yield;             // �ʷ�/�ķ�/���� �� (������ ���淮 ���)

public:
    Spirit() = default;

    // Getter
    std::string getId() const;
    std::string getStorageLocation() const;
    std::string getTransferHistory() const;
    std::string getProductionDate() const;
    double getAlcoholPercentage() const;
    double getYieldLiters() const;
    std::string getRawMaterialRatio() const;
    int getFermentationDays() const;
    std::string getFirstCutTime() const;
    std::string getLastCutTime() const;
    int getDistillationCount() const;
    std::string getParticleSize() const;
    std::string getBatchId() const;
    std::string getCutYield() const;

    // Setter
    void setId(const std::string& val);
    void setStorageLocation(const std::string& val);
    void setTransferHistory(const std::string& val);
    void setProductionDate(const std::string& val);
    void setAlcoholPercentage(double val);
    void setYieldLiters(double val);
    void setRawMaterialRatio(const std::string& val);
    void setFermentationDays(int val);
    void setFirstCutTime(const std::string& val);
    void setLastCutTime(const std::string& val);
    void setDistillationCount(int val);
    void setParticleSize(const std::string& val);
    void setBatchId(const std::string& val);
    void setCutYield(const std::string& val);

};

// -----------------------------------------------------------------------------
// [SpiritManager] ���Ǹ� ��ü ��� �� ��� ���� Ŭ����
// -----------------------------------------------------------------------------
class RecipeManager; // ���� ����

class SpiritManager {
public:
    // [1] ������ �����
    void loadSpiritsFromCSV(const std::string& filename);
    void saveSpiritsToCSV(const std::string& filename);

    // [2] ������ ��� ����
    void produceSpiritByRecipe(RecipeManager& recipeMgr);

    // [3] ���� ���/��ȸ/���
    std::string getSummary();
    std::vector<std::string> getPageInfoLines();
    void displaySpirits();

    // [4] CSV ��������
    void exportSpiritsToCSV(const std::string& filename);

    // [5] �Է�/����/����/�˻�
    void addSpirit();
    void deleteSpirit();
    void updateSpirit();

    // [6] ���� �޴�
    void showSpiritPage();

private:
    std::vector<Spirit> spirits; // ���Ǹ� ����Ʈ (�޸� �� ����)
};
