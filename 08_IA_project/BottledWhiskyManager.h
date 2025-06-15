#pragma once
#include <string>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// [BottledWhisky] 병입 위스키 정보 클래스
// -----------------------------------------------------------------------------
class BottledWhisky {
private:
    // 병입 위스키 속성(캡슐화)
    string productId;         // 제품 ID
    string productName;       // 제품명
    string labelName;         // 라벨명
    string batchNumber;       // 배치 번호
    string exportTarget;      // 출고 대상
    string oakId;             // 오크통 ID
    string shipmentDate;      // 출고 일자
    string serialNumber;      // 제조 번호
    string bottlingManager;   // 병입 담당자
    int bottleCount = 0;      // 수량(병)
    double totalVolume = 0.0; // 총 용량(ml)
    double pricePerBottle = 0.0; // 병당 가격
    bool labeled = false;     // 라벨 부착 여부

public:
    BottledWhisky() = default;

    // 정보 출력
    void ShowInfo() const;

    // Getter
    string getProductId() const;
    string getName() const;
    string getLabelName() const;
    string getBatchNumber() const;
    string getExportTarget() const;
    string getOakId() const;
    string getShipmentDate() const;
    string getSerialNumber() const;
    string getBottlingManager() const;
    int getBottleCount() const;
    double getTotalVolume() const;
    double getPricePerBottle() const;
    bool isLabeled() const;

    // Setter
    void setProductId(const string& id);
    void setName(const string& name);
    void setLabelName(const string& name);
    void setBatchNumber(const string& num);
    void setExportTarget(const string& target);
    void setOakId(const string& id);
    void setShipmentDate(const string& date);
    void setSerialNumber(const string& serial);
    void setBottlingManager(const string& manager);
    void setBottleCount(int count);
    void setTotalVolume(double volume);
    void setPricePerBottle(double price);
    void setLabeled(bool value);

    // 재고 차감
    void decreaseStock(int count);
};

// -----------------------------------------------------------------------------
// [ShipmentRecord] 출고 기록 클래스
// -----------------------------------------------------------------------------
class ShipmentRecord {
private:
    string productName; // 제품명
    string date;        // 출고 날짜
    int quantity = 0;   // 출고 수량
    double totalPrice = 0.0; // 총 가격

public:
    ShipmentRecord() = default;

    void ShowInfo() const;

    // Getter
    string getProductName() const;
    string getDate() const;
    int getQuantity() const;
    double getTotalPrice() const;

    // Setter
    void setProductName(const string& name);
    void setDate(const string& d);
    void setQuantity(int q);
    void setTotalPrice(double price);
};

// -----------------------------------------------------------------------------
// [BottledWhiskyManager] 병입 위스키 전체 목록 및 기능 관리 클래스
// -----------------------------------------------------------------------------
class BottledWhiskyManager {
public:
    // [1] 데이터 입출력
    void loadInventoryFromCSV(const string& filename); // CSV에서 병입 위스키 목록 로드
    void saveInventoryToCSV(const string& filename);   // 전체 inventory를 CSV로 저장

    // [2] 레시피 기반 병입 생산
    void produceBottledByRecipe(class RecipeManager& recipeMgr);

    // [3] 정보 요약/조회/출력
    string getSummary();   // 대시보드용 요약 문자열 반환
    vector<string> getPageInfoLines(); // 정보 요약 라인 구성
    void showInventory();      // 현재 재고 출력
    void showShipmentLog();    // 출고 기록 출력

    // [4] CSV 내보내기
    void exportInventoryToCSV(const string& filename); // 재고 CSV로 저장

    // [5] 입력/수정/삭제/검색
    void addWhisky();          // 병입 위스키 추가
    void updateWhisky();       // 제품 정보 수정
    void deleteWhisky();       // 제품 삭제
    void shipWhisky();         // 출고 기록 생성 및 재고 감소

    // [6] 메인 메뉴
    void showBottledWhiskyPage(); // 콘솔 메뉴 루프 실행

    // [7] ESP32 연동
    void receiveWhiskyFromESP32(); // TCP로 수신

private:
    vector<BottledWhisky> inventory;      // 병입 위스키 재고 목록
    vector<ShipmentRecord> shipmentLog;   // 출고 내역
};
