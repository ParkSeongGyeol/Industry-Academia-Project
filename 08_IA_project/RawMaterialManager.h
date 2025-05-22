#pragma once
#include <string>
#include <vector>

using namespace std;

// 원재료 정보를 담는 구조체
struct RawMaterial {
    string material_id;           // 고유 식별자
    string name;                  // 예: 보리, 호밀, 물 등
    string type;                  // 곡물, 기타 등
    string origin;                // 원산지, 예: 스코틀랜드, 미국
    double weight_kg = 0.0;       // 현재 보유 중인 수량 (kg)
    string storage_location;      // 창고 A, 탱크 B 등
    string storage_method;        // 저온 건조, 청결 밀봉 등
    string expiry_date;           // 유효 사용 기한
    string entry_date;            // 공장 입고일
    string exit_date;             // 배치로 투입된 날짜
    string status;                // 정상, 입박, 폐기, 파손 등
    string unit;                  // kg 단위
    double unit_price = 0.0;      // kg 당 단가
    string entry_manager;         // 입고 등록한 사람
    string exit_manager;          // 출고 처리 담당자
    string quality_check;         // 입고 시 품질 확인 여부
    string quality_check_date;    // 품질 검사 일자
};

// 원재료 관리 기능을 담당하는 클래스
class RawMaterialManager {
public:
    string getSummary();   // 대시보드용 요약 문자열 반환
    vector<string> getPageInfoLines(); // 정보 요약 라인 구성

    void showInventory();       // 현재 보유 원재료만 출력
    void showAllMaterials();    // 전체 입출고 이력 출력
    void showRawMaterialPage(); // 콘솔 메뉴 루프 실행
    void addMaterial();         // 원재료 추가
    void updateMaterial();      // 원재료 정보 수정
    void deleteMaterial();      // 출고 처리
    void searchMaterial();      // 이름으로 원재료 검색
    void showStorageEnvironment(); // 보관 장소 환경 정보 출력
	void exportUsedInventoryToCSV(); // 사용된 원재료 CSV로 저장
	void exportRemainingStockToCSV(); // 출고되지 않은 재고 CSV로 저장
	void showUninspectedMaterials(); // 품질 검사 미완료 재료 목록 보기
	void showMaterialsByManager(); // 담당자별 입출고 이력 보기
    double getStock(const string& name); // 현재 재고 조회
    void consumeMaterial(const string& name, double amount); // 재고 차감
    bool processFermentationBatch(double totalBatchKg); // 발효 배치용 원재료 처리
    bool exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList); // 사용 재료 저장

private:
    vector<RawMaterial> materials; // 원재료 리스트 (메모리 상 저장)
    void initializeDummyData();        // 초기 더미 데이터 삽입
};
