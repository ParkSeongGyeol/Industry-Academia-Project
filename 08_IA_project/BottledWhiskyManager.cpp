#include "BottledWhiskyManager.h"
#include <iostream>

using namespace std;

// === BottledWhisky 클래스 구현 ===
BottledWhisky::BottledWhisky(string name, string label, string batch, string target, int count, double volume, double price, bool labeled)
    : productName(name), labelName(label), batchNumber(batch), exportTarget(target), bottleCount(count), totalVolume(volume), pricePerBottle(price), labeled(labeled) {}

void BottledWhisky::ShowInfo() const {
    cout << "제품명: " << productName << endl;
    cout << "수량: " << bottleCount << "병" << endl;
    cout << "총 용량: " << totalVolume << "L" << endl;
    cout << "병당 가격 : " << pricePerBottle << "원" << endl;
    cout << "라벨 부착 여부: " << (labeled ? "O" : "X") << endl;
    cout << "라벨명: " << labelName << endl;
    cout << "배치 번호: " << batchNumber << endl;
    cout << "출고 대상: " << exportTarget << endl;
}

// getter
string BottledWhisky::getName() const { return productName; }
string BottledWhisky::getLabelName() const { return labelName; }
string BottledWhisky::getBatchNumber() const { return batchNumber; }
string BottledWhisky::getExportTarget() const { return exportTarget; }

int BottledWhisky::getBottleCount() const { return bottleCount; }

double BottledWhisky::getTotalVolume() const { return totalVolume; }
double BottledWhisky::getPricePerBottle() const { return pricePerBottle; }

bool BottledWhisky::isLabeled() const { return labeled; }

// setter
void BottledWhisky::setLabelName(string name) { labelName = name; }
void BottledWhisky::setBatchNumber(string num) { batchNumber = num; }
void BottledWhisky::setExportTarget(string target) { exportTarget = target; }
void BottledWhisky::setLabeled(bool value) { labeled = value; }

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
    cout << "[출고기록] 날짜: " << date << endl;
    cout << "제품명: " << productName << endl;
    cout << "출고 수량: " << quantity << "병" << endl;
    cout << "총 가격 : " << totalPrice << "원" << endl;
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
    string name, label, batch, target;
    int count;
    double volume, price;
    int labeledInt;

    cout << "\n[위스키 추가]\n";
    cout << "제품명: "; cin >> name;
    cout << "라벨명: "; cin >> label;
    cout << "배치 번호: "; cin >> batch;
    cout << "출고 대상: "; cin >> target;
    cout << "수량(병): "; cin >> count;
    cout << "총 용량(L): "; cin >> volume;
    cout << "병당 가격: "; cin >> price;
    cout << "라벨 부착 여부 (1: O, 0: X): "; cin >> labeledInt;

    BottledWhisky newWhisky(name, label, batch, target, count, volume, price, labeledInt == 1);
    inventory.push_back(newWhisky);

    cout << "제품이 추가되었습니다.\n";
}

void BottledWhiskyManager::updateWhisky() {
    string name;
    cout << "수정할 제품명 입력: ";
    cin >> name;

    for (auto& w : inventory) {
        if (w.getName() == name) {
            int choice;
            cout << "[1] 라벨명" << endl;
            cout << "[2] 배치번호" << endl;
            cout << "[3] 출고대상" << endl;
            cout << "[4] 라벨 여부\n선택: ";
            cin >> choice;
            switch (choice) {
            case 1: { string label; cout << "새 라벨명: "; cin >> label; w.setLabelName(label); break; }
            case 2: { string batch; cout << "새 배치번호: "; cin >> batch; w.setBatchNumber(batch); break; }
            case 3: { string target; cout << "새 출고 대상: "; cin >> target; w.setExportTarget(target); break; }
            case 4: { int l; cout << "라벨 여부 (1: O, 0: X): "; cin >> l; w.setLabeled(l == 1); break; }
            default: cout << "잘못된 선택입니다.\n";
            }
            return;
        }
    }
    cout << "해당 제품을 찾을 수 없습니다.\n";
}

void BottledWhiskyManager::deleteWhisky() {
    string name;
    cout << "삭제할 제품명 입력: ";
    cin >> name;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getName() == name) {
            inventory.erase(it);
            cout << "삭제되었습니다.\n";
            return;
        }
    }
    cout << "해당 제품을 찾을 수 없습니다.\n";
}

// 출고 기록 생성 및 재고 감소
void BottledWhiskyManager::shipWhisky() {
    string name, date;
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
        cout << "5. 완제품 수정\n";
        cout << "6. 완제품 삭제\n";
        cout << "0. 메인 메뉴로 돌아가기\n";
        cout << "선택: ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n";
        }
    } while (choice != 0);
}
