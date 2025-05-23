#pragma once
#include <string>
#include <vector>

using namespace std;

// ���Ե� ����Ű ������ �����ϴ� Ŭ����
class BottledWhisky {
private:
    string productId;           // ��ǰ ID
    string productName;         // ��ǰ��
    string labelName;           // �󺧸�
    string batchNumber;         // ��ġ ��ȣ
    string exportTarget;        // ��� ���
    string oakId;               // ��ũ�� ID
    string shipmentDate;        // ��� ����
    string serialNumber;        // ���� ��ȣ
    string bottlingManager;     // ���� �����

    int bottleCount;            // ����
    double totalVolume;         // �� �뷮
    double pricePerBottle;      // ���� ����
    bool labeled;               // �� ���� ����

public:
    BottledWhisky(
        string productId, string name, string label, string batch, string target,
        string oakId, string shipmentDate, string serialNumber, string bottlingManager,
        int count,
        double volume, double price,
        bool labeled
    );

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
