#pragma once
#include <string>
#include <vector>

using namespace std;

// ���Ե� ����Ű ������ �����ϴ� Ŭ����
class BottledWhisky {
private:
    string productId, productName, labelName, batchNumber, exportTarget;
    string oakId, shipmentDate, serialNumber, bottlingManager;
    int bottleCount = 0;
    double totalVolume = 0.0, pricePerBottle = 0.0;
    bool labeled = false;

public:
    // ������
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

// ��� ��� Ŭ����
class ShipmentRecord {
private:
    string productName; // ��ǰ��
    string date; // ��� ��¥
    int quantity = 0; // ��� ����
    double totalPrice = 0.0; // �� ����

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

// ��ü ���� ����Ű ��� �� ��� ����� �����ϴ� Ŭ����
class BottledWhiskyManager {
private:
    vector<BottledWhisky> inventory; // ��� ���
    vector<ShipmentRecord> shipmentLog; // ��� ����

public:
    string getSummary(); // ��� ���� ��ȯ
    void showBottledWhiskyPage();     // ����
    void showInventory();      // ���� ��� ���
    void addWhisky();          // ���� ����Ű �߰�
    void shipWhisky();         // ��� ��� ���� �� ��� ����
    void showShipmentLog();    // ��� ��� ���
    void updateWhisky();       // ��ǰ ����
    void deleteWhisky();       // ��ǰ ����
	void exportInventoryToCSV(const string& filename); // ��� CSV�� ����
};
