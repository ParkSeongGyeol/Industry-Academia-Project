#include "BottledWhiskyManager.h"
#include <iostream>

using namespace std;

// === BottledWhisky 클래스 구현 ===
BottledWhisky::BottledWhisky(string name, int count, double volume, double price)
    : productName(name), bottleCount(count), totalVolume(volume), pricePerBottle(price) {
}

void BottledWhisky::ShowInfo() const {
    cout << "제품명: " << productName << ", 수량: " << bottleCount << "병"
        << ", 총 용량: " << totalVolume << "L, 병당 가격: " << pricePerBottle << "원" << endl;
}

// getter
string BottledWhisky::getName() const { return productName; }
int BottledWhisky::getBottleCount() const { return bottleCount; }
double BottledWhisky::getTotalVolume() const { return totalVolume; }
double BottledWhisky::getPricePerBottle() const { return pricePerBottle; }

// 출고 시 병 수와 총 용량을 감소시키는 함수
void BottledWhisky::decreaseStock(int count) {
    if (bottleCount == 0) return;
    double perBottleVolume = totalVolume / bottleCount; // 병당 용량
    bottleCount -= count;
    totalVolume -= perBottleVolume * count;
}


// === ShipmentRecord 클래스 구현 ===
ShipmentRecord::ShipmentRecord(string name, string date, int qty, double price)
    : productName(name), date(date), quantity(qty), totalPrice(price) {
}
// 출고 기록 출력
void ShipmentRecord::ShowInfo() const {
    cout << "[출고기록] 날짜: " << date << ", 제품명: " << productName
        << ", 출고 수량: " << quantity << "병, 총 가격: " << totalPrice << "원" << endl;
}


// === BottledWhiskyManager 클래스 구현 ===
std::string BottledWhiskyManager::getSummary() {
    int totalCount = 0;
    double totalPrice = 0;

    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalPrice += w.getPricePerBottle();
    }

    std::string result = "병입: " + std::to_string(totalCount) + "병";
    if (!inventory.empty()) {
        result += " / 평균가: " + std::to_string(static_cast<int>(totalPrice / inventory.size())) + "원";
    }
    return result;
}


// 전체 재고 출력
void BottledWhiskyManager::showInventory() {
    if (inventory.empty()) {
        cout << "현재 재고가 없습니다.\n";
        return;
    }
    cout << "\n[병입 위스키 재고 목록]\n";
    for (const auto& whisky : inventory) {
        whisky.ShowInfo();
    }
}

// 새 제품 추가
void BottledWhiskyManager::addWhisky() {
    string name;
    int count;
    double volume, price;

    cout << "\n[위스키 추가]\n";
    cout << "제품명: "; cin >> name;
    cout << "수량(병): "; cin >> count;
    cout << "총 용량(L): "; cin >> volume;
    cout << "병당 가격: "; cin >> price;

    BottledWhisky newWhisky(name, count, volume, price);
    inventory.push_back(newWhisky);

    cout << "제품이 추가되었습니다.\n";
}
// 출고 기록 생성 및 재고 감소
void BottledWhiskyManager::shipWhisky() {
    string name, buyer, date;
    int qty;

    cout << "\n[출고 등록]\n";
    cout << "출고할 제품명: "; cin >> name;

    for (auto& whisky : inventory) {
        if (whisky.getName() == name) {
            cout << "출고 수량(병): "; cin >> qty;
            if (qty > whisky.getBottleCount()) {
                cout << "출고 수량이 재고보다 많습니다.\n";
                return;
            }

            cout << "출고 날짜 (YYYY-MM-DD): "; cin >> date;

            double totalPrice = qty * whisky.getPricePerBottle();
            ShipmentRecord record(name, date, qty, totalPrice);
            shipmentLog.push_back(record);

            whisky.decreaseStock(qty);

            cout << "출고 기록이 등록되었습니다.\n";
            return;
        }
    }

    cout << "해당 제품을 찾을 수 없습니다.\n";
}
// 출고 기록 출력
void BottledWhiskyManager::showShipmentLog() {
    if (shipmentLog.empty()) {
        cout << "출고 기록이 없습니다.\n";
        return;
    }

    cout << "\n[출고 기록 목록]\n";
    for (const auto& record : shipmentLog) {
        record.ShowInfo();
    }
}

// 병입 위스키 관리 메뉴 실행
void BottledWhiskyManager::BottledListRun() {
    int choice;

    do {
        cout << "\n[병입 및 완성품 관리 메뉴]\n";
        cout << "1. 재고 목록 보기\n";
        cout << "2. 제품 추가\n";
        cout << "3. 제품 출고\n";
        cout << "4. 출고 기록 보기\n";
        cout << "0. 메인 메뉴로 돌아가기\n";
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n";
        }
    } while (choice != 0);
}
