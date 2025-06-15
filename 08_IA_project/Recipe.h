#pragma once
#include <string>
#include <vector>
#include <map>

class Recipe {
public:
    // --- ������ �⺻ ���� ---
    std::string recipeId;
    std::string name;

    // --- ����� ���� ---
    std::map<std::string, double> rawMaterialRatio; // <������, ����(%)>
    std::map<std::string, double> rawMaterialUsed;  // <������, ���� ���Է�(kg/L)>

    // --- ��ġ(��ȿ) ���� ---
    std::string yeastType;
    double fermentationTemp = 0.0;
    int fermentationHours = 0;
    std::string batchId;
    double batchOutput = 0.0; // ��ȿ �� ���� ���귮(kg/L)

    // --- ���� ���� ---
    int distillationCount = 2;
    double distillationABV = 0.0;
    std::string spiritId;
    double spiritOutput = 0.0; // ���� �� �ѷ�(L)
    std::map<std::string, double> spiritFractions; // Head/Heart/Tail ��

    // --- ���� ���� ---
    std::string oakType;
    int agingMonths = 0;
    std::string agingId;
    double evaporationLoss = 0.0;
    double agedOutput = 0.0; // ���� �� ��(L)

    // --- ���� ���� ---
    std::string bottledName;
    int bottleCount = 0;
    double bottleVolume = 0.0;
    double bottlePrice = 0.0;
    std::string bottledId;
    double totalBottledVolume = 0.0;

    // --- ������ �� �ֿ� �޼��� ---
    Recipe();

    // ����� ���Է� ��� �� ���
    void calculateRawMaterialUsage(double batchSize);

    // ����� ��� ����
    bool validateRawMaterialStock(const class RawMaterialManager& mgr, double batchSize) const;

    // ��ġ ���� (����� ����, ��ġID/���귮 ���)
    std::string produceBatch(class RawMaterialManager& mgr, double batchSize);

    // ���� (��ȹ�� �� ���, ����ID/�ѷ� ���)
    std::string distillBatch(double yieldRate, double headPct, double tailPct);

    // ���� (���߷�/������ ���, ����ID ���)
    std::string ageSpirit(double evaporationRate);

    // ���� (����ID/�������Է� ���)
    std::string bottleProduct();

    // CSV ����ȭ/������ȭ
    std::string toCSV() const;
    static Recipe fromCSV(const std::string& line);

    // ������ �� �α�/��� ��ȯ
    std::vector<std::string> getProcessLog() const;
};