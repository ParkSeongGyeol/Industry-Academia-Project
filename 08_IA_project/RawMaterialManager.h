#pragma once
#include <string>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// [RawMaterial] 원재료 정보 클래스
// -----------------------------------------------------------------------------
class RawMaterial {
private:
    // 원재료의 속성(캡슐화)
    string material_id;         // 원재료 고유 ID
    string name;                // 원재료 이름
    string type;                // 원재료 종류
    string origin;              // 원산지
    double weight_kg = 0.0;     // 무게(kg)
    string storage_location;    // 보관 위치
    string storage_method;      // 보관 방법
    string expiry_date;         // 유통기한
    string entry_date;          // 입고일
    string exit_date;           // 출고일
    string status;              // 상태(정상/출고 등)
    string unit;                // 단위(kg, L 등)
    double unit_price = 0.0;    // 단가
    string entry_manager;       // 입고 담당자
    string exit_manager;        // 출고 담당자
    string quality_check;       // 품질 검사 결과
    string quality_check_date;  // 품질 검사 일자

public:
    RawMaterial() = default;

    // Getter/Setter (각 속성별)
    string getMaterialId() const;
    string getName() const;
    string getType() const;
    string getOrigin() const;
    double getWeightKg() const;
    string getStorageLocation() const;
    string getStorageMethod() const;
    string getExpiryDate() const;
    string getEntryDate() const;
    string getExitDate() const;
    string getStatus() const;
    string getUnit() const;
    double getUnitPrice() const;
    string getEntryManager() const;
    string getExitManager() const;
    string getQualityCheck() const;
    string getQualityCheckDate() const;

    void setMaterialId(const string& val);
    void setName(const string& val);
    void setType(const string& val);
    void setOrigin(const string& val);
    void setWeightKg(double val);
    void setStorageLocation(const string& val);
    void setStorageMethod(const string& val);
    void setExpiryDate(const string& val);
    void setEntryDate(const string& val);
    void setExitDate(const string& val);
    void setStatus(const string& val);
    void setUnit(const string& val);
    void setUnitPrice(double val);
    void setEntryManager(const string& val);
    void setExitManager(const string& val);
    void setQualityCheck(const string& val);
    void setQualityCheckDate(const string& val);
};

// -----------------------------------------------------------------------------
// [RecipeManager] 레시피 관리 클래스 (전방 선언)
// -----------------------------------------------------------------------------
class RecipeManager; // 전방 선언 필요

// -----------------------------------------------------------------------------
// [RawMaterialManager] 원재료 전체 목록 및 기능 관리 클래스
// -----------------------------------------------------------------------------
class RawMaterialManager {
public:
    // [1] 데이터 입출력
    void loadMaterialsFromCSV(const string& filename); // CSV에서 원재료 목록 로드
    void saveMaterialsToCSV(const string& filename);   // 전체 materials를 CSV로 저장
    bool exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList); // 사용 재료 저장

    // [2] 내부 연산
    double getStock(const string& materialId) const; // 현재 재고 조회
    void consumeMaterial(const string& name, double amount); // 재고 차감
    void produceBatchByRecipe(RecipeManager& recipeMgr); // 레시피 기반 배치 생산

    // [3] 정보 요약/조회/출력
    string getSummary();   // 대시보드용 요약 문자열 반환
    vector<string> getPageInfoLines(); // 정보 요약 라인 구성
    void showInventory();       // 현재 보유 원재료만 출력
    void showAllMaterials();    // 전체 입출고 이력 출력
    void showUninspectedMaterials(); // 품질 검사 미완료 재료 목록 보기
    void showMaterialsByManager(); // 담당자별 입출고 이력 보기
    void showStorageEnvironment(); // 보관 장소 환경 정보 출력

    // [4] CSV 내보내기
    void exportUsedInventoryToCSV(); // 사용된 원재료 CSV로 저장
    void exportRemainingStockToCSV(); // 출고되지 않은 재고 CSV로 저장

    // [5] 입력/수정/삭제/검색
    void addMaterial();         // 원재료 추가
    void updateMaterial();      // 원재료 정보 수정
    void deleteMaterial();      // 출고 처리
    void searchMaterial();      // 이름으로 원재료 검색

    // [6] 메인 메뉴
    void showRawMaterialPage(); // 콘솔 메뉴 루프 실행

private:
    vector<RawMaterial> materials; // 원재료 리스트 (메모리 상 저장)
};
