#pragma once
#include <string>
#include <vector>

using namespace std;

// ����� ������ ��� ����ü
struct RawMaterial {
    string material_id;           // ���� �ĺ���
    string name;                  // ��: ����, ȣ��, �� ��
    string type;                  // �, ��Ÿ ��
    string origin;                // ������, ��: ����Ʋ����, �̱�
    double weight_kg = 0.0;       // ���� ���� ���� ���� (kg)
    string storage_location;      // â�� A, ��ũ B ��
    string storage_method;        // ���� ����, û�� �к� ��
    string expiry_date;           // ��ȿ ��� ����
    string entry_date;            // ���� �԰���
    string exit_date;             // ��ġ�� ���Ե� ��¥
    string status;                // ����, �Թ�, ���, �ļ� ��
    string unit;                  // kg ����
    double unit_price = 0.0;      // kg �� �ܰ�
    string entry_manager;         // �԰� ����� ���
    string exit_manager;          // ��� ó�� �����
    string quality_check;         // �԰� �� ǰ�� Ȯ�� ����
    string quality_check_date;    // ǰ�� �˻� ����
};

// ����� ���� ����� ����ϴ� Ŭ����
class RawMaterialManager {
public:
    string getSummary();   // ��ú���� ��� ���ڿ� ��ȯ
    vector<string> getPageInfoLines(); // ���� ��� ���� ����

    void showInventory();       // ���� ���� ����Ḹ ���
    void showAllMaterials();    // ��ü ����� �̷� ���
    void showRawMaterialPage(); // �ܼ� �޴� ���� ����
    void addMaterial();         // ����� �߰�
    void updateMaterial();      // ����� ���� ����
    void deleteMaterial();      // ��� ó��
    void searchMaterial();      // �̸����� ����� �˻�
    void showStorageEnvironment(); // ���� ��� ȯ�� ���� ���
	void exportUsedInventoryToCSV(); // ���� ����� CSV�� ����
	void exportRemainingStockToCSV(); // ������ ���� ��� CSV�� ����
	void showUninspectedMaterials(); // ǰ�� �˻� �̿Ϸ� ��� ��� ����
	void showMaterialsByManager(); // ����ں� ����� �̷� ����
    double getStock(const string& name); // ���� ��� ��ȸ
    void consumeMaterial(const string& name, double amount); // ��� ����
    bool processFermentationBatch(double totalBatchKg); // ��ȿ ��ġ�� ����� ó��
    bool exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList); // ��� ��� ����

private:
    vector<RawMaterial> materials; // ����� ����Ʈ (�޸� �� ����)
    void initializeDummyData();        // �ʱ� ���� ������ ����
};
