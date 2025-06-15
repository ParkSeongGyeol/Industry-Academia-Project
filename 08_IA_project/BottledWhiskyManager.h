#pragma once
#include <string>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// [BottledWhisky] ���� ����Ű ���� Ŭ����
// -----------------------------------------------------------------------------
class BottledWhisky {
private:
    // ���� ����Ű �Ӽ�(ĸ��ȭ)
    string productId;         // ��ǰ ID
    string productName;       // ��ǰ��
    string labelName;         // �󺧸�
    string batchNumber;       // ��ġ ��ȣ
    string exportTarget;      // ��� ���
    string oakId;             // ��ũ�� ID
    string shipmentDate;      // ��� ����
    string serialNumber;      // ���� ��ȣ
    string bottlingManager;   // ���� �����
    int bottleCount = 0;      // ����(��)
    double totalVolume = 0.0; // �� �뷮(ml)
    double pricePerBottle = 0.0; // ���� ����
    bool labeled = false;     // �� ���� ����

public:
    BottledWhisky() = default;

    // ���� ���
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

    // ��� ����
    void decreaseStock(int count);
};

// -----------------------------------------------------------------------------
// [ShipmentRecord] ��� ��� Ŭ����
// -----------------------------------------------------------------------------
class ShipmentRecord {
private:
    string productName; // ��ǰ��
    string date;        // ��� ��¥
    int quantity = 0;   // ��� ����
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
    void setProductName(const string& name);
    void setDate(const string& d);
    void setQuantity(int q);
    void setTotalPrice(double price);
};

// -----------------------------------------------------------------------------
// [BottledWhiskyManager] ���� ����Ű ��ü ��� �� ��� ���� Ŭ����
// -----------------------------------------------------------------------------
class BottledWhiskyManager {
public:
    // [1] ������ �����
    void loadInventoryFromCSV(const string& filename); // CSV���� ���� ����Ű ��� �ε�
    void saveInventoryToCSV(const string& filename);   // ��ü inventory�� CSV�� ����

    // [2] ������ ��� ���� ����
    void produceBottledByRecipe(class RecipeManager& recipeMgr);

    // [3] ���� ���/��ȸ/���
    string getSummary();   // ��ú���� ��� ���ڿ� ��ȯ
    vector<string> getPageInfoLines(); // ���� ��� ���� ����
    void showInventory();      // ���� ��� ���
    void showShipmentLog();    // ��� ��� ���

    // [4] CSV ��������
    void exportInventoryToCSV(const string& filename); // ��� CSV�� ����

    // [5] �Է�/����/����/�˻�
    void addWhisky();          // ���� ����Ű �߰�
    void updateWhisky();       // ��ǰ ���� ����
    void deleteWhisky();       // ��ǰ ����
    void shipWhisky();         // ��� ��� ���� �� ��� ����

    // [6] ���� �޴�
    void showBottledWhiskyPage(); // �ܼ� �޴� ���� ����

    // [7] ESP32 ����
    void receiveWhiskyFromESP32(); // TCP�� ����

private:
    vector<BottledWhisky> inventory;      // ���� ����Ű ��� ���
    vector<ShipmentRecord> shipmentLog;   // ��� ����
};
