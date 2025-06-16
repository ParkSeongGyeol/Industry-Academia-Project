#pragma once
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// [Spirit] 증류주(스피릿) 정보 클래스
// -----------------------------------------------------------------------------
class Spirit {
private:
    std::string id;                    // 스피릿 ID (고유 식별자)
    std::string storage_location;      // 보관 장소
    std::string transfer_history;      // 이동 기록
    std::string production_date;       // 생산 날짜 (스피릿 생산 완료일)
    double alcohol_percentage = 0.0;   // 도수 (%)
    double yield_liters = 0.0;         // 전체 증류량 (L)
    std::string raw_material_ratio;    // 원재료 함량 비율 (배치 기반)
    int fermentation_days = 0;         // 발효 기간 (일수)
    std::string first_cut_time;        // 초류 끊은 시점
    std::string last_cut_time;         // 후류 종료 시점
    int distillation_count = 0;        // 증류 횟수 (일반적으로 2회)
    std::string particle_size;         // 입자 크기 (분쇄 정도 - 선택적)
    std::string batch_id;              // 재료 배치 ID (연결된 배치 식별자)
    std::string cut_yield;             // 초류/후류/본류 양 (구간별 수득량 요약)

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
// [SpiritManager] 스피릿 전체 목록 및 기능 관리 클래스
// -----------------------------------------------------------------------------
class RecipeManager; // 전방 선언

class SpiritManager {
public:
    // [1] 데이터 입출력
    void loadSpiritsFromCSV(const std::string& filename);
    void saveSpiritsToCSV(const std::string& filename);

    // [2] 레시피 기반 생산
    void produceSpiritByRecipe(RecipeManager& recipeMgr);

    // [3] 정보 요약/조회/출력
    std::string getSummary();
    std::vector<std::string> getPageInfoLines();
    void displaySpirits();

    // [4] CSV 내보내기
    void exportSpiritsToCSV(const std::string& filename);

    // [5] 입력/수정/삭제/검색
    void addSpirit();
    void deleteSpirit();
    void updateSpirit();

    // [6] 메인 메뉴
    void showSpiritPage();

private:
    std::vector<Spirit> spirits; // 스피릿 리스트 (메모리 상 저장)
};
