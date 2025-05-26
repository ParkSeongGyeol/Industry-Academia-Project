#pragma once
#include <string>
#include <vector>
#include <map>

/**
 * Recipe 클래스
 * - 위스키 생산 전체 공정(원재료→배치→스피릿→숙성→병입) 흐름을 하나의 객체로 관리
 * - 각 공정별 투입/산출 규칙, 분할, 추적성, 유효성 검사, CSV 직렬화 등 지원
 */
class Recipe {
public:
    // 레시피 고유 ID 및 이름
    std::string recipeId;
    std::string name;

    // --- 원재료 정보 ---
    // <원재료명, 비율(%)>
    std::map<std::string, double> rawMaterialRatio;
    std::vector<std::string> usedRawMaterialIds;

    // --- 배치(발효) 정보 ---
    double batchSizeKg = 0.0;           // 배치 생산량(kg)
    std::string yeastType;               // 사용 효모
    double fermentationTemp = 0.0;       // 발효 온도(℃)
    int fermentationHours = 0;           // 발효 시간(시간)
    std::string batchId;

    // --- 스피릿(증류) 정보 ---
    double distillationABV = 0.0;        // 증류 도수(%)
    int distillationCount = 2;           // 증류 횟수
    // <스피릿 이름, 분할 비율(%)>
    std::vector<std::pair<std::string, double>> spiritSplits;
    std::vector<std::string> spiritIds;

    // --- 숙성(오크) 정보 ---
    std::string oakType;                 // 오크통 종류
    int agingMonths = 0;                 // 숙성 기간(월)
    std::vector<std::string> oakAgingIds;

    // --- 병입 정보 ---
    std::string bottledName;             // 병입 제품명
    int bottleCount = 0;                 // 병 수량
    double bottleVolume = 0.0;           // 병당 용량(ml)
    double bottlePrice = 0.0;            // 병당 가격
    std::vector<std::string> bottledIds;

    // 생성자
    Recipe();

    // --- 원재료 관련 ---
    // 배치 생산량에 따라 필요한 원재료별 수량(kg) 계산
    std::map<std::string, double> getRequiredRawMaterials(double batchSize) const;
    // 원재료 재고가 충분한지 검증
    bool validateRawMaterialStock(const class RawMaterialManager& mgr, double batchSize) const;

    // --- 배치 생성 ---
    // 배치 생성에 필요한 정보 반환(예: 효모, 온도, 시간 등)
    std::map<std::string, std::string> getBatchParameters() const;

    // --- 스피릿 분할/생성 ---
    // 스피릿 분할 결과(스피릿명, 분할량) 반환
    std::vector<std::pair<std::string, double>> getSpiritSplits(double totalAmount) const;

    // --- 숙성(오크) 생성 ---
    // 숙성에 필요한 정보 반환(오크 종류, 기간 등)
    std::map<std::string, std::string> getAgingParameters() const;

    // --- 병입 생성 ---
    // 병입에 필요한 정보 반환(제품명, 수량, 용량, 가격 등)
    std::map<std::string, std::string> getBottlingParameters() const;

    // --- 유효성 검사 ---
    // 레시피 정의 자체의 유효성 검사 (예: 비율 합 100%)
    bool validateRecipe() const;

    // --- 생산 이력/ID 기록 ---
    void addUsedRawMaterialId(const std::string& id);
    void setBatchId(const std::string& id);
    void addSpiritId(const std::string& id);
    void addOakAgingId(const std::string& id);
    void addBottledId(const std::string& id);

    // --- CSV 직렬화/역직렬화 ---
    std::string toCSV() const;
    static Recipe fromCSV(const std::string& line);
};