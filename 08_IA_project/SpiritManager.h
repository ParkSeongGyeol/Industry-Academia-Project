#pragma once
#include <string>
#include <vector>

// 스피릿 정보를 저장하는 구조체
struct Spirit {
    std::string id{};                   // 고유 ID
    std::string storage_location{};     // 보관 장소
    std::string transfer_history{};     // 이동 기록
    std::string production_date{};      // 생산일
    double alcohol_percentage = 0.0;    // 알코올 도수 (%)
    double yield_liters = 0.0;          // 생산량 (리터)
    std::string raw_material_ratio{};   // 원재료 구성 비율
    int fermentation_days = 0;          // 발효 일수
    std::string first_cut_time{};       // 초류 끊은 시점
    std::string last_cut_time{};        // 후류 종료 시점
    int distillation_count = 0;         // 증류 횟수
};

// 스피릿 관리 기능을 제공하는 클래스
class SpiritManager {
private:
    std::vector<Spirit> spirits; // 전체 스피릿 목록 저장

public:
    std::string getSummary();                  // 대시보드에 표시할 요약 정보
    std::vector<std::string> getPageInfoLines(); // 메뉴 상단 요약 라인

    void showSpiritPage(); // 콘솔 메뉴 루프 실행
    void addSpirit();      // 스피릿 추가
    void deleteSpirit();   // 스피릿 삭제
    void displaySpirits(); // 전체 스피릿 목록 출력
    void updateSpirit();   // 스피릿 정보 수정
};
