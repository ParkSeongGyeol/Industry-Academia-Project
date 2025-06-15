#pragma once
#include <string>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// [RawMaterial] ����� ���� Ŭ����
// -----------------------------------------------------------------------------
class RawMaterial {
private:
    // ������� �Ӽ�(ĸ��ȭ)
    string material_id;         // ����� ���� ID
    string name;                // ����� �̸�
    string type;                // ����� ����
    string origin;              // ������
    double weight_kg = 0.0;     // ����(kg)
    string storage_location;    // ���� ��ġ
    string storage_method;      // ���� ���
    string expiry_date;         // �������
    string entry_date;          // �԰���
    string exit_date;           // �����
    string status;              // ����(����/��� ��)
    string unit;                // ����(kg, L ��)
    double unit_price = 0.0;    // �ܰ�
    string entry_manager;       // �԰� �����
    string exit_manager;        // ��� �����
    string quality_check;       // ǰ�� �˻� ���
    string quality_check_date;  // ǰ�� �˻� ����

public:
    RawMaterial() = default;

    // Getter/Setter (�� �Ӽ���)
    string getMaterialId() const;
    string getName() const;
    string getType() const;
    string getOrigin() const;
    double getWeightKg() const;
    string getStorageLocation() const;
    string getStorageMethod() const;
    string getExpiryDate() const;
    string getEntryDate() const;
    string getExitDate() const;
    string getStatus() const;
    string getUnit() const;
    double getUnitPrice() const;
    string getEntryManager() const;
    string getExitManager() const;
    string getQualityCheck() const;
    string getQualityCheckDate() const;

    void setMaterialId(const string& val);
    void setName(const string& val);
    void setType(const string& val);
    void setOrigin(const string& val);
    void setWeightKg(double val);
    void setStorageLocation(const string& val);
    void setStorageMethod(const string& val);
    void setExpiryDate(const string& val);
    void setEntryDate(const string& val);
    void setExitDate(const string& val);
    void setStatus(const string& val);
    void setUnit(const string& val);
    void setUnitPrice(double val);
    void setEntryManager(const string& val);
    void setExitManager(const string& val);
    void setQualityCheck(const string& val);
    void setQualityCheckDate(const string& val);
};

// -----------------------------------------------------------------------------
// [RecipeManager] ������ ���� Ŭ���� (���� ����)
// -----------------------------------------------------------------------------
class RecipeManager; // ���� ���� �ʿ�

// -----------------------------------------------------------------------------
// [RawMaterialManager] ����� ��ü ��� �� ��� ���� Ŭ����
// -----------------------------------------------------------------------------
class RawMaterialManager {
public:
    // [1] ������ �����
    void loadMaterialsFromCSV(const string& filename); // CSV���� ����� ��� �ε�
    void saveMaterialsToCSV(const string& filename);   // ��ü materials�� CSV�� ����
    bool exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList); // ��� ��� ����

    // [2] ���� ����
    double getStock(const string& materialId) const; // ���� ��� ��ȸ
    void consumeMaterial(const string& name, double amount); // ��� ����
    void produceBatchByRecipe(RecipeManager& recipeMgr); // ������ ��� ��ġ ����

    // [3] ���� ���/��ȸ/���
    string getSummary();   // ��ú���� ��� ���ڿ� ��ȯ
    vector<string> getPageInfoLines(); // ���� ��� ���� ����
    void showInventory();       // ���� ���� ����Ḹ ���
    void showAllMaterials();    // ��ü ����� �̷� ���
    void showUninspectedMaterials(); // ǰ�� �˻� �̿Ϸ� ��� ��� ����
    void showMaterialsByManager(); // ����ں� ����� �̷� ����
    void showStorageEnvironment(); // ���� ��� ȯ�� ���� ���

    // [4] CSV ��������
    void exportUsedInventoryToCSV(); // ���� ����� CSV�� ����
    void exportRemainingStockToCSV(); // ������ ���� ��� CSV�� ����

    // [5] �Է�/����/����/�˻�
    void addMaterial();         // ����� �߰�
    void updateMaterial();      // ����� ���� ����
    void deleteMaterial();      // ��� ó��
    void searchMaterial();      // �̸����� ����� �˻�

    // [6] ���� �޴�
    void showRawMaterialPage(); // �ܼ� �޴� ���� ����

private:
    vector<RawMaterial> materials; // ����� ����Ʈ (�޸� �� ����)
};
