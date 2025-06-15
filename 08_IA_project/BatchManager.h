#pragma once
#include <string>
#include <vector>
#include <set>

using namespace std;

// -----------------------------------------------------------------------------
// [FermentationBatch] ��ȿ ��ġ ���� Ŭ����
// -----------------------------------------------------------------------------
class FermentationBatch {
private:
    string batch_id;                // ��ġ ID
    string yeast_type;              // ȿ�� ����
    string ingredient_info;         // ����� ���� (��: RM101:60%)
    string start_date;              // ��ȿ ������
    int duration_hours = 0;         // ��ȿ �ð�(�ð�)
    double temperature = 0.0;       // �µ�(��C)
    double amount_liters = 0.0;     // �� ��(����)
    string end_date;                // ��ȿ �Ϸ���
    string expiry_date;             // �������
    string particle_size;           // ���� ũ��(��/��/��)
    string batch_finish_date;       // ��ġ ������(�ߺ� ����)

public:
    FermentationBatch() = default;

    // Getter/Setter
    string getBatchId() const;
    void setBatchId(const string& val);
    string getYeastType() const;
    void setYeastType(const string& val);
    string getIngredientInfo() const;
    void setIngredientInfo(const string& val);
    string getStartDate() const;
    void setStartDate(const string& val);
    int getDurationHours() const;
    void setDurationHours(int val);
    double getTemperature() const;
    void setTemperature(double val);
    double getAmountLiters() const;
    void setAmountLiters(double val);
    string getEndDate() const;
    void setEndDate(const string& val);
    string getExpiryDate() const;
    void setExpiryDate(const string& val);
    string getParticleSize() const;
    void setParticleSize(const string& val);
    string getBatchFinishDate() const;
    void setBatchFinishDate(const string& val);

    // CSV ����ȭ/������ȭ
    string toCSV() const;
    static FermentationBatch fromCSV(const string& line);
};

// -----------------------------------------------------------------------------
// [BatchManager] ��ȿ ��ġ ��ü ��� �� ��� ���� Ŭ����
// -----------------------------------------------------------------------------
class BatchManager {
public:
    // [1] ������ �����
    void loadBatchesFromCSV(const string& filename);
    void saveBatchesToCSV(const string& filename);

    // [2] ���� ����/������ ����
    void produceBatchByRecipe(class RecipeManager& recipeMgr, class RawMaterialManager& rawMgr);

    // [3] ���� ���/��ȸ/���
    string getSummary();
    vector<string> getPageInfoLines();
    void showBatchList();
    void showBatchDetail();
    void showFermentingBatches();

    // [4] CSV ��������
    void exportBatchesToCSV();

    // [5] �Է�/����/����/�˻�
    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    // [6] ��ȿ ����
    void predictBatchFermentation();

    // [7] ���� �޴�
    void showBatchPage(class RecipeManager& recipeMgr, class RawMaterialManager& rawMgr);

private:
    vector<FermentationBatch> batches;
};
