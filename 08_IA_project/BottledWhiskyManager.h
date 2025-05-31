#pragma once
#include <string>
#include <vector>

using namespace std;

// 병입된 위스키 정보를 저장하는 클래스
class BottledWhisky {
private:
    string productId, productName, labelName, batchNumber, exportTarget;
    string oakId, shipmentDate, serialNumber, bottlingManager;
    int bottleCount = 0;
    double totalVolume = 0.0, pricePerBottle = 0.0;
    bool labeled = false;

public:
    // 생성자
    BottledWhisky() = default;

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
    void setProductId(string id);
    void setName(string name);
    void setLabelName(string name);
    void setBatchNumber(string num);
    void setExportTarget(string target);
    void setOakId(string id);
    void setShipmentDate(string date);
    void setSerialNumber(string serial);
    void setBottlingManager(string manager);
    void setBottleCount(int count);
    void setTotalVolume(double volume);
    void setPricePerBottle(double price);
    void setLabeled(bool value);

    void decreaseStock(int count);
};

// 출고 기록 클래스
class ShipmentRecord {
private:
    string productName; // 제품명
    string date; // 출고 날짜
    int quantity = 0; // 출고 수량
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
    void setProductName(string name);
    void setDate(string d);
    void setQuantity(int q);
    void setTotalPrice(double price);
};

// 전체 병입 위스키 재고 및 출고 기록을 관리하는 클래스
class BottledWhiskyManager {
private:
    vector<BottledWhisky> inventory; // 재고 목록
    vector<ShipmentRecord> shipmentLog; // 출고 내역

public:
    string getSummary(); // 요약 정보 반환
    void showBottledWhiskyPage();     // 시작
    void showInventory();      // 현재 재고 출력
    void addWhisky();          // 병입 위스키 추가
    void shipWhisky();         // 출고 기록 생성 및 재고 감소
    void showShipmentLog();    // 출고 기록 출력
    void updateWhisky();       // 제품 수정
    void deleteWhisky();       // 제품 삭제
	void exportInventoryToCSV(const string& filename); // 재고 CSV로 저장
};
