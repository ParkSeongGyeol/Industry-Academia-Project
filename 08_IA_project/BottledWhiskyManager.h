#pragma once
#include <string>
#include <vector>

using namespace std;

// ���Ե� ����Ű ������ �����ϴ� Ŭ����
class BottledWhisky {
private:
    string productName; // ��ǰ��

    int bottleCount; // ����

    double totalVolume; // �� �뷮
    double pricePerBottle; // ���� ����

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

    // ��� �� ��� ����
    void decreaseStock(int count);
};

// ��� ��� Ŭ����
class ShipmentRecord {
private:
    string productName; // ��ǰ��
    string date; // ��� ��¥

    int quantity; // ��� ����
    
    double totalPrice; // �� ����

public:
    ShipmentRecord
    (string name, string date,
        int qty,
        double price);

    void ShowInfo() const;
};

// ��ü ���� ����Ű ��� �� ��� ����� �����ϴ� Ŭ����
class BottledWhiskyManager {
private:
    vector<BottledWhisky> inventory; // ��� ���
    vector<ShipmentRecord> shipmentLog; // ��� ����

public:
    void BottledListRun();     // ����
    void showInventory();      // ���� ��� ���
    void addWhisky();          // ���� ����Ű �߰�
    void shipWhisky();         // ��� ��� ���� �� ��� ����
    void showShipmentLog();    // ��� ��� ���
};
