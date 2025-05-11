#pragma once
#include <string>
#include <vector>

// 원재료 정보를 담는 구조체
struct RawMaterial {
    std::string name{};              // 원재료 이름
    std::string type{};              // 원재료 종류 (예: 곡물, 기타)
    std::string origin{};            // 원산지
    double weight_kg = 0.0;          // 무게 (kg)
    std::string storage_location{};  // 보관 위치
    std::string storage_method{};    // 보관 방법
    std::string expiry_date{};       // 유통기한
    std::string entry_date{};        // 입고일
    std::string exit_date{};         // 출고일 (출고 전이면 비어 있음)
};

// 원재료 관리 기능을 담당하는 클래스
class RawMaterialManager {
public:
    std::string getSummary();   // 대시보드용 요약 문자열 반환
    std::vector<std::string> getPageInfoLines(); // 정보 요약 라인 구성

    void showInventory();       // 현재 보유 원재료만 출력
    void showAllMaterials();    // 전체 입출고 이력 출력
    void showRawMaterialPage(); // 콘솔 메뉴 루프 실행
    void addMaterial();         // 원재료 추가
    void updateMaterial();      // 원재료 정보 수정
    void deleteMaterial();      // 출고 처리
    void searchMaterial();      // 이름으로 원재료 검색
    void showStorageEnvironment(); // 보관 장소 환경 정보 출력

private:
    std::vector<RawMaterial> materials; // 원재료 리스트 (메모리 상 저장)
    void initializeDummyData();        // 초기 더미 데이터 삽입
};
