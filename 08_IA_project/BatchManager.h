#pragma once
#include <string>
#include <vector>

// 발효 배치 정보 구조체
struct FermentationBatch {
    std::string batch_id{};
    std::string start_date{};
    std::string particle_size{};
    std::string yeast_type{};
    std::string ingredients{};
    double amount_liters = 0.0;
    double temperature = 0.0;
    int humidity = 0;
    int duration_hours = 0;

    std::string toCSV() const; // CSV 문자열 변환 함수
};

// 발효 배치 관리 클래스
class BatchManager {
private:
    std::vector<FermentationBatch> batches; // 현재 등록된 배치 목록

public:
    // 요약 정보 반환 (예: "배치 수: 2개")
    std::string getSummary();

    // 테스트용 더미 배치 생성
    std::vector<FermentationBatch> getDummyBatches();

    // 배치 목록 출력
    void displayBatches(const std::vector<FermentationBatch>& batches);

    // 전체 메뉴 실행
    void showBatchPage();

    // 배치 추가/수정/삭제/조회 기능
    void addBatch();
    void updateBatch();
    void deleteBatch();
    void searchBatch();

    // 발효 정도 예측 관련 기능
    double predictFermentation(const FermentationBatch& batch);
    void predictBatchFermentation();
};
