#pragma once
#include <string>
#include <vector>
#include <map>

class Recipe {
public:
    // --- 레시피 기본 정보 ---
    std::string recipeId;
    std::string name;

    // --- 원재료 정보 ---
    std::map<std::string, double> rawMaterialRatio; // <원재료명, 비율(%)>
    std::map<std::string, double> rawMaterialUsed;  // <원재료명, 실제 투입량(kg/L)>

    // --- 배치(발효) 정보 ---
    std::string yeastType;
    double fermentationTemp = 0.0;
    int fermentationHours = 0;
    std::string batchId;
    double batchOutput = 0.0; // 발효 후 실제 생산량(kg/L)

    // --- 증류 정보 ---
    int distillationCount = 2;
    double distillationABV = 0.0;
    std::string spiritId;
    double spiritOutput = 0.0; // 증류 후 총량(L)
    std::map<std::string, double> spiritFractions; // Head/Heart/Tail 등

    // --- 숙성 정보 ---
    std::string oakType;
    int agingMonths = 0;
    std::string agingId;
    double evaporationLoss = 0.0;
    double agedOutput = 0.0; // 숙성 후 양(L)

    // --- 병입 정보 ---
    std::string bottledName;
    int bottleCount = 0;
    double bottleVolume = 0.0;
    double bottlePrice = 0.0;
    std::string bottledId;
    double totalBottledVolume = 0.0;

    // --- 생성자 및 주요 메서드 ---
    Recipe();

    // 원재료 투입량 계산 및 기록
    void calculateRawMaterialUsage(double batchSize);

    // 원재료 재고 검증
    bool validateRawMaterialStock(const class RawMaterialManager& mgr, double batchSize) const;

    // 배치 생산 (원재료 차감, 배치ID/생산량 기록)
    std::string produceBatch(class RawMaterialManager& mgr, double batchSize);

    // 증류 (분획별 양 계산, 증류ID/총량 기록)
    std::string distillBatch(double yieldRate, double headPct, double tailPct);

    // 숙성 (증발량/최종양 계산, 숙성ID 기록)
    std::string ageSpirit(double evaporationRate);

    // 병입 (병입ID/최종병입량 계산)
    std::string bottleProduct();

    // CSV 직렬화/역직렬화
    std::string toCSV() const;
    static Recipe fromCSV(const std::string& line);

    // 공정별 상세 로그/요약 반환
    std::vector<std::string> getProcessLog() const;
};