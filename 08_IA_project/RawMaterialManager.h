#pragma once
#include <string>
#include <vector>

using namespace std;

//  RawMaterial 클래스 정의 (기존 구조체 → 캡슐화된 클래스)
class RawMaterial {
private:
    string material_id;
    string name;
    string type;
    string origin;
    double weight_kg = 0.0;
    string storage_location;
    string storage_method;
    string expiry_date;
    string entry_date;
    string exit_date;
    string status;
    string unit;
    double unit_price = 0.0;
    string entry_manager;
    string exit_manager;
    string quality_check;
    string quality_check_date;

public:
    // 생성자 (필요 시 확장 가능)
    RawMaterial() = default;

    // Getter
    string getMaterialId() const { return material_id; }
    string getName() const { return name; }
    string getType() const { return type; }
    string getOrigin() const { return origin; }
    double getWeightKg() const { return weight_kg; }
    string getStorageLocation() const { return storage_location; }
    string getStorageMethod() const { return storage_method; }
    string getExpiryDate() const { return expiry_date; }
    string getEntryDate() const { return entry_date; }
    string getExitDate() const { return exit_date; }
    string getStatus() const { return status; }
    string getUnit() const { return unit; }
    double getUnitPrice() const { return unit_price; }
    string getEntryManager() const { return entry_manager; }
    string getExitManager() const { return exit_manager; }
    string getQualityCheck() const { return quality_check; }
    string getQualityCheckDate() const { return quality_check_date; }

    // Setter
    void setMaterialId(const string& val) { material_id = val; }
    void setName(const string& val) { name = val; }
    void setType(const string& val) { type = val; }
    void setOrigin(const string& val) { origin = val; }
    void setWeightKg(double val) { weight_kg = val; }
    void setStorageLocation(const string& val) { storage_location = val; }
    void setStorageMethod(const string& val) { storage_method = val; }
    void setExpiryDate(const string& val) { expiry_date = val; }
    void setEntryDate(const string& val) { entry_date = val; }
    void setExitDate(const string& val) { exit_date = val; }
    void setStatus(const string& val) { status = val; }
    void setUnit(const string& val) { unit = val; }
    void setUnitPrice(double val) { unit_price = val; }
    void setEntryManager(const string& val) { entry_manager = val; }
    void setExitManager(const string& val) { exit_manager = val; }
    void setQualityCheck(const string& val) { quality_check = val; }
    void setQualityCheckDate(const string& val) { quality_check_date = val; }
};

//  원재료 전체 목록 및 기능을 관리하는 클래스
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

    double getStock(const string& materialId) const; // 현재 재고 조회
    void consumeMaterial(const string& name, double amount); // 재고 차감
    bool processFermentationBatch(double totalBatchKg); // 발효 배치용 원재료 처리
    bool exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList); // 사용 재료 저장

    void loadMaterialsFromCSV(const string& filename); // CSV에서 원재료 목록 로드

private:
    vector<RawMaterial> materials; // 원재료 리스트 (메모리 상 저장)
};
