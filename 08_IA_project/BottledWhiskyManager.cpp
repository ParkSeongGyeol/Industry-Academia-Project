#include "BottledWhiskyManager.h"
#include "UIUtils.h"
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
void BottledWhisky::setName(string name) { productName = name; }

void BottledWhisky::setBottleCount(int count) { bottleCount = count;}

void BottledWhisky::setTotalVolume(double volume) { totalVolume = volume; }
void BottledWhisky::setPricePerBottle(double price) { pricePerBottle = price; }

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
            while (true) {
                int choice;
                cout << "\n[수정할 항목 선택]\n";
                cout << "[1] 제품명\n";
                cout << "[2] 라벨명\n";
                cout << "[3] 배치번호\n";
                cout << "[4] 출고대상\n";
                cout << "[5] 수량(병)\n";
                cout << "[6] 총 용량(L)\n";
                cout << "[7] 병당 가격\n";
                cout << "[8] 라벨 여부\n";
                cout << "[0] 수정을 완료하고 나가기\n";
                cout << "선택: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    string newName;
                    cout << "새 제품명: ";
                    cin >> newName;
                    w.setName(newName);
                    break;
                }
                case 2: {
                    string label;
                    cout << "새 라벨명: ";
                    cin >> label;
                    w.setLabelName(label);
                    break;
                }
                case 3: {
                    string batch;
                    cout << "새 배치번호: ";
                    cin >> batch;
                    w.setBatchNumber(batch);
                    break;
                }
                case 4: {
                    string target;
                    cout << "새 출고 대상: ";
                    cin >> target;
                    w.setExportTarget(target);
                    break;
                }
                case 5: {
                    int count;
                    cout << "새 수량(병): ";
                    cin >> count;
                    w.setBottleCount(count);
                    break;
                }
                case 6: {
                    double volume;
                    cout << "새 총 용량(L): ";
                    cin >> volume;
                    w.setTotalVolume(volume);
                    break;
                }
                case 7: {
                    double price;
                    cout << "새 병당 가격: ";
                    cin >> price;
                    w.setPricePerBottle(price);
                    break;
                }
                case 8: {
                    int l;
                    cout << "라벨 여부 (1: O, 0: X): ";
                    cin >> l;
                    w.setLabeled(l == 1);
                    break;
                }
                case 0:
                    cout << "수정을 완료했습니다.\n";
                    return;
                default:
                    cout << "잘못된 선택입니다.\n";
                }
            }
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
void BottledWhiskyManager::showBottledWhiskyPage() {
    int choice;

    do {
        system("cls");

        // 좌측 정보 구성 (infoLines)
        vector<string> infoLines;
        int totalCount = 0;
        double totalVolume = 0, totalValue = 0;

        for (const auto& w : inventory) {
            totalCount += w.getBottleCount();
            totalVolume += w.getTotalVolume();
            totalValue += w.getBottleCount() * w.getPricePerBottle();
        }

        infoLines.push_back("▶ 전체 병 수량: " + to_string(totalCount) + "병");
        infoLines.push_back("▶ 전체 용량: " + to_string((int)totalVolume) + "L");
        if (totalCount > 0) {
            infoLines.push_back("▶ 평균 병당 가격: " + to_string((int)(totalValue / totalCount)) + "원");
        }
        else {
            infoLines.push_back("▶ 평균 병당 가격: -");
        }

        // 우측 메뉴 구성 (menuLines)
        vector<string> menu = {
            "[1] 재고 목록 보기",
            "[2] 제품 추가",
            "[3] 제품 출고",
            "[4] 출고 기록 보기",
            "[5] 완제품 수정",
            "[6] 완제품 삭제",
            "[0] 메인 메뉴로 돌아가기"
        };

        // 대시보드 출력
        UIUtils::drawDashboard(infoLines, menu, 72, 30);

        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}


