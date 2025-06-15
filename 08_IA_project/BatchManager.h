#pragma once
#include <string>
#include <vector>
#include <set>

using namespace std;

// -----------------------------------------------------------------------------
// [FermentationBatch] 발효 배치 정보 클래스
// -----------------------------------------------------------------------------
class FermentationBatch {
private:
    string batch_id;                // 배치 ID
    string yeast_type;              // 효모 종류
    string ingredient_info;         // 원재료 조성 (예: RM101:60%)
    string start_date;              // 발효 시작일
    int duration_hours = 0;         // 발효 시간(시간)
    double temperature = 0.0;       // 온도(°C)
    double amount_liters = 0.0;     // 총 양(리터)
    string end_date;                // 발효 완료일
    string expiry_date;             // 유통기한
    string particle_size;           // 입자 크기(소/중/대)
    string batch_finish_date;       // 배치 종료일(중복 가능)

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

    // CSV 직렬화/역직렬화
    string toCSV() const;
    static FermentationBatch fromCSV(const string& line);
};

// -----------------------------------------------------------------------------
// [BatchManager] 발효 배치 전체 목록 및 기능 관리 클래스
// -----------------------------------------------------------------------------
class BatchManager {
public:
    // [1] 데이터 입출력
    void loadBatchesFromCSV(const string& filename);
    void saveBatchesToCSV(const string& filename);

    // [2] 내부 연산/레시피 연동
    void produceBatchByRecipe(class RecipeManager& recipeMgr);

    // [3] 정보 요약/조회/출력
    string getSummary();
    vector<string> getPageInfoLines();
    void showBatchList();
    void showBatchDetail();
    void showFermentingBatches();

    // [4] CSV 내보내기
    void exportBatchesToCSV();

    // [5] 입력/수정/삭제/검색
    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    // [6] 발효 예측
    void predictBatchFermentation();

    // [7] 메인 메뉴
    void showBatchPage();

private:
    vector<FermentationBatch> batches;
};
