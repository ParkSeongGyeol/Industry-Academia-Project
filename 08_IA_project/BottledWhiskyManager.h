#pragma once
#include <string>
#include <vector>

using namespace std;

// 병입된 위스키 정보를 저장하는 클래스
class BottledWhisky {
private:
    string productName; // 제품명
    string labelName; // 라벨명
    string batchNumber; // 배치 번호
    string exportTarget; // 출고 대상

    int bottleCount; // 수량

    double totalVolume; // 총 용량
    double pricePerBottle; // 병당 가격

    bool labeled; // 라벨 부착 여부


public:
    BottledWhisky(
        string name, string label, string batch, string target,

        int count,

        double volume, double price,
        
        bool labeled);

    void ShowInfo() const;

    // getter
    string getName() const;
    string getLabelName() const;
    string getBatchNumber() const;
    string getExportTarget() const;

    int getBottleCount() const;

    double getTotalVolume() const;
    double getPricePerBottle() const;

    bool isLabeled() const;
    
    // setter
    void setLabelName(string name);
    void setBatchNumber(string num);
    void setExportTarget(string target);
    void setName(string name);

    void setBottleCount(int count);

    void setTotalVolume(double volume);
    void setPricePerBottle(double price);

    void setLabeled(bool value);

    // 출고 시 재고 감소
    void decreaseStock(int count);
};

// 출고 기록 클래스
class ShipmentRecord {
private:
    string productName; // 제품명
    string date; // 출고 날짜

    int quantity; // 출고 수량
    
    double totalPrice; // 총 가격

public:
    ShipmentRecord
    (string name, string date,
        int qty,
        double price);

    void ShowInfo() const;
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
};
