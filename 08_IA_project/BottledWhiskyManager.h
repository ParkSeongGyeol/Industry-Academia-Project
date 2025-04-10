#pragma once
#include <string>
#include <vector>

using namespace std;

// 병입된 위스키 정보를 저장하는 클래스
class BottledWhisky {
private:
    string productName; // 제품명

    int bottleCount; // 수량

    double totalVolume; // 총 용량
    double pricePerBottle; // 병당 가격

public:
    BottledWhisky(
        string name,
        int count,
        double volume, double price);

    void ShowInfo() const;

    // getter
    string getName() const;

    int getBottleCount() const;

    double getTotalVolume() const;
    double getPricePerBottle() const;

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
    void BottledListRun();     // 시작
    void showInventory();      // 현재 재고 출력
    void addWhisky();          // 병입 위스키 추가
    void shipWhisky();         // 출고 기록 생성 및 재고 감소
    void showShipmentLog();    // 출고 기록 출력
};
