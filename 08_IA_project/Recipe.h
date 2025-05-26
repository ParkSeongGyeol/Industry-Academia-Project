#pragma once
#include <string>
#include <vector>
#include <map>

/**
 * Recipe Ŭ����
 * - ����Ű ���� ��ü ����(�������ġ�潺�Ǹ�������溴��) �帧�� �ϳ��� ��ü�� ����
 * - �� ������ ����/���� ��Ģ, ����, ������, ��ȿ�� �˻�, CSV ����ȭ �� ����
 */
class Recipe {
public:
    // ������ ���� ID �� �̸�
    std::string recipeId;
    std::string name;

    // --- ����� ���� ---
    // <������, ����(%)>
    std::map<std::string, double> rawMaterialRatio;
    std::vector<std::string> usedRawMaterialIds;

    // --- ��ġ(��ȿ) ���� ---
    double batchSizeKg = 0.0;           // ��ġ ���귮(kg)
    std::string yeastType;               // ��� ȿ��
    double fermentationTemp = 0.0;       // ��ȿ �µ�(��)
    int fermentationHours = 0;           // ��ȿ �ð�(�ð�)
    std::string batchId;

    // --- ���Ǹ�(����) ���� ---
    double distillationABV = 0.0;        // ���� ����(%)
    int distillationCount = 2;           // ���� Ƚ��
    // <���Ǹ� �̸�, ���� ����(%)>
    std::vector<std::pair<std::string, double>> spiritSplits;
    std::vector<std::string> spiritIds;

    // --- ����(��ũ) ���� ---
    std::string oakType;                 // ��ũ�� ����
    int agingMonths = 0;                 // ���� �Ⱓ(��)
    std::vector<std::string> oakAgingIds;

    // --- ���� ���� ---
    std::string bottledName;             // ���� ��ǰ��
    int bottleCount = 0;                 // �� ����
    double bottleVolume = 0.0;           // ���� �뷮(ml)
    double bottlePrice = 0.0;            // ���� ����
    std::vector<std::string> bottledIds;

    // ������
    Recipe();

    // --- ����� ���� ---
    // ��ġ ���귮�� ���� �ʿ��� ����Ằ ����(kg) ���
    std::map<std::string, double> getRequiredRawMaterials(double batchSize) const;
    // ����� ��� ������� ����
    bool validateRawMaterialStock(const class RawMaterialManager& mgr, double batchSize) const;

    // --- ��ġ ���� ---
    // ��ġ ������ �ʿ��� ���� ��ȯ(��: ȿ��, �µ�, �ð� ��)
    std::map<std::string, std::string> getBatchParameters() const;

    // --- ���Ǹ� ����/���� ---
    // ���Ǹ� ���� ���(���Ǹ���, ���ҷ�) ��ȯ
    std::vector<std::pair<std::string, double>> getSpiritSplits(double totalAmount) const;

    // --- ����(��ũ) ���� ---
    // ������ �ʿ��� ���� ��ȯ(��ũ ����, �Ⱓ ��)
    std::map<std::string, std::string> getAgingParameters() const;

    // --- ���� ���� ---
    // ���Կ� �ʿ��� ���� ��ȯ(��ǰ��, ����, �뷮, ���� ��)
    std::map<std::string, std::string> getBottlingParameters() const;

    // --- ��ȿ�� �˻� ---
    // ������ ���� ��ü�� ��ȿ�� �˻� (��: ���� �� 100%)
    bool validateRecipe() const;

    // --- ���� �̷�/ID ��� ---
    void addUsedRawMaterialId(const std::string& id);
    void setBatchId(const std::string& id);
    void addSpiritId(const std::string& id);
    void addOakAgingId(const std::string& id);
    void addBottledId(const std::string& id);

    // --- CSV ����ȭ/������ȭ ---
    std::string toCSV() const;
    static Recipe fromCSV(const std::string& line);
};